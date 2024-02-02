#include<RestFunctions.h>
#include<charconv>

namespace rest {
	namespace data {
		namespace json = boost::json;
		namespace {
			int64_t readNumber(std::string_view s) {
				int64_t result;
				auto [ptr, ec] = std::from_chars(&*begin(s), &*end(s), result);
				if (ec == std::errc()) {
					return result;
				}
				throw net::RestError("Problem Parsing number: '" + std::string(s) + "'", net::RestError::Type::BAD_REQUEST);
			}
		}
		std::string upload_mod(net::HTTPHeaders const& headers, std::string body) {
			//TODO: Authenticate Headers
			json::object obj = json::parse(body).as_object();
			dTypes::ModData modData;
			modData.readFrom(obj);
			auto ret = db::upload_mod(modData);
			if (ret)
				return "Success";
			throw net::RestError("Internal Error Uploading Mod", net::RestError::Type::INTERNAL_ERROR);
		}
		std::string get_mod_metadata(net::HTTPHeaders const& headers) {
			auto name = headers.getHeader("name");
			auto version = headers.getHeader("version");
			auto modIdString = headers.getHeader("modid");
			std::optional<int64_t> modId;
			if (modIdString) {
				modId = readNumber(*modIdString);
			}

			auto ret = db::get_mod_metadata(modId, name, version);
			json::object obj;
			ret.writeTo(obj);
			return serialize(obj);
		}
		std::string get_units(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("unitname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_units(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_weapons(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("weaponname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_weapons(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_terrains(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("terrainname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_terrains(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_commanders(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("commandername");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_commanders(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_movements(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("movementname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_movements(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_players(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("playername");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_players(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_pues(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_pues(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_aues(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_aues(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_ptes(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_ptes(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_ates(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_ates(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_pges(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_pges(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_ages(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("effectname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_ages(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
		std::string get_settings(net::HTTPHeaders const& headers) {
			auto filter = headers.getHeader("settingname");
			auto modId = readNumber(headers.getHeader("modid").value());

			auto ret = db::get_settings(modId, filter);
			json::array arr;
			for (auto const& v : ret) {
				json::object obj;
				v.writeTo(obj);
				arr.emplace_back(std::move(obj));
			}
			return serialize(arr);
		}
	}
}