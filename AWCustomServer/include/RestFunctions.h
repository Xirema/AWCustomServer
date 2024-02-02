#pragma once
#include<RestServer.h>

#include<jss/Commander.h>
#include<jss/DefaultResourcePack.h>
#include<jss/Effect.h>
#include<jss/GameState.h>
#include<jss/ModMetadata.h>
#include<jss/Movement.h>
#include<jss/PlayerState.h>
#include<jss/Resource.h>
#include<jss/Settings.h>
#include<jss/SettingsState.h>
#include<jss/Terrain.h>
#include<jss/TerrainState.h>
#include<jss/Unit.h>
#include<jss/UnitState.h>

namespace rest {
	namespace data {
		std::string upload_mod(net::HTTPHeaders const& headers, std::string body);
		std::string get_units(net::HTTPHeaders const& headers);
		std::string get_weapons(net::HTTPHeaders const& headers);
		std::string get_terrains(net::HTTPHeaders const& headers);
		std::string get_commanders(net::HTTPHeaders const& headers);
		std::string get_movements(net::HTTPHeaders const& headers);
		std::string get_players(net::HTTPHeaders const& headers);
		std::string get_pues(net::HTTPHeaders const& headers);
		std::string get_aues(net::HTTPHeaders const& headers);
		std::string get_ptes(net::HTTPHeaders const& headers);
		std::string get_ates(net::HTTPHeaders const& headers);
		std::string get_pges(net::HTTPHeaders const& headers);
		std::string get_ages(net::HTTPHeaders const& headers);
		std::string get_settings(net::HTTPHeaders const& headers);
		std::string get_mod_metadata(net::HTTPHeaders const& headers);
	}

	namespace state {
		std::string get_gamestate(net::HTTPHeaders const& headers);
		std::string get_playerstates(net::HTTPHeaders const& headers);
		std::string get_unitstates(net::HTTPHeaders const& headers);
		std::string get_terrainstates(net::HTTPHeaders const& headers);
		std::string get_settingstate(net::HTTPHeaders const& headers);
	}

	namespace resource {
		std::string upload_pack(net::HTTPHeaders const& headers, std::string body);
		std::string get_resource_pack(net::HTTPHeaders const& headers);
		std::string get_pack_metadata(net::HTTPHeaders const& headers);
		std::string list_packs(net::HTTPHeaders const& headers);
	}
}

namespace db {
	bool upload_mod(dTypes::ModData const& mod);
	std::vector<dTypes::UnitType> get_units(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::WeaponType> get_weapons(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::TerrainType> get_terrains(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::CommanderType> get_commanders(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::MovementClass> get_movements(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::PlayerType> get_players(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::PassiveUnitEffect> get_pues(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::ActiveUnitEffect> get_aues(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::PassiveTerrainEffect> get_ptes(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::ActiveTerrainEffect> get_ates(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::PassiveGlobalEffect> get_pges(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::ActiveGlobalEffect> get_ages(int64_t modId, std::optional<std::string_view> const& filter);
	std::vector<dTypes::Settings> get_settings(int64_t modId, std::optional<std::string_view> const& filter);
	dTypes::ModMetadata get_mod_metadata(std::optional<int64_t> const& modId, std::optional<std::string_view> const& name, std::optional<std::string_view> const& version);
	sTypes::GameState get_gamestate(int64_t gameId);
	std::vector<sTypes::PlayerState> get_playerstates(int64_t gameId);
	std::vector<sTypes::UnitState> get_unitstates(int64_t gameId);
	std::vector<sTypes::TerrainState> get_terrainstates(int64_t gameId);
	std::vector<sTypes::SettingsState> get_settingstate(int64_t gameId);
	bool upload_pack(rTypes::ResourcePack const& pack);
	rTypes::ResourcePack get_resource_pack(int64_t packId);
	rTypes::PackMetadata get_pack_metadata(std::optional<std::string_view> name, std::optional<std::string_view> version, std::optional<int64_t> packId);
	std::vector<rTypes::PackMetadata> list_packs(bool includeOldPacks);
	/*template<typename T, typename ... Parameters>
	T errorGuard(T(*func)(Parameters&&...), Parameters&& ...p) {
		try {
			return func(std::forward<Parameters...>(p...));
		}
		catch () {
		}
	}*/
}