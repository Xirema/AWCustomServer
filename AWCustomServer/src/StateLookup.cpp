#include<RestFunctions.h>
#include<StateTypes.h>
#include<random>

namespace {
	std::string error_guard(net::POSTFunc func, net::HTTPHeaders const& headers, std::string body) {
		try {
			return func(headers, std::move(body));
		}
		catch (boost::system::system_error const& e) {
			throw net::RestError("There was a problem requesting the Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
		}
		catch (std::invalid_argument const& e) {
			throw net::RestError("There was a problem requesting the Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
		}
		catch (std::out_of_range const& e) {
			throw net::RestError("There was a problem requesting the Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
		}
		catch (std::runtime_error const& e) {
			throw net::RestError("Unknown Error: " + std::string(e.what()), net::RestError::Type::INTERNAL_ERROR);
		}
		catch (net::RestError const& e) {
			throw e;
		}
		catch (...) {
			try {
				std::rethrow_exception(std::current_exception());
			}
			catch (std::exception const& e) {
				throw net::RestError("Unknown Error: " + std::string(e.what()), net::RestError::Type::INTERNAL_ERROR);
			}
		}
	}

	int64_t get_id(std::string_view str) {
		return std::atoll(str.data());
	}

	std::string get_gamestate_impl(net::HTTPHeaders const& headers, std::string body) {
		auto gameId = get_id(boost::json::parse(body).at("gameId").as_string().c_str());
		if (gameId != -1) {
			throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
		}
		statetypes::GameState gameState;
		gameState.active = true;
		gameState.day = 1;
		gameState.id = "-1";
		gameState.playerOrder = { "1","2" };
		gameState.playerTurn = 0;
		gameState.variant = "normal";
		boost::json::object obj;
		gameState.writeTo(obj);
		return boost::json::serialize(obj);
	}

	std::string get_playerstates_impl(net::HTTPHeaders const& headers, std::string body) {
		auto gameId = get_id(boost::json::parse(body).at("gameId").as_string().c_str());
		if (gameId != -1) {
			throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
		}
		std::vector<statetypes::PlayerState> playerStates;
		auto& player1 = playerStates.emplace_back();
		player1.alive = true;
		player1.armyColor = "os";
		player1.commanderName = "max";
		player1.id = "9999";
		player1.owner = "1";
		player1.funds = 10000;
		player1.powerCharge = 0;
		player1.totalPowerUses = 1;
		player1.powerActive = "scop";
		player1.playerType = "normal";
		auto& player2 = playerStates.emplace_back();
		player2.alive = true;
		player2.armyColor = "bh";
		player2.commanderName = "sturm";
		player2.id = "10000";
		player2.owner = "2";
		player2.funds = 15000;
		player2.powerCharge = 63000;
		player2.totalPowerUses = 0;
		player2.playerType = "normal";

		boost::json::array arr;
		for (auto const& player : playerStates) {
			boost::json::object obj;
			player.writeTo(obj);
			if (player.owner == "2")
				obj["unitFacing"] = 1;
			arr.push_back(std::move(obj));
		}
		return boost::json::serialize(arr);
	}

	std::string get_unitstates_impl(net::HTTPHeaders const& headers, std::string body) {
		auto gameId = get_id(boost::json::parse(body).at("gameId").as_string().c_str());
		if (gameId != -1) {
			throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
		}
		std::vector<statetypes::UnitState> unitStates;
		for (int i = 0; i < 8; i++) {
			auto& unit = unitStates.emplace_back();
			unit.id = i + 1;
			unit.y = i;
			unit.x = 1;
			unit.active = true;
			unit.ammo = 0;
			unit.name = "infantry";
			unit.fuel = 99;
			unit.hitPoints = 100;
			unit.owner = "9999";
			if (unit.y == 3) {
				unit.name = "fighter";
			}
			if (unit.y == 4) {
				unit.name = "tank";
			}
		}
		for (int i = 0; i < 8; i++) {
			auto& unit = unitStates.emplace_back();
			unit.id = i + 9;
			unit.y = i;
			unit.x = 6;
			unit.active = true;
			unit.ammo = 0;
			unit.name = "infantry";
			unit.fuel = 99;
			unit.hitPoints = 100;
			unit.owner = "10000";
		}

		boost::json::array arr;
		for (auto const& unit : unitStates) {
			boost::json::object obj;
			unit.writeTo(obj);
			arr.push_back(std::move(obj));
		}
		return boost::json::serialize(arr);
	}

	std::string get_terrainstates_impl(net::HTTPHeaders const& headers, std::string body) {
		auto gameId = get_id(boost::json::parse(body).at("gameId").as_string().c_str());
		if (gameId != -1) {
			throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
		}
		std::vector<statetypes::TerrainState> terrainStates;
		int nextId = 0;
		std::minstd_rand engine{ static_cast<uint32_t>(nextId + 55) };
		std::uniform_int_distribution<uint32_t> dist{ 0, 6 };
		for (int i = 0; i < 50; i++) {
			for (int j = 0; j < 50; j++) {
				auto& terrain = terrainStates.emplace_back();
				terrain.id = nextId++;
				terrain.name = "plains";
				terrain.x = i;
				terrain.y = j;
				if (((terrain.y * 2) % 5) == (terrain.x % 5)) {
					terrain.name = "mountain";
				}
				if (terrain.name == "plains") {
					terrain.orientation = dist(engine);
				}
			}
		}

		boost::json::array arr;
		for (auto const& terrain : terrainStates) {
			boost::json::object obj;
			terrain.writeTo(obj);
			arr.push_back(std::move(obj));
		}
		return boost::json::serialize(arr);
	}

	std::string get_settingstate_impl(net::HTTPHeaders const& headers, std::string body) {
		auto gameId = get_id(boost::json::parse(body).at("gameId").as_string().c_str());
		if (gameId != -1) {
			throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
		}
		statetypes::SettingsState settingState;
		settingState.id = 1;
		settingState.fogOfWar = false;
		settingState.teams = false;
		settingState.variant = { {"normal", 100} };
		settingState.incomeMultiplier = 100;
		settingState.startingFunds = 0;
		settingState.coMeterMultiplier = 100;
		settingState.coMeterSize = 9000;
		settingState.coPowers = true;
		settingState.modId = "51";
		boost::json::object obj;
		settingState.writeTo(obj);
		return boost::json::serialize(obj);
	}
}

std::string rest::state::get_gamestate(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_gamestate_impl, headers, std::move(body));
}

std::string rest::state::get_playerstates(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_playerstates_impl, headers, std::move(body));
}

std::string rest::state::get_unitstates(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_unitstates_impl, headers, std::move(body));
}

std::string rest::state::get_terrainstates(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_terrainstates_impl, headers, std::move(body));
}

std::string rest::state::get_settingstate(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_settingstate_impl, headers, std::move(body));
}
