#include <Base64.h>
#include <boost/mysql.hpp>
#include <SQLUtil.h>
#include <ZipFunctions.h>
#include <boost/algorithm/string.hpp>
#include <format>
#include <DBFunctions.h>
#include <Error.h>

namespace
{
  namespace mysql = boost::mysql;
  using ParameterPack = std::vector<mysql::field>;

  mysql::blob toBlob(std::string const &s)
  {
    mysql::blob ret;
    ret.resize(s.size());
    std::ranges::transform(s, ret.begin(), [](char c)
                           { return static_cast<uint8_t>(c); });
    return ret;
  }
  std::string fromBlob(mysql::blob const &b)
  {
    std::string ret;
    ret.resize(b.size());
    std::ranges::transform(b, ret.begin(), [](uint8_t c)
                           { return static_cast<char>(c); });
    return ret;
  }

  void submitTextResources(
      std::vector<rTypes::TextResource> textResources,
      mysql::tcp_ssl_connection &connection,
      int64_t packId)
  {
    auto textResourceInsertStatement = connection.prepare_statement(R"SQL(
	    insert into
        RESOURCE.TEXT_RESOURCE
      (
        PACK_ID, `KEY`, `TYPE`, SHORTNAME, LONGNAME, DESCRIPTION, LANGUAGE
      )
      values
      (
        ?, ?, ?, ?, ?, ?, ?
      )
    )SQL");
    for (auto const &resource : textResources)
    {
      mysql::results results;

      connection.execute(
          textResourceInsertStatement.bind(
              packId,
              resource.key,
              resource.type,
              resource.shortName,
              resource.longName,
              resource.description,
              resource.language),
          results);
    }
  }

  void submitImageResources(
      std::vector<rTypes::ImageResource> imageResources,
      mysql::tcp_ssl_connection &connection,
      int64_t packId)
  {
    auto imageResourceInsertStatement = connection.prepare_statement(R"SQL(
	insert into
	  RESOURCE.IMAGE_RESOURCE
	(
	  PACK_ID, `KEY`, `TYPE`, SMALLIMAGE, LARGEIMAGE, ARMYCOLOR, `ORDER`, ORIENTATION, VARIANT
	)
	values
	(
	  ?, ?, ?, ?, ?, ?, ?, ?, ?
	)
      )SQL");
    for (auto const &resource : imageResources)
    {
      size_t i = 0;
      constexpr size_t BLOCK_SIZE = 512;
      do
      {
        auto getPart = [&](std::string const &str) -> std::optional<std::string>
        {
          if (i < str.size())
          {
            return std::string{
                str.begin() + i,
                (i + BLOCK_SIZE <= str.size()) ? (str.begin() + i + BLOCK_SIZE) : (str.end())};
          }
          if (i == 0)
          {
            return str;
          }
          return {};
        };
        auto smallPart = getPart(resource.smallImage);
        auto largePart = getPart(resource.largeImage);
        mysql::results results;
        connection.execute(
            imageResourceInsertStatement.bind(
                packId,
                resource.key,
                resource.type,
                smallPart,
                largePart,
                resource.armyColor,
                i / BLOCK_SIZE,
                resource.orientation,
                resource.variant),
            results);
        i += BLOCK_SIZE;
      } while (i < std::max(resource.smallImage.size(), resource.largeImage.size()));
    }
  }

  void submitImageResources2(
      std::vector<rTypes::ImageResource> imageResources,
      mysql::tcp_ssl_connection &connection,
      int64_t packId)
  {
    auto imageResourceInsertStatement = connection.prepare_statement(R"SQL(
      insert into
        RESOURCE.IMAGE_RESOURCE
      (
        PACK_ID, `KEY`, `TYPE`, SMALLIMAGEBLOB, LARGEIMAGEBLOB, ARMYCOLOR, ORIENTATION, VARIANT
      )
      values
      (
        ?, ?, ?, ?, ?, ?, ?, ?
      )
    )SQL");
    for (auto const &resource : imageResources)
    {
      mysql::blob smallImage = toBlob(resource.smallImage);
      mysql::blob largeImage = toBlob(resource.largeImage);
      mysql::results results;
      connection.execute(
          imageResourceInsertStatement.bind(
              packId,
              resource.key,
              resource.type,
              smallImage,
              largeImage,
              resource.armyColor,
              resource.orientation,
              resource.variant),
          results);
    }
  }

  int64_t submitPackMetadata(
      rTypes::PackMetadata const &metadata,
      mysql::tcp_ssl_connection &connection)
  {
    auto expireOldPacks = connection.prepare_statement(R"SQL(
	update
	  RESOURCE.PACK
	set
	  EXPIRED = current_timestamp
	where
	  NAME = ?
	  and EXPIRED is null
      )SQL");
    mysql::results results;
    connection.execute(
        expireOldPacks.bind(
            metadata.name),
        results);

    auto insertNewPackMetadata = connection.prepare_statement(R"SQL(
	insert into 
	  RESOURCE.PACK
	(NAME, VERSION, CREATED, PROTOCOL)
	values
	(
	  ?, ?, current_timestamp, ?
	)
      )SQL");

    results = {};
    connection.execute(
        insertNewPackMetadata.bind(
            metadata.name,
            metadata.version,
            metadata.protocol.value_or(0)
          ),
        results);
    int64_t packId = results.last_insert_id();
    return packId;
  }

  std::vector<rTypes::ImageResource> extractImages(ziputil::FileMap const &filemap)
  {
    std::vector<rTypes::ImageResource> ret;
    for (auto const &[fileName, fileData] : filemap)
    {
      if (fileName.ends_with(".json"))
      {
        continue;
      }
      std::vector<std::string_view> filenameParts;
      boost::split(filenameParts, fileName, [](char c)
                   { return c == '/'; });
      if (filenameParts.size() < 3)
      {
        throw net::RestError("Resource Pack Directory Structure is incorrect.", net::RestErrorType::INVALID_DATA);
      }
      auto &newImage = ret.emplace_back();
      newImage.type = std::string{filenameParts.at(0)};
      newImage.key = std::string{filenameParts.at(1)};
      if (filenameParts.size() >= 4)
      {
        if (filenameParts.at(2) != "neutral")
        {
          newImage.armyColor = std::string{filenameParts.at(2)};
        }
      }
      if (filenameParts.size() >= 5)
      {
        newImage.variant = std::string{filenameParts.at(4)};
      }
      auto orientation = filenameParts.back().substr(0, filenameParts.back().find('.'));
      newImage.orientation = std::strtol(orientation.data(), nullptr, 10);
      newImage.smallImage = fromBlob(fileData);
    }
    return ret;
  }

  std::vector<rTypes::TextResource> get_text_resources(int64_t packId)
  {
    std::vector<rTypes::TextResource> ret;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select
        `ID`, `PACK_ID`, `KEY`, `TYPE`, `SHORTNAME`, `LONGNAME`, `DESCRIPTION`, `LANGUAGE`
      from
        RESOURCE.TEXT_RESOURCE
      where
        PACK_ID = ?
    )SQL");

    mysql::results results;
    session.connection.execute(
        statement.bind(
            packId),
        results);
    for (auto const &row : results.rows())
    {
      auto& newText = ret.emplace_back();
      sqlutil::set(newText.key, row.at(2));
      sqlutil::set(newText.type, row.at(3));
      sqlutil::set(newText.shortName, row.at(4));
      sqlutil::set(newText.longName, row.at(5));
      sqlutil::set(newText.description, row.at(6));
      sqlutil::set(newText.language, row.at(7));
    }
    return ret;
  }

  std::vector<rTypes::ImageResource> get_image_resources(int64_t packId)
  {
    std::map<std::tuple<std::string, std::string, std::optional<std::string>, std::optional<int64_t>>, rTypes::ImageResource> resources;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select
        ID, PACK_ID, `KEY`, `TYPE`, ARMYCOLOR, SMALLIMAGE, LARGEIMAGE, ORIENTATION, VARIANT, `ORDER`
      from
        RESOURCE.IMAGE_RESOURCE
      where
        PACK_ID = ?
      order by
        `ORDER` asc
    )SQL");
    mysql::results results;
    session.connection.execute(
        statement.bind(
            packId),
        results);

    for (auto const &row : results.rows())
    {
      std::string key, type;
      std::optional<std::string> armyColor;
      std::optional<int64_t> orientation;

      key = row.at(2).as_string();
      type = row.at(3).as_string();
      if (auto val = row.at(4); !val.is_null())
      {
        armyColor = val.as_string();
      }
      if (auto val = row.at(7); !val.is_null())
      {
        orientation = val.as_int64();
      }
      auto it = resources.find(std::make_tuple(key, type, armyColor, orientation));
      rTypes::ImageResource *ptr;
      if (it != resources.end())
      {
        ptr = &it->second;
      }
      else
      {
        ptr = &resources[std::make_tuple(key, type, armyColor, orientation)];
      }
      auto &newResource = *ptr;
      newResource.key = key;
      newResource.type = type;
      newResource.armyColor = armyColor;
      newResource.orientation = orientation;

      std::optional<std::string> smallImagePart, largeImagePart;
      if (auto val = row.at(5); !val.is_null())
      {
        smallImagePart = val.as_string();
      }
      if (auto val = row.at(6); !val.is_null())
      {
        largeImagePart = val.as_string();
      }
      if (smallImagePart)
        newResource.smallImage += *smallImagePart;
      if (largeImagePart)
        newResource.largeImage += *largeImagePart;
      if (auto val = row.at(8); !val.is_null())
      {
        newResource.variant = val.as_string();
      }
    }
    std::vector<rTypes::ImageResource> ret;
    for (auto const &[key, resource] : resources)
    {
      ret.push_back(resource);
    }
    return ret;
  }

  std::vector<rTypes::ImageResource> get_image_resources2(uint64_t packId)
  {
    std::vector<rTypes::ImageResource> resources;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select
        ID, PACK_ID, `KEY`, `TYPE`, ARMYCOLOR, SMALLIMAGEBLOB, LARGEIMAGEBLOB, ORIENTATION, VARIANT
      from
        RESOURCE.IMAGE_RESOURCE
      where
        PACK_ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(
        statement.bind(
            packId),
        results);

    for (auto const &row : results.rows())
    {
      auto &newImageResource = resources.emplace_back();

      newImageResource.key = row.at(2).as_string();
      newImageResource.type = row.at(3).as_string();
      if (auto val = row.at(4); !val.is_null())
      {
        newImageResource.armyColor = val.as_string();
      }
      if (auto val = row.at(7); !val.is_null())
      {
        newImageResource.orientation = val.as_int64();
      }
      mysql::blob smallImageBlob;
      auto obj5 = row.at(5).as_blob();
      smallImageBlob.insert(smallImageBlob.end(), obj5.begin(), obj5.end());
      mysql::blob largeImageBlob;
      auto obj6 = row.at(6).as_blob();
      largeImageBlob.insert(largeImageBlob.end(), obj6.begin(), obj6.end());
      newImageResource.smallImage = base64::encode(smallImageBlob);
      newImageResource.largeImage = base64::encode(largeImageBlob);
    }
    return resources;
  }
}

bool db::upload_pack(rTypes::ResourcePack const &pack)
{
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};
  auto packId = submitPackMetadata(pack.packMetadata, session.connection);
  submitTextResources(pack.textResources, session.connection, packId);
  submitImageResources(pack.imageResources, session.connection, packId);
  transaction.commit();
  return true;
}

bool db::upload_pack2(std::vector<uint8_t> archiveFile)
{
  auto filemap = ziputil::readArchive(archiveFile);
  auto metadataAndText = filemap.at("text.json");
  auto metadataTextValue = fromBlob(metadataAndText);
  auto metadataTextObject = json::parse(metadataTextValue).as_object();
  rTypes::ResourcePack pack;
  pack.packMetadata = json::value_to<rTypes::PackMetadata>(metadataTextObject.at("packMetadata"));
  pack.packMetadata.protocol = 1;
  pack.textResources = json::value_to<std::vector<rTypes::TextResource>>(metadataTextObject.at("textResources"));
  pack.imageResources = extractImages(filemap);
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};
  auto packId = submitPackMetadata(pack.packMetadata, session.connection);
  submitTextResources(pack.textResources, session.connection, packId);
  submitImageResources2(pack.imageResources, session.connection, packId);
  transaction.commit();
  return true;
}

rTypes::ResourcePack db::get_resource_pack(int64_t packId)
{
  rTypes::ResourcePack ret;
  ret.packMetadata = get_pack_metadata({}, {}, packId);
  ret.textResources = get_text_resources(packId);
  if(ret.packMetadata.protocol == 0) {
    ret.imageResources = get_image_resources(packId);
  } else if(ret.packMetadata.protocol == 1) {
    ret.imageResources = get_image_resources2(packId);
  }
  return ret;
}
rTypes::PackMetadata db::get_pack_metadata(std::optional<std::string_view> name, std::optional<std::string_view> version, std::optional<int64_t> packId)
{
  sqlutil::Session session;
  if (!packId)
  {
    if (!name || !version)
    {
      throw net::RestError("Name or Version not specified", net::RestErrorType::BAD_REQUEST);
    }
    ParameterPack parameters;
    parameters.emplace_back(*name);
    std::string getPackIdSql = R"SQL(
      select 
        PACK_ID 
      from 
        RESOURCE.PACK 
      where 
        NAME = ?
    )SQL";
    if (version)
    {
      getPackIdSql += R"SQL(
        and VERSION = ?
	    )SQL";
      parameters.emplace_back(*version);
    }
    else
    {
      getPackIdSql += R"SQL( 
	      and EXPIRED is null
	    )SQL";
    }
    auto getPackIdStatement = session.connection.prepare_statement(getPackIdSql);
    mysql::results results;
    session.connection.execute(
        getPackIdStatement.bind(
            parameters.begin(),
            parameters.end()),
        results);
    if (results.size() == 0)
    {
      throw net::RestError(std::format("Unable to find Resource Pack named '{}'", *name), net::RestErrorType::NOT_FOUND);
    }
    packId = results.rows().at(0).at(0).as_int64();
  }
  rTypes::PackMetadata ret;
  auto getMetadataStatement = session.connection.prepare_statement(R"SQL(
    select 
      `PACK_ID`, `PROTOCOL`, `NAME`, `VERSION`, `CREATED`, `EXPIRED`
    from
      RESOURCE.PACK
    where
      PACK_ID = ?
  )SQL");

  mysql::results results;
  session.connection.execute(getMetadataStatement.bind(packId), results);
  if (results.size() == 0)
  {
    throw net::RestError(std::format("Unable to find Resource Pack for Pack Id {}", *packId), net::RestErrorType::NOT_FOUND);
  }
  for (auto const &row : results.rows())
  {
    ret.packId = *packId;
    sqlutil::set(ret.protocol, row.at(1));
    sqlutil::set(ret.name, row.at(2));
    sqlutil::set(ret.version, row.at(3));
    ret.created = sqlutil::dateToString(row.at(4).as_datetime());
    if(!row.at(5).is_null()) {
      ret.expired = sqlutil::dateToString(row.at(5).as_datetime());
    }
    return ret;
  }
  throw net::RestError("This shouldn't happen...!", net::RestErrorType::INTERNAL_ERROR);
}

std::vector<rTypes::PackMetadata> db::list_packs(bool includeOldPacks)
{
  sqlutil::Session session;
  auto &connection = session.connection;

  ParameterPack parameters;
  std::string getPacksSql = R"SQL(
    select 
      `PACK_ID`, `PROTOCOL`, `NAME`, `VERSION`, `CREATED`, `EXPIRED`
    from
      RESOURCE.PACK
    where
      1 = 1
  )SQL";
  if (!includeOldPacks)
  {
    getPacksSql += R"SQL(
      and EXPIRED is null
    )SQL";
  }
  auto getPacksStatement = connection.prepare_statement(getPacksSql);
  mysql::results results;
  connection.execute(
      getPacksStatement.bind(),
      results);

  std::vector<rTypes::PackMetadata> ret;
  for (auto const &row : results.rows())
  {
    auto& newPack = ret.emplace_back();
    sqlutil::set(newPack.packId, row.at(0));
    sqlutil::set(newPack.protocol, row.at(1));
    sqlutil::set(newPack.name, row.at(2));
    sqlutil::set(newPack.version, row.at(3));
    newPack.created = sqlutil::dateToString(row.at(4).as_datetime());
    if(!row.at(5).is_null()) {
      newPack.expired = sqlutil::dateToString(row.at(5).as_datetime());
    }
  }
  return ret;
}