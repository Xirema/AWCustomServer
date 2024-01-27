#include<RestFunctions.h>
#include<SQLUtil.h>
#include<jss/Resource.h>
#include<iostream>

namespace {
	namespace mysql = boost::mysql;
	using ParameterPack = std::vector<mysql::field>;
	void submitTextResources(std::vector<rTypes::TextResource> textResources, mysql::tcp_ssl_connection& connection, uint64_t packId) {
		auto textResourceInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				RESOURCE.TEXT_RESOURCE
				(
					PACK_ID, `KEY`, `TYPE`, SHORTNAME, LONGNAME, DESCRIPTION, LANGUAGE, `ORDER`
				)
			values
			(
				?, ?, ?, ?, ?, ?, ?, ?
			)
		)SQL");
		for (auto const& resource : textResources) {
			size_t i = 0;
			do {
				mysql::results results;
				
				std::string descPart{
					resource.description.begin() + i,
					(i + 512 <= resource.description.size()) ?
						(resource.description.begin() + i + 512) :
						(resource.description.end())
				};
				connection.execute(
					textResourceInsertStatement.bind(
						packId,
						resource.key,
						resource.type,
						resource.shortName,
						resource.longName,
						descPart,
						resource.language,
						i / 512
					)
					,
					results
				);
				i += 512;
			} while (i < resource.description.size());
		}
	}

	void submitImageResources(std::vector<rTypes::ImageResource> imageResources, mysql::tcp_ssl_connection& connection, uint64_t packId) {
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
		for (auto const& resource : imageResources) {
			size_t i = 0;
			constexpr size_t BLOCK_SIZE = 512;
			do {
				auto getPart = [&](std::string const& str) -> std::optional<std::string> {
					if (i < str.size()) {
						return std::string{
							str.begin() + i,
							(i + BLOCK_SIZE <= str.size()) ?
								(str.begin() + i + BLOCK_SIZE) :
								(str.end())
						};
					}
					if (i == 0) {
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
						resource.variant
					)
					,
					results
				);
				i += BLOCK_SIZE;
			} while (i < std::max(resource.smallImage.size(), resource.largeImage.size()));
		}
	}

	uint64_t submitPackMetadata(rTypes::PackMetadata const& metadata, mysql::tcp_ssl_connection& connection) {
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
				metadata.name
			)
			,
			results
		);

		auto insertNewPackMetadata = connection.prepare_statement(R"SQL(
			insert into 
				RESOURCE.PACK
				(NAME, VERSION, CREATED)
			values
			(
				?, ?, current_timestamp
			)
		)SQL");

		results = {};
		connection.execute(
			insertNewPackMetadata.bind(
				metadata.name,
				metadata.version
			)
			,
			results
		);
		uint64_t packId = results.last_insert_id();
		return packId;
	}

	std::string upload_pack_impl(net::HTTPHeaders const& headers, json::value const& packJson) {
		rTypes::ResourcePack pack;
		pack.readFrom(packJson.as_object());

		sqlutil::Session session;
		sqlutil::Transaction transaction(session);

		auto packId = submitPackMetadata(pack.packMetadata, session.connection);
		submitTextResources(pack.textResources, session.connection, packId);
		submitImageResources(pack.imageResources, session.connection, packId);

		transaction.commit();
		return "Success";
	}

	rTypes::PackMetadata get_metadata_impl(net::HTTPHeaders const& headers, mysql::tcp_ssl_connection& connection) {
		uint64_t packId;
		if (auto it = headers.httpHeaders.find("packid"); it != headers.httpHeaders.end()) {
			packId = std::stoull(it->second);
		}
		else {
			auto nameIt = headers.httpHeaders.find("name");
			auto versionIt = headers.httpHeaders.find("version");
			if (nameIt == headers.httpHeaders.end()) {
				throw net::RestError("Name or PackId not specified", net::RestError::Type::BAD_REQUEST);
			}
			std::string name = nameIt->second;
			std::optional<std::string> version;
			if (versionIt != headers.httpHeaders.end()) {
				version = versionIt->second;
			}
			ParameterPack parameters;
			parameters.emplace_back(name);
			std::string getPackIdSql = R"SQL(
				select 
					PACK_ID 
				from 
					RESOURCE.PACK 
				where 
					NAME = ?
			)SQL";
			if (version) {
				getPackIdSql += R"SQL( 
					and VERSION = ?
				)SQL";
				parameters.emplace_back(*version);
			}
			else {
				getPackIdSql += R"SQL( 
					and EXPIRED is null
				)SQL";
			}
			auto getPackIdStatement = connection.prepare_statement(getPackIdSql);
			mysql::results results;
			connection.execute(
				getPackIdStatement.bind(
					parameters.begin(), parameters.end()
				)
				,
				results
			);
			if (results.size() == 0) {
				throw net::RestError("Unable to find Resource Pack named '" + name + "'", net::RestError::Type::NOT_FOUND);
			}
			packId = results.rows().at(0).at(0).as_int64();
		}
		rTypes::PackMetadata ret;
		auto getMetadataStatement = connection.prepare_statement(R"SQL(
			select 
				*
			from
				RESOURCE.PACK
			where
				PACK_ID = ?
		)SQL");

		mysql::results results;
		connection.execute(
			getMetadataStatement.bind(
				packId
			)
			,
			results
		);
		if (results.size() == 0) {
			throw net::RestError("Unable to find Resource Pack for Pack Id " + std::to_string(packId), net::RestError::Type::NOT_FOUND);
		}
		for (auto const& row : results.rows()) {
			ret.packId = std::to_string(packId);
			ret.name = row.at(1).as_string();
			ret.version = row.at(2).as_string();
			mysql::datetime created = row.at(3).as_datetime();
			ret.created = std::to_string(created.as_time_point().time_since_epoch().count());
			break;
		}
		return ret;
	}

	std::vector<rTypes::TextResource> get_text_resources(uint64_t packId, mysql::tcp_ssl_connection& connection) {
		std::map<std::tuple<std::string, std::string, std::optional<std::string>>, rTypes::TextResource> resources;
		auto statement = connection.prepare_statement(R"SQL(
			select
				*
			from
				RESOURCE.TEXT_RESOURCE
			where
				PACK_ID = ?
			order by
				`ORDER` asc
		)SQL");

		mysql::results results;
		connection.execute(
			statement.bind(
				packId
			)
			,
			results
		);
		for (auto const& row : results.rows()) {
			std::string key, type;
			std::optional<std::string> language;
			key = row.at(2).as_string();
			type = row.at(3).as_string();
			if (!row.at(7).is_null()) {
				language = row.at(7).as_string();
			}
			auto it = resources.find(std::make_tuple(key, type, language));
			rTypes::TextResource* ptr;
			if (it != resources.end()) {
				ptr = &it->second;
			}
			else {
				ptr = &resources[std::make_tuple(key, type, language)];
			}
			auto& newResource = *ptr;
			newResource.key = key;
			newResource.type = type;
			newResource.language = language;

			std::string descPart = row.at(6).as_string();
			newResource.description += descPart;
			newResource.shortName = row.at(4).as_string();
			newResource.longName = row.at(5).as_string();
		}
		std::vector<rTypes::TextResource> ret;
		for (auto const& [key, resource] : resources) {
			ret.push_back(resource);
		}
		return ret;
	}

	std::vector<rTypes::ImageResource> get_image_resources(uint64_t packId, mysql::tcp_ssl_connection& connection) {
		std::map<std::tuple<std::string, std::string, std::optional<std::string>, std::optional<int64_t>>, rTypes::ImageResource> resources;

		auto statement = connection.prepare_statement(R"SQL(
			select
				*
			from
				RESOURCE.IMAGE_RESOURCE
			where
				PACK_ID = ?
			order by
				`ORDER` asc
		)SQL");
		mysql::results results;
		connection.execute(
			statement.bind(
				packId
			)
			,
			results
		);

		for (auto const& row : results.rows()) {
			std::string key, type;
			std::optional<std::string> armyColor;
			std::optional<int64_t> orientation;

			key = row.at(2).as_string();
			type = row.at(3).as_string();
			if (auto val = row.at(4); !val.is_null()) {
				armyColor = val.as_string();
			}
			if (auto val = row.at(7); !val.is_null()) {
				orientation = val.as_int64();
			}
			auto it = resources.find(std::make_tuple(key, type, armyColor, orientation));
			rTypes::ImageResource* ptr;
			if (it != resources.end()) {
				ptr = &it->second;
			}
			else {
				ptr = &resources[std::make_tuple(key, type, armyColor, orientation)];
			}
			auto& newResource = *ptr;
			newResource.key = key;
			newResource.type = type;
			newResource.armyColor = armyColor;
			newResource.orientation = orientation;

			std::optional<std::string> smallImagePart, largeImagePart;
			if (auto val = row.at(5); !val.is_null()) {
				smallImagePart = val.as_string();
			}
			if (auto val = row.at(6); !val.is_null()) {
				largeImagePart = val.as_string();
			}
			if (smallImagePart)
				newResource.smallImage += *smallImagePart;
			if (largeImagePart)
				newResource.largeImage += *largeImagePart;
			if (auto val = row.at(8); !val.is_null()) {
				newResource.variant = val.as_string();
			}
		}
		std::vector<rTypes::ImageResource> ret;
		for (auto const& [key, resource] : resources) {
			ret.push_back(resource);
		}
		return ret;
	}

	std::string get_pack_impl(net::HTTPHeaders const& headers) {
		rTypes::ResourcePack pack;
		uint64_t packId;
		if (auto it = headers.httpHeaders.find("packid"); it != headers.httpHeaders.end()) {
			packId = std::stoull(it->second);
		}
		else {
			throw net::RestError("Pack Id not specified", net::RestError::Type::NOT_FOUND);
		}
		sqlutil::Session session;
		pack.packMetadata = get_metadata_impl(headers, session.connection);
		pack.textResources = get_text_resources(packId, session.connection);
		pack.imageResources = get_image_resources(packId, session.connection);
		json::object obj;
		pack.writeTo(obj);
		return serialize(obj);
	}
}

std::string rest::resource::get_pack_metadata(net::HTTPHeaders const& headers) {
	sqlutil::Session session;
	rTypes::PackMetadata metadata = get_metadata_impl(headers, session.connection);
	json::object obj;
	metadata.writeTo(obj);
	return serialize(obj);
}

std::string rest::resource::get_resource_pack(net::HTTPHeaders const& headers) {
	return get_pack_impl(headers);
}

std::string rest::resource::upload_pack(net::HTTPHeaders const& headers, std::string body) {
	try {
		return upload_pack_impl(headers, json::parse(body));
	}
	catch (std::runtime_error const& e) {
		throw net::RestError("There was a problem parsing the Resource Pack: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
	}
}

std::string rest::resource::list_packs(net::HTTPHeaders const& headers) {
	sqlutil::Session session;
	auto& connection = session.connection;

	ParameterPack parameters;
	std::string getPacksSql = R"SQL(
		select 
			*
		from
			RESOURCE.PACK
		where
			1 = 1
	)SQL";
	if (auto it = headers.httpHeaders.find("include-old-packs"); it == headers.httpHeaders.end() || it->second == "false") {
		getPacksSql += R"SQL(
			and EXPIRED is null
		)SQL";
	}
	auto getPacksStatement = connection.prepare_statement(getPacksSql);
	mysql::results results;
	connection.execute(
		getPacksStatement.bind()
		,
		results
	);

	json::array ret;
	for (auto const& row : results.rows()) {
		rTypes::PackMetadata newPack;
		newPack.packId = std::to_string(row.at(0).as_int64());
		newPack.name = row.at(1).as_string();
		newPack.version = row.at(2).as_string();
		mysql::datetime created = row.at(3).as_datetime();
		newPack.created = std::to_string(created.as_time_point().time_since_epoch().count());
		json::object obj;
		newPack.writeTo(obj);
		ret.push_back(std::move(obj));
	}
	return serialize(ret);
}