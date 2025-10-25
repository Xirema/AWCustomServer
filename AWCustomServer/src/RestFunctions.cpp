#include <RestFunctions.h>
#include <charconv>
#include<GameManager.h>
#include<DBFunctions.h>

namespace rest
{
  namespace data
  {
    namespace json = boost::json;
    namespace
    {
      int64_t readNumber(std::string_view s)
      {
        int64_t result;
        auto [ptr, ec] = std::from_chars(&*begin(s), &*end(s), result);
        if (ec == std::errc())
        {
          return result;
        }
        throw net::RestError("Problem Parsing number: '" + std::string(s) + "'", net::RestErrorType::BAD_REQUEST);
      }
    }
    std::string upload_mod(net::HTTPHeaders const &headers, std::string body)
    {
      // TODO: Authenticate Headers
      auto modData = json::value_to<dTypes::ModData>(json::parse(body));
      auto ret = db::upload_mod(modData);
      if (ret)
        return "Success";
      throw net::RestError("Internal Error Uploading Mod", net::RestErrorType::INTERNAL_ERROR);
    }
    std::string get_mod_metadata(net::HTTPHeaders const &headers)
    {
      auto name = headers.getHeader("name");
      auto version = headers.getHeader("version");
      auto modIdString = headers.getHeader("modid");
      std::optional<int64_t> modId;
      if (modIdString)
      {
        modId = readNumber(*modIdString);
      }

      auto ret = db::get_mod_metadata(modId, name, version);
      return serialize(json::value_from(ret));
    }
    std::string get_mods(net::HTTPHeaders const &headers)
    {
      auto nameFilter = headers.getHeader("namefilter");
      auto authorFilter = headers.getHeader("authorfilter");
      auto showExpiredString = headers.getHeader("showexpired");
      std::optional<bool> showExpired;
      if (showExpiredString == "true")
      {
        showExpired = true;
      }

      auto ret = db::get_mods(showExpired, nameFilter, authorFilter);
      return serialize(json::value_from(ret));
    }
    std::string get_units(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("unitname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_units(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_weapons(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("weaponname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_weapons(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_terrains(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("terrainname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_terrains(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_commanders(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("commandername");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_commanders(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_movements(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("movementname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_movements(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_movement_rules(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("rulename");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_movement_rules(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_players(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("playername");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_players(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_pues(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_pues(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_aues(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_aues(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_ptes(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_ptes(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_ates(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_ates(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_pges(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_pges(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_ages(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("effectname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_ages(modId, filter);
      return serialize(json::value_from(ret));
    }
    std::string get_settings(net::HTTPHeaders const &headers)
    {
      auto filter = headers.getHeader("settingname");
      auto modId = readNumber(headers.getHeader("modid").value());

      auto ret = db::get_settings(modId, filter);
      return serialize(json::value_from(ret));
    }
		std::string get_mod_config(net::HTTPHeaders const& headers) {
      auto modId = readNumber(headers.getHeader("modid").value());
      auto ret = db::get_mod_config(modId);
      return serialize(json::value_from(ret));
    }
		std::string get_mod(net::HTTPHeaders const& headers) {
      auto modId = readNumber(headers.getHeader("modid").value());
      game::GameManager & manager = game::GameManager::instance();
      auto mod = manager.getMod(modId);
      return serialize(json::value_from(mod->object));
    }
  }
}
