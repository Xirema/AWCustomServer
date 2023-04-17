#include<RestFunctions.h>
#include<boost/json.hpp>
#include<SQLUtil.h>
#include<boost/mysql.hpp>
#include<boost/algorithm/string.hpp>
#include<DataTypes.h>
#include<iostream>

namespace mysql = boost::mysql;
namespace {
	using ParameterPack = std::vector<mysql::field>;
	using Results = std::vector<mysql::row>;
	std::vector<mysql::field_view> asView(ParameterPack const& parameters) {
		std::vector<mysql::field_view> ret;
		for (auto const& field : parameters) {
			ret.emplace_back(field);
		}
		return ret;
	}

	Results getResults(
		mysql::tcp_ssl_connection& connection,
		mysql::execution_state& state
	) {
		Results results;
		while (!state.complete()) {
			auto rows = connection.read_some_rows(state);
			for (auto const& row : rows) {
				results.emplace_back(row);
			}
		}
		return results;
	}

	Results getLookup(
		mysql::tcp_ssl_connection& connection,
		std::string_view tableName,
		int64_t modId,
		std::optional<std::string_view> nameFilter = {}
	) {
		std::string sql = R"SQL(
			select 
				T.* 
			from 
				DATA.[TABLE] T
			where 
				T.MOD_ID = ?
		)SQL";
		ParameterPack parameters;
		parameters.emplace_back(modId);
		if (nameFilter) {
			sql += R"SQL(
			and T.NAME = ?
			)SQL";
			parameters.emplace_back(*nameFilter);
		}

		boost::replace_first(sql, "[TABLE]", tableName);

		auto statement = connection.prepare_statement(sql);
		mysql::execution_state state;
		auto parametersView = asView(parameters);
		connection.start_statement_execution(
			statement,
			parametersView.begin(),
			parametersView.end(),
			state
		);
		return getResults(connection, state);
	}

	Results getReferenceLookup(
		mysql::tcp_ssl_connection& connection,
		std::string_view referenceTableName, 
		std::string_view referenceColumn,
		int64_t modId, 
		std::optional<std::string_view> ownerNameFilter = {},
		std::optional<std::string_view> effectReferenceFilter = {},
		std::optional<std::string_view> sortColumn = {}
	) {
		std::string sql = R"SQL(
			select 
				T.* 
			from 
				DATA.[REFERENCETABLE] T
			where 
				T.MOD_ID = ?
		)SQL";
		ParameterPack parameters;
		parameters.emplace_back(modId);
		if (ownerNameFilter) {
			sql += R"SQL(
			and T.[REFERENCECOLUMN] = ?
			)SQL";
			parameters.emplace_back(*ownerNameFilter);
		}
		if (effectReferenceFilter) {
			sql += R"SQL(
			and T.EFFECT_TYPE = ?
			)SQL";
			parameters.emplace_back(*effectReferenceFilter);
		}
		if (sortColumn) {
			sql += R"SQL(
			order by [SORTCOLUMN] asc
			)SQL";
		}

		boost::replace_first(sql, "[REFERENCETABLE]", referenceTableName);
		boost::replace_first(sql, "[REFERENCECOLUMN]", referenceColumn);
		if (sortColumn) {
			boost::replace_first(sql, "[SORTCOLUMN]", *sortColumn);
		}

		auto statement = connection.prepare_statement(sql);
		auto parametersView = asView(parameters);
		mysql::execution_state state;
		connection.start_statement_execution(
			statement,
			parametersView.begin(),
			parametersView.end(),
			state
		);
		
		return getResults(connection, state);
	}

	std::optional<int64_t> getModId(mysql::tcp_ssl_connection& connection, boost::json::object const& json) {
		if (auto ret = json.if_contains("modId"); ret && ret->is_int64()) {
			return static_cast<int64_t>(ret->as_int64());
		}
		if (auto ret = json.if_contains("modId"); ret && ret->is_string()) {
			return static_cast<int64_t>(std::atoll(ret->as_string().c_str()));
		}
		std::string_view modName;
		std::optional<std::string_view> modVersion;
		if (auto ret = json.if_contains("modName"); ret && ret->is_string()) {
			modName = ret->as_string().c_str();
		}
		else {
			throw net::RestError("No ModId or ModName specified for lookup.", net::RestError::Type::INVALID_DATA);
		}
		if (auto ret = json.if_contains("modVersion"); ret && ret->is_string()) {
			modVersion = ret->as_string().c_str();
		}
		std::string sql = R"SQL(
			select
				ID
			from
				DATA.MOD M
			where
				NAME = ?
		)SQL";
		ParameterPack parameters;
		parameters.emplace_back(modName);
		if (modVersion) {
			sql += R"SQL(
			and VERSION = ?
			)SQL";
			parameters.emplace_back(*modVersion);
		}
		else {
			sql += R"SQL(
			and EXPIRED is null
			)SQL";
		}
		sql += R"SQL(
			order by ID desc
		)SQL";

		auto statement = connection.prepare_statement(sql);
		auto parametersView = asView(parameters);
		mysql::execution_state state;
		connection.start_statement_execution(
			statement,
			parametersView.begin(),
			parametersView.end(),
			state
		);
		Results results = getResults(connection, state);

		if (results.size() == 0) {
			return {};
		}
		return results.at(0).at(0).as_int64();
	}

	template<typename T>
	std::optional<T> get(mysql::field_view const& value) {
		if (value.is_null()) {
			return {};
		}
		if constexpr (std::is_same_v<int64_t, T>) {
			return value.as_int64();
		}
		else if constexpr (std::is_same_v<uint64_t, T>) {
			return value.as_uint64();
		}
		else if constexpr (std::is_same_v<std::string, T>) {
			return std::string{ value.as_string() };
		} 
		else if constexpr (std::is_same_v<int32_t, T>) {
			return static_cast<int32_t>(value.as_int64());
		}
		else if constexpr (std::is_same_v<uint32_t, T>) {
			return static_cast<uint32_t>(value.as_uint64());
		}
		throw std::runtime_error("Unsupported type in get<T>");
	}

	std::optional<bool> getBool(mysql::field_view const& value) {
		auto ret = get<int64_t>(value);
		if (ret)
			return *ret != 0;
		return {};
	}

	template<typename T>
	void set(T& var, mysql::field_view const& value) {
		auto opt = get<T>(value);
		if (!opt) {
			throw std::runtime_error("Null value for non-nullable field");
		}
		var = *opt;
	}

	template<typename T>
	void set(std::optional<T>& var, mysql::field_view const& value) {
		var = get<T>(value);
	}

	template<>
	void set<bool>(bool& var, mysql::field_view const& value) {
		auto opt = getBool(value);
		if (!opt) {
			throw std::runtime_error("Null value for non-nullable field");
		}
		var = *opt;
	}

	template<>
	void set<bool>(std::optional<bool>& var, mysql::field_view const& value) {
		var = getBool(value);
	}

	void assignUnitData(mysql::row_view const& row, datatypes::UnitType& unit) {
		set(unit.name, row[2]);
		set(unit.cost, row[3]);
		set(unit.maxFuel, row[4]);
		set(unit.maxAmmo, row[5]);
		set(unit.visionRange, row[6]);
		set(unit.movementClass, row[7]);
		set(unit.movementRange, row[8]);
		set(unit.fuelPerDay, row[9]);
		set(unit.fuelPerDayStealth, row[10]);
		set(unit.supplyRepair, row[11]);
		set(unit.transportCapacity, row[12]);
		set(unit.hitPoints, row[13]);
		set(unit.captureSpeed, row[14]);
		set(unit.ignoresVisionOcclusion, row[15]);
		set(unit.stealthType, row[16]);
	}

	void assignUnitWeaponData(mysql::row_view const& row, datatypes::UnitType& unit) {
		auto& newWeapon = unit.weapons->emplace_back();
		set(newWeapon, row[3]);
	}

	void assignUnitClassificationData(mysql::row_view const& row, datatypes::UnitType& unit) {
		auto& newClassification = unit.classifications.emplace_back();
		set(newClassification, row[3]);
	}

	void assignUnitTransportListData(mysql::row_view const& row, datatypes::UnitType& unit) {
		auto& newTransport = unit.transportList->emplace_back();
		set(newTransport, row[3]);
	}

	void assignWeaponData(mysql::row_view const& row, datatypes::WeaponType& weapon) {
		set(weapon.name, row[2]);
		set(weapon.ammoConsumed, row[3]);
		set(weapon.maxRange, row[4]);
		set(weapon.minRange, row[5]);
		set(weapon.selfTarget, row[6]);
		set(weapon.affectedByLuck, row[7]);
		set(weapon.nonLethal, row[8]);
		set(weapon.areaOfEffect, row[9]);
		set(weapon.flatDamage, row[10]);
	}

	void assignWeaponBaseDamageData(mysql::row_view const& row, datatypes::WeaponType& weapon) {
		std::string key;
		int64_t value;
		set(key, row[3]);
		set(value, row[4]);
		weapon.baseDamage->emplace(key, value);
	}

	void assignWeaponStealthTargetsData(mysql::row_view const& row, datatypes::WeaponType& weapon) {
		auto& newStealth = weapon.targetsStealth->emplace_back();
		set(newStealth, row[3]);
	}

	void assignTerrainData(mysql::row_view const& row, datatypes::TerrainType& terrain) {
		set(terrain.name,row[2]);
		set(terrain.stars,row[3]);
		set(terrain.maxCapturePoints,row[4]);
		set(terrain.sameAs,row[5]);
		set(terrain.income,row[6]);
		set(terrain.repair,row[7]);
		set(terrain.occludesVision,row[8]);
		set(terrain.hitPoints,row[9]);
		set(terrain.destroyed,row[10]);
		set(terrain.damagedLike,row[11]);
		set(terrain.activationMax,row[12]);
		set(terrain.activationChange,row[13]);
		set(terrain.loseIfCaptured,row[14]);
		set(terrain.loseIfAllCaptured,row[15]);
	}

	void assignTerrainBuildRepairData(mysql::row_view const& row, datatypes::TerrainType& terrain) {
		auto type = *get<int64_t>(row[4]);
		auto name = *get<std::string>(row[3]);
		if (type == 2) {
			if (!terrain.repairList)
				terrain.repairList.emplace();
			terrain.repairList->push_back(std::move(name));
		}
		else if (type == 1) {
			if (!terrain.buildList)
				terrain.buildList.emplace();
			terrain.buildList->push_back(std::move(name));
		}
	}

	void assignTerrainActivationListData(mysql::row_view const& row, datatypes::TerrainType& terrain) {
		auto unitName = *get<std::string>(row[3]);
		if (!terrain.activateList)
			terrain.activateList.emplace();
		terrain.activateList->push_back(unitName);
	}
	void assignTerrainActivationEffectData(mysql::row_view const& row, datatypes::TerrainType& terrain) {
		auto effectType = *get<std::string>(row[3]);
		auto effectName = *get<std::string>(row[4]);
		auto getEffectList = [&terrain, &effectType]() -> std::optional<std::vector<std::string>>&{
			if (effectType == "AUE")
				return terrain.activateActiveUnitEffects;
			if (effectType == "ATE")
				return terrain.activateActiveTerrainEffects;
			if (effectType == "AGE")
				return terrain.activateActiveGlobalEffects;
			throw std::exception("There was an invalid entry in the TERRAIN_ACTIVATIONEFFECT_REFERENCE table.");
		};
		auto& effectList = getEffectList();
		if (!effectList)
			effectList.emplace();
		effectList->push_back(effectName);
	}

	void assignMovementData(mysql::row_view const& row, datatypes::MovementClass& movement) {
		movement.name = *get<std::string>(row[2]);
	}

	void assignMovementCostData(mysql::row_view const& row, datatypes::MovementClass& movement) {
		auto variant = get<std::string>(row[5]);
		auto cost = *get<int64_t>(row[4]);
		auto terrain = *get<std::string>(row[3]);
		if (variant) {
			if (!movement.variantMods)
				movement.variantMods.emplace();
			(*movement.variantMods)[*variant][terrain] = cost;
		}
		else {
			movement.movementCosts[terrain] = cost;
		}
	}

	void assignPlayerData(mysql::row_view const& row, datatypes::PlayerType& player) {
		player.name = *get<std::string>(row[2]);
		player.commanderTypeMod = get<std::string>(row[3]);
		player.teamName = get<std::string>(row[4]);
	}

	void assignPlayerPermittedSlotData(mysql::row_view const& row, datatypes::PlayerType& player) {
		player.permittedPlayerSlots->push_back(*get<std::string>(row[3]));
	}

	void assignPlayerCommanderTypeData(mysql::row_view const& row, datatypes::PlayerType& player) {
		player.permittedCommanderTypes->push_back(*get<std::string>(row[3]));
	}

	template<typename T>
	void assignTargetData(mysql::row_view const& row, T& effect) {
		effect.targets->push_back(*get<std::string>(row[4]));
	}

	template<typename T>
	void assignAffectData(mysql::row_view const& row, T& effect) {
		effect.affects->push_back(*get<std::string>(row[4]));
	}

	template<typename T>
	void assignClassificationData(mysql::row_view const& row, T& effect) {
		effect.classificationRequired->push_back(*get<std::string>(row[4]));
	}

	template<typename T>
	void assignTerrainRequiredData(mysql::row_view const& row, T& effect) {
		effect.terrainRequired->push_back(*get<std::string>(row[4]));
	}

	template<typename T>
	void assignUnitRequiredData(mysql::row_view const& row, T& effect) {
		effect.unitTypeRequired->push_back(*get<std::string>(row[4]));
	}
	
	template<typename T>
	void getEffectTargets(mysql::tcp_ssl_connection& connection, T& effect, int64_t modId, std::string_view effectTypeCode) {
		auto results = getReferenceLookup(
			connection,
			"EFFECT_TARGET_REFERENCE",
			"EFFECT_NAME",
			modId,
			effect.name,
			effectTypeCode
		);
		if (results.size() > 0) {
			effect.targets.emplace();
			for (auto const& row : results) {
				assignTargetData(row, effect);
			}
		}
	}

	template<typename T>
	void getEffectAffects(mysql::tcp_ssl_connection& connection, T& effect, int64_t modId, std::string_view effectTypeCode) {
		auto results = getReferenceLookup(
			connection,
			"EFFECT_AFFECT_REFERENCE",
			"EFFECT_NAME",
			modId,
			effect.name,
			effectTypeCode
		);
		if (results.size() > 0) {
			effect.affects.emplace();
			for (auto const& row : results) {
				assignAffectData(row, effect);
			}
		}
	}

	template<typename T>
	void getEffectClassifications(mysql::tcp_ssl_connection& connection, T& effect, int64_t modId, std::string_view effectTypeCode) {
		auto results = getReferenceLookup(
			connection,
			"EFFECT_CLASSIFICATION_REFERENCE",
			"EFFECT_NAME",
			modId,
			effect.name,
			effectTypeCode
		);
		if (results.size() > 0) {
			effect.classificationRequired.emplace();
			for (auto const& row : results) {
				assignClassificationData(row, effect);
			}
		}
	}

	template<typename T>
	void getEffectTerrainRequired(mysql::tcp_ssl_connection& connection, T& effect, int64_t modId, std::string_view effectTypeCode) {
		auto results = getReferenceLookup(
			connection,
			"EFFECT_TERRAIN_REFERENCE",
			"EFFECT_NAME",
			modId,
			effect.name,
			effectTypeCode
		);
		if (results.size() > 0) {
			effect.terrainRequired.emplace();
			for (auto const& row : results) {
				assignTerrainRequiredData(row, effect);
			}
		}
	}

	template<typename T>
	void getEffectUnitRequired(mysql::tcp_ssl_connection& connection, T& effect, int64_t modId, std::string_view effectTypeCode) {
		auto results = getReferenceLookup(
			connection,
			"EFFECT_UNITTYPEREQUIRED_REFERENCE",
			"EFFECT_NAME",
			modId,
			effect.name,
			effectTypeCode
		);
		if (results.size() > 0) {
			effect.unitTypeRequired.emplace();
			for (auto const& row : results) {
				assignUnitRequiredData(row, effect);
			}
		}
	}

	void assignPUEData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		set(effect.name, row[2]);
		set(effect.firepowerMod, row[3]);
		set(effect.defenseMod, row[4]);
		set(effect.indirectDefenseMod, row[5]);
		set(effect.minRangeMod, row[6]);
		set(effect.maxRangeMod, row[7]);
		set(effect.fuelUseMod, row[8]);
		set(effect.ammoUseMod, row[9]);
		set(effect.goodLuckMod, row[10]);
		set(effect.badLuckMod, row[11]);
		set(effect.movementMod, row[12]);
		set(effect.visionMod, row[13]);
		set(effect.terrainStarsMod, row[14]);
		set(effect.terrainStarsFlatMod, row[15]);
		set(effect.terrainStarsDefense, row[16]);
		set(effect.terrainStarsFirepower, row[17]);
		set(effect.counterfireMod, row[18]);
		set(effect.counterFirst, row[19]);
		set(effect.captureRateMod, row[20]);
		set(effect.unitCostMod, row[21]);
		set(effect.hiddenHitPoints, row[22]);
		set(effect.firepowerFromFunds, row[23]);
		set(effect.defenseFromFunds, row[24]);
		set(effect.fundsFromDamage, row[25]);
		set(effect.coMeterChargeFromDealtDamage, row[26]);
		set(effect.coMeterChargeFromReceivedDamage, row[27]);
	}

	void assignPUEDefenseFromTerrainData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		effect.defenseFromOwnedTerrain->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEFirepowerFromTerrainData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		effect.firepowerFromOwnedTerrain->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEVisionVariantData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		effect.visionVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEFirepowerVariantData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		effect.firepowerVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEDefenseVariantData(mysql::row_view const& row, datatypes::PassiveUnitEffect& effect) {
		effect.defenseVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignAUEData(mysql::row_view const& row, datatypes::ActiveUnitEffect& effect) {
		set(effect.name, row[2]);
		set(effect.hitPointMod, row[3]);
		set(effect.roundHitPoints, row[4]);
		set(effect.resupply, row[5]);
		set(effect.halveFuel, row[6]);
		set(effect.makeActive, row[7]);
		set(effect.stunDuration, row[8]);
	}

	void assignPTEData(mysql::row_view const& row, datatypes::PassiveTerrainEffect& effect) {
		set(effect.name, row[2]);
		set(effect.incomeMod, row[3]);
		set(effect.incomeFlatMod, row[4]);
		set(effect.repairMod, row[5]);
		set(effect.occludesVisionMod, row[6]);
		set(effect.visionModBoost, row[7]);
		set(effect.buildCostMod, row[8]);
	}

	void assignPTEBuildListData(mysql::row_view const& row, datatypes::PassiveTerrainEffect& effect) {
		effect.buildListMod->push_back(*get<std::string>(row[3]));
	}

	void assignATEData(mysql::row_view const& row, datatypes::ActiveTerrainEffect& effect) {
		set(effect.name, row[2]);
		set(effect.unitSummonedName, row[3]);
		set(effect.unitSummonedInitialDamage, row[4]);
		set(effect.unitSummonedActive, row[5]);
	}

	void assignPGEData(mysql::row_view const& row, datatypes::PassiveGlobalEffect& effect) {
		set(effect.name, row[2]);
		set(effect.variantMod, row[3]);
		set(effect.movementClassVariantReplace, row[4]);
		set(effect.movementClassVariantOverride, row[5]);
	}

	void assignPGEVariantHintData(mysql::row_view const& row, datatypes::PassiveGlobalEffect& effect) {
		effect.variantHintMod->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignAGEData(mysql::row_view const& row, datatypes::ActiveGlobalEffect& effect) {
		set(effect.name, row[2]);
		set(effect.fundMod, row[3]);
		set(effect.fundFlatMod, row[4]);
		set(effect.powerBarMod, row[5]);
		set(effect.powerBarPerFunds, row[6]);
		set(effect.missileCount, row[7]);
		set(effect.missileDamage, row[8]);
		set(effect.missileAreaOfEffect, row[9]);
		set(effect.missileStunDuration, row[10]);
	}

	void assignAGEMissileTargetData(mysql::row_view const& row, datatypes::ActiveGlobalEffect& effect) {
		effect.missileTargetMethod->push_back(*get<std::string>(row[3]));
	}

	void assignCommanderData(mysql::row_view const& row, datatypes::CommanderType& commander) {
		set(commander.name, row[2]);
		set(commander.copCost, row[3]);
		set(commander.scopCost, row[4]);
		set(commander.coMeterMultiplier, row[5]);
		set(commander.playable, row[6]);
	}

	void assignCommanderEffectData(mysql::row_view const& row, datatypes::CommanderType& commander) {
		std::string effectType = *get<std::string>(row[4]);
		int32_t effectCategory = *get<int32_t>(row[5]);
		auto selector = [&]() -> std::optional<std::vector<std::string>>&{
			if (effectType == "PUE" && effectCategory == 0)
				return commander.passiveUnitEffectsD2d;
			if (effectType == "PUE" && effectCategory == 1)
				return commander.passiveUnitEffectsCop;
			if (effectType == "PUE" && effectCategory == 2)
				return commander.passiveUnitEffectsScop;
			if (effectType == "AUE" && effectCategory == 1)
				return commander.activeUnitEffectsCop;
			if (effectType == "AUE" && effectCategory == 2)
				return commander.activeUnitEffectsScop;
			if (effectType == "PTE" && effectCategory == 0)
				return commander.passiveTerrainEffectsD2d;
			if (effectType == "PTE" && effectCategory == 1)
				return commander.passiveTerrainEffectsCop;
			if (effectType == "PTE" && effectCategory == 2)
				return commander.passiveTerrainEffectsScop;
			if (effectType == "ATE" && effectCategory == 1)
				return commander.activeTerrainEffectsCop;
			if (effectType == "ATE" && effectCategory == 2)
				return commander.activeTerrainEffectsScop;
			if (effectType == "PGE" && effectCategory == 0)
				return commander.passiveGlobalEffectsD2d;
			if (effectType == "PGE" && effectCategory == 1)
				return commander.passiveGlobalEffectsCop;
			if (effectType == "PGE" && effectCategory == 2)
				return commander.passiveGlobalEffectsScop;
			if (effectType == "AGE" && effectCategory == 1)
				return commander.activeGlobalEffectsCop;
			if (effectType == "AGE" && effectCategory == 2)
				return commander.activeGlobalEffectsScop;
			throw std::exception("There was an invalid entry in the COMMANDER_EFFECT_REFERENCE table.");
		};

		auto& vec = selector();
		if (!vec) {
			vec.emplace();
		}
		vec->push_back(*get<std::string>(row[3]));
	}

	void assignSettingsData(mysql::row_view const& row, datatypes::Settings& setting) {
		set(setting.name, row[2]);
		set(setting.fogOfWar, row[3]);
		set(setting.coPowers, row[4]);
		set(setting.teams, row[5]);
		set(setting.startingFunds, row[6]);
		set(setting.incomeMultiplier, row[7]);
		set(setting.unitLimit, row[8]);
		set(setting.captureLimit, row[9]);
		set(setting.dayLimit, row[10]);
		set(setting.coMeterSize, row[11]);
		set(setting.coMeterMultiplier, row[12]);
	}

	void assignSettingsVariantData(mysql::row_view const& row, datatypes::Settings& setting) {
		setting.variant->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	std::string error_guard(net::POSTFunc func, net::HTTPHeaders const& headers, std::string body) {
		try {
			return func(headers, std::move(body));
		}
		catch (mysql::error_with_diagnostics const& e) {
			auto const& diagnostics = e.get_diagnostics();
			throw net::RestError("There was a problem with the SQL request to the database: " + std::string(e.what()) + ": " + std::string(diagnostics.server_message()), net::RestError::Type::INTERNAL_ERROR);
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
		catch (std::exception const& e) {
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

	std::string get_weapons_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> weaponName;
		if (auto ret = json.if_contains("weaponName"); ret && ret->is_string()) {
			weaponName = ret->as_string().c_str();
		}
		auto results = getLookup(
			session.connection,
			"WEAPON_TYPE",
			*modId,
			weaponName
		);
		std::vector<datatypes::WeaponType> weapons;
		for (auto const& row : results) {
			auto& newWeapon = weapons.emplace_back();
			assignWeaponData(row, newWeapon);
			auto baseDamageResults = getReferenceLookup(
				session.connection,
				"WEAPON_BASEDAMAGE_REFERENCE",
				"WEAPONTYPE_NAME",
				*modId,
				newWeapon.name
			);
			if (baseDamageResults.size() > 0) {
				newWeapon.baseDamage.emplace();
				for (auto const& baseDamageRow : baseDamageResults) {
					assignWeaponBaseDamageData(baseDamageRow, newWeapon);
				}
			}
			auto stealthTargetsResults = getReferenceLookup(
				session.connection,
				"WEAPON_STEALTHTARGET_REFERENCE",
				"WEAPONTYPE_NAME",
				*modId,
				newWeapon.name
			);
			if (stealthTargetsResults.size() > 0) {
				newWeapon.targetsStealth.emplace();
				for (auto const& stealthTargetsRow : stealthTargetsResults) {
					assignWeaponStealthTargetsData(stealthTargetsRow, newWeapon);
				}
			}
		}
		boost::json::array ret;
		for (auto const& weapon : weapons) {
			boost::json::object obj;
			weapon.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_units_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> unitName;
		if (auto ret = json.if_contains("unitName"); ret && ret->is_string()) {
			unitName = ret->as_string().c_str();
		}
		auto results = getLookup(session.connection, "UNIT_TYPE", *modId, unitName);
		std::vector<datatypes::UnitType> units;
		for (auto const& row : results) {
			auto& newUnit = units.emplace_back();
			assignUnitData(row, newUnit);
			auto weaponResults = getReferenceLookup(
				session.connection,
				"UNIT_WEAPON_REFERENCE",
				"UNITTYPE_NAME",
				*modId,
				newUnit.name,
				{},
				"WEAPONORDER"
			);
			if (weaponResults.size() > 0) {
				newUnit.weapons.emplace();
				for (auto const& weaponRow : weaponResults) {
					assignUnitWeaponData(weaponRow, newUnit);
				}
			}
			auto classificationResults = getReferenceLookup(
				session.connection,
				"UNIT_CLASSIFICATION_REFERENCE",
				"UNITTYPE_NAME",
				*modId,
				newUnit.name
			);
			for (auto const& classificationRow : classificationResults) {
				assignUnitClassificationData(classificationRow, newUnit);
			}
			auto transportListResults = getReferenceLookup(
				session.connection,
				"UNIT_TRANSPORTLIST_REFERENCE",
				"UNITTYPE_NAME",
				*modId,
				newUnit.name
			);
			if (transportListResults.size() > 0) {
				newUnit.transportList.emplace();
				for (auto const& transportListRow : transportListResults) {
					assignUnitTransportListData(transportListRow, newUnit);
				}
			}
		}
		boost::json::array ret;
		for (auto const& unit : units) {
			boost::json::object obj;
			unit.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_terrains_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> terrainName;
		if (auto ret = json.if_contains("terrainName"); ret && ret->is_string()) {
			terrainName = ret->as_string().c_str();
		}
		std::vector<datatypes::TerrainType> terrains;
		auto results = getLookup(
			session.connection,
			"TERRAIN_TYPE",
			*modId,
			terrainName
		);
		for (auto const& row : results) {
			auto& newTerrain = terrains.emplace_back();
			assignTerrainData(row, newTerrain);
			auto buildRepairResults = getReferenceLookup(
				session.connection,
				"TERRAIN_BUILDREPAIR_REFERENCE",
				"TERRAINTYPE_NAME",
				*modId,
				newTerrain.name,
				{},
				"UNITORDER"
			);
			for (auto const& buildRepairRow : buildRepairResults) {
				assignTerrainBuildRepairData(buildRepairRow, newTerrain);
			}

			auto activateListResults = getReferenceLookup(
				session.connection,
				"TERRAIN_ACTIVATIONLIST_REFERENCE",
				"TERRAINTYPE_NAME",
				*modId,
				newTerrain.name
			);
			for (auto const& activateListRow : activateListResults) {
				assignTerrainActivationListData(activateListRow, newTerrain);
			}

			auto activateEffectResults = getReferenceLookup(
				session.connection,
				"TERRAIN_ACTIVATIONEFFECT_REFERENCE",
				"TERRAINTYPE_NAME",
				*modId,
				newTerrain.name
			);
			for (auto const& activateEffectRow : activateEffectResults) {
				assignTerrainActivationEffectData(activateEffectRow, newTerrain);
			}
		}
		boost::json::array ret;
		for (auto const& terrain : terrains) {
			boost::json::object obj;
			terrain.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_commanders_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> commanderName;
		if (auto ret = json.if_contains("commanderName"); ret && ret->is_string()) {
			commanderName = ret->as_string().c_str();
		}
		std::vector<datatypes::CommanderType> commanders;
		auto results = getLookup(
			session.connection,
			"COMMANDER_TYPE",
			*modId,
			commanderName
		);
		for (auto const& row : results) {
			auto& newCommander = commanders.emplace_back();
			assignCommanderData(row, newCommander);
			auto commanderEffectResults = getReferenceLookup(
				session.connection,
				"COMMANDER_EFFECT_REFERENCE",
				"COMMANDER_NAME",
				*modId,
				newCommander.name
			);
			for (auto const& commanderEffectRow : commanderEffectResults) {
				assignCommanderEffectData(commanderEffectRow, newCommander);
			}
		}
		boost::json::array ret;
		for (auto const& commander : commanders) {
			boost::json::object obj;
			commander.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_movements_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> movementName;
		if (auto ret = json.if_contains("movementName"); ret && ret->is_string()) {
			movementName = ret->as_string().c_str();
		}
		std::vector<datatypes::MovementClass> movements;
		auto results = getLookup(
			session.connection,
			"MOVEMENT_TYPE",
			*modId,
			movementName
		);
		for (auto const& row : results) {
			auto& newMovement = movements.emplace_back();
			assignMovementData(row, newMovement);
			auto movementCostResults = getReferenceLookup(
				session.connection,
				"MOVEMENT_COST_REFERENCE",
				"MOVEMENT_NAME",
				*modId,
				newMovement.name
			);
			for (auto const& movementCostRow : movementCostResults) {
				assignMovementCostData(movementCostRow, newMovement);
			}
		}
		boost::json::array ret;
		for (auto const& movement : movements) {
			boost::json::object obj;
			movement.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_players_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> playerName;
		if (auto ret = json.if_contains("playerName"); ret && ret->is_string()) {
			playerName = ret->as_string().c_str();
		}
		std::vector<datatypes::PlayerType> players;
		auto results = getLookup(
			session.connection,
			"PLAYER_TYPE",
			*modId,
			playerName
		);
		for (auto const& row : results) {
			auto& newPlayer = players.emplace_back();
			assignPlayerData(row, newPlayer);
			auto permittedPlayerSlotsResults = getReferenceLookup(
				session.connection,
				"PLAYER_PERMITTEDPLAYERSLOT_REFERENCE",
				"PLAYERTYPE_NAME",
				*modId,
				newPlayer.name
			);
			if (permittedPlayerSlotsResults.size() > 0) {
				newPlayer.permittedPlayerSlots.emplace();
				for (auto const& permittedPlayerSlotRow : permittedPlayerSlotsResults) {
					assignPlayerPermittedSlotData(permittedPlayerSlotRow, newPlayer);
				}
			}

			auto permittedCommanderResults = getReferenceLookup(
				session.connection,
				"PLAYER_PERMITTEDCOMMANDERTYPE_REFERENCE",
				"PLAYERTYPE_NAME",
				*modId,
				newPlayer.name
			);
			if (permittedCommanderResults.size() > 0) {
				newPlayer.permittedCommanderTypes.emplace();
				for (auto const& permittedCommanderRow : permittedCommanderResults) {
					assignPlayerCommanderTypeData(permittedCommanderRow, newPlayer);
				}
			}
		}
		boost::json::array ret;
		for (auto const& player : players) {
			boost::json::object obj;
			player.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_pues_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::PassiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_UNIT_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignPUEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "PUE");
			getEffectUnitRequired(session.connection, newEffect, *modId, "PUE");
			getEffectClassifications(session.connection, newEffect, *modId, "PUE");
			getEffectTerrainRequired(session.connection, newEffect, *modId, "PUE");

			auto firepowerTerrainResults = getReferenceLookup(
				session.connection,
				"PUE_FIREPOWERFROMTERRAIN_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (firepowerTerrainResults.size() > 0) {
				newEffect.firepowerFromOwnedTerrain.emplace();
				for (auto const& sRow : firepowerTerrainResults) {
					assignPUEFirepowerFromTerrainData(sRow, newEffect);
				}
			}

			auto defenseTerrainResults = getReferenceLookup(
				session.connection,
				"PUE_DEFENSEFROMTERRAIN_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (defenseTerrainResults.size() > 0) {
				newEffect.defenseFromOwnedTerrain.emplace();
				for (auto const& sRow : defenseTerrainResults) {
					assignPUEDefenseFromTerrainData(sRow, newEffect);
				}
			}

			auto visionVariantResults = getReferenceLookup(
				session.connection,
				"PUE_VISIONVARIANT_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (visionVariantResults.size() > 0) {
				newEffect.visionVariantMods.emplace();
				for (auto const& sRow : visionVariantResults) {
					assignPUEVisionVariantData(sRow, newEffect);
				}
			}

			auto firepowerVariantResults = getReferenceLookup(
				session.connection,
				"PUE_FIREPOWERVARIANT_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (firepowerVariantResults.size() > 0) {
				newEffect.firepowerVariantMods.emplace();
				for (auto const& sRow : firepowerVariantResults) {
					assignPUEFirepowerVariantData(sRow, newEffect);
				}
			}

			auto defenseVariantResults = getReferenceLookup(
				session.connection,
				"PUE_DEFENSEVARIANT_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (defenseVariantResults.size() > 0) {
				newEffect.defenseVariantMods.emplace();
				for (auto const& sRow : defenseVariantResults) {
					assignPUEDefenseVariantData(sRow, newEffect);
				}
			}
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_aues_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::ActiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_UNIT_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignAUEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "AUE");
			getEffectUnitRequired(session.connection, newEffect, *modId, "AUE");
			getEffectClassifications(session.connection, newEffect, *modId, "AUE");
			getEffectTerrainRequired(session.connection, newEffect, *modId, "AUE");
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_ptes_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::PassiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_TERRAIN_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignPTEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "PTE");
			getEffectAffects(session.connection, newEffect, *modId, "PTE");
			getEffectClassifications(session.connection, newEffect, *modId, "PTE");
			getEffectTerrainRequired(session.connection, newEffect, *modId, "PTE");

			auto buildListResults = getReferenceLookup(
				session.connection,
				"PTE_BUILDLISTMOD_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (buildListResults.size() > 0) {
				newEffect.buildListMod.emplace();
				for (auto const& sRow : buildListResults) {
					assignPTEBuildListData(sRow, newEffect);
				}
			}
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_ates_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::ActiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_TERRAIN_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignATEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "ATE");
			getEffectAffects(session.connection, newEffect, *modId, "ATE");
			getEffectTerrainRequired(session.connection, newEffect, *modId, "ATE");
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_pges_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::PassiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_GLOBAL_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignPGEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "PGE");

			auto variantHintResults = getReferenceLookup(
				session.connection,
				"PGE_VARIANTHINTMOD_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (variantHintResults.size() > 0) {
				newEffect.variantHintMod.emplace();
				for (auto const& sRow : variantHintResults) {
					assignPGEVariantHintData(sRow, newEffect);
				}
			}
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_ages_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = json.if_contains("effectName"); ret && ret->is_string()) {
			effectName = ret->as_string().c_str();
		}
		std::vector<datatypes::ActiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_GLOBAL_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results) {
			auto& newEffect = effects.emplace_back();
			assignAGEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, *modId, "AGE");

			auto missileTargetResults = getReferenceLookup(
				session.connection,
				"AGE_MISSILETARGETMETHOD_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (missileTargetResults.size() > 0) {
				newEffect.missileTargetMethod.emplace();
				for (auto const& sRow : missileTargetResults) {
					assignAGEMissileTargetData(sRow, newEffect);
				}
			}
		}
		boost::json::array ret;
		for (auto const& effect : effects) {
			boost::json::object obj;
			effect.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_settings_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> settingsName;
		if (auto ret = json.if_contains("settingsName"); ret && ret->is_string()) {
			settingsName = ret->as_string().c_str();
		}
		std::vector<datatypes::Settings> settings;
		auto results = getLookup(
			session.connection,
			"DEFAULT_GAME_SETTINGS",
			*modId,
			settingsName
		);
		for (auto const& row : results) {
			auto& newSettings = settings.emplace_back();
			assignSettingsData(row, newSettings);

			auto settingsVariantResults = getReferenceLookup(
				session.connection,
				"DEFAULTSETTINGS_VARIANT_REFERENCE",
				"SETTING_NAME",
				*modId,
				newSettings.name
			);
			if (settingsVariantResults.size() > 0) {
				newSettings.variant.emplace();
				for (auto const& sRow : settingsVariantResults) {
					assignSettingsVariantData(sRow, newSettings);
				}
			}
		}
		boost::json::array ret;
		for (auto const& setting : settings) {
			boost::json::object obj;
			setting.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_mod_metadata_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find mod for provided input parameters", net::RestError::Type::INVALID_DATA);
		}
		std::string sql = R"SQL(
			select 
				*
			from 
				DATA.MOD
			where 
				ID = ?
		)SQL";
		auto statement = session.connection.prepare_statement(sql);
		mysql::results results;
		session.connection.execute_statement(statement, std::make_tuple(*modId), results);
		if (results.rows().size() > 0) {
			auto row = results.rows().at(0);
			boost::json::object ret;
			ret["modName"] = *get<std::string>(row[1]);
			ret["modId"] = std::to_string(*get<int64_t>(row[0]));
			ret["modVersion"] = *get<std::string>(row[2]);
			auto expiredOpt = get<std::string>(row[3]);
			if (expiredOpt)
				ret["expired"] = *expiredOpt;
			else
				ret["expired"] = nullptr;
			return boost::json::serialize(ret);
		}
		else {
			throw net::RestError("No Mod Found", net::RestError::Type::INVALID_DATA);
		}
	}

	std::string get_text_resources_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> key;
		if (auto name = json.if_contains("key")) {
			key.emplace(name->as_string().c_str());
		}
		std::optional<std::string_view> type;
		if (auto name = json.if_contains("type")) {
			type.emplace(name->as_string().c_str());
		}
		std::optional<std::string_view> language;
		if (auto name = json.if_contains("language")) {
			language.emplace(name->as_string().c_str());
		}
		std::map<std::tuple<std::string, std::string, std::optional<std::string>>, datatypes::TextResource> resources;
		
		std::string sql = R"SQL(
			select
				*
			from
				DATA.TEXT_RESOURCE
			where
				MOD_ID = ?
		)SQL";
		std::vector<mysql::field> parameterPack;
		parameterPack.emplace_back(*modId);
		if (key) {
			sql += R"SQL(
			and KEY = ?
			)SQL";
			parameterPack.emplace_back(*key);
		}
		if (type) {
			sql += R"SQL(
			and TYPE = ?
			)SQL";
			parameterPack.emplace_back(*type);
		}
		if (language) {
			sql += R"SQL(
			and LANGUAGE = ?
			)SQL";
			parameterPack.emplace_back(*language);
		}
		sql += R"SQL(
			order by `order` asc
		)SQL";
		auto statement = session.connection.prepare_statement(sql);
		mysql::execution_state state;
		std::vector<mysql::row> results;
		auto parameterPackView = asView(parameterPack);

		session.connection.start_statement_execution(
			statement,
			parameterPackView.begin(),
			parameterPackView.end(),
			state
		);

		while (!state.complete()) {
			auto newRows = session.connection.read_some_rows(state);
			for (auto const& row : newRows) {
				results.emplace_back(row);
			}
		}
		for (auto const& row : results) {
			std::string key, type;
			std::optional<std::string> language;

			set(key, row[2]);
			set(type, row[3]);
			set(language, row[7]);
			auto it = resources.find(std::make_tuple(key, type, language));
			datatypes::TextResource* ptr;
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

			std::string descPart;
			set(descPart, row[6]);
			newResource.description += descPart;
			set(newResource.shortName, row[4]);
			set(newResource.longName, row[5]);
		}
		boost::json::array ret;
		for (auto const& [key, resource] : resources) {
			boost::json::object obj;
			resource.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}

	std::string get_image_resources_impl(net::HTTPHeaders const& headers, std::string body) {
		sqlutil::Session session;
		auto json = boost::json::parse(body).as_object();
		auto modId = getModId(session.connection, json);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> key;
		if (auto name = json.if_contains("key")) {
			key.emplace(name->as_string().c_str());
		}
		std::optional<std::string_view> type;
		if (auto name = json.if_contains("type")) {
			type.emplace(name->as_string().c_str());
		}
		std::optional<std::string_view> army;
		if (auto name = json.if_contains("army")) {
			army.emplace(name->as_string().c_str());
		}
		std::map<std::tuple<std::string, std::string, std::optional<std::string>>, datatypes::ImageResource> resources;

		std::string sql = R"SQL(
			select
				*
			from
				DATA.IMAGE_RESOURCE
			where
				MOD_ID = ?
		)SQL";
		std::vector<mysql::field> parameterPack;
		parameterPack.emplace_back(*modId);
		if (key) {
			sql += R"SQL(
			and KEY = ?
			)SQL";
			parameterPack.emplace_back(*key);
		}
		if (type) {
			sql += R"SQL(
			and TYPE = ?
			)SQL";
			parameterPack.emplace_back(*type);
		}
		if (army) {
			sql += R"SQL(
			and ARMYCOLOR = ?
			)SQL";
			parameterPack.emplace_back(*army);
		}
		sql += R"SQL(
			order by `order` asc
		)SQL";
		auto statement = session.connection.prepare_statement(sql);
		std::vector<mysql::row> results;
		mysql::execution_state state;
		auto parameterPackView = asView(parameterPack);

		session.connection.start_statement_execution(
			statement,
			parameterPackView.begin(),
			parameterPackView.end(),
			state
		);

		while (!state.complete()) {
			auto newRows = session.connection.read_some_rows(state);
			for (auto const& row : newRows) {
				mysql::row newRow{ row };
				results.emplace_back(row);
			}
		}

		for (auto const& row : results) {
			std::string key, type;
			std::optional<std::string> armyColor;

			set(key, row[2]);
			set(type, row[3]);
			set(armyColor, row[4]);
			auto it = resources.find(std::make_tuple(key, type, armyColor));
			datatypes::ImageResource* ptr;
			if (it != resources.end()) {
				ptr = &it->second;
			}
			else {
				ptr = &resources[std::make_tuple(key, type, armyColor)];
			}
			auto& newResource = *ptr;
			newResource.key = key;
			newResource.type = type;
			newResource.armyColor = armyColor;

			std::optional<std::string> smallImagePart, largeImagePart;
			set(smallImagePart, row[5]);
			set(largeImagePart, row[6]);
			if (smallImagePart)
				newResource.smallImage += *smallImagePart;
			if (largeImagePart)
				newResource.largeImage += *largeImagePart;
			set(newResource.orientation, row[7]);
			set(newResource.variant, row[8]);
		}
		boost::json::array ret;
		for (auto const& [key, resource] : resources) {
			boost::json::object obj;
			resource.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}
}

std::string rest::data::get_weapons(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_weapons_impl, headers, std::move(body));
}

std::string rest::data::get_units(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_units_impl, headers, std::move(body));
}

std::string rest::data::get_terrains(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_terrains_impl, headers, std::move(body));
}

std::string rest::data::get_commanders(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_commanders_impl, headers, std::move(body));
}

std::string rest::data::get_movements(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_movements_impl, headers, std::move(body));
}

std::string rest::data::get_players(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_players_impl, headers, std::move(body));
}

std::string rest::data::get_pues(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_pues_impl, headers, std::move(body));
}

std::string rest::data::get_aues(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_aues_impl, headers, std::move(body));
}

std::string rest::data::get_ptes(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_ptes_impl, headers, std::move(body));
}

std::string rest::data::get_ates(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_ates_impl, headers, std::move(body));
}

std::string rest::data::get_pges(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_pges_impl, headers, std::move(body));
}

std::string rest::data::get_ages(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_ages_impl, headers, std::move(body));
}

std::string rest::data::get_settings(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_settings_impl, headers, std::move(body));
}

std::string rest::data::get_mod_metadata(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_mod_metadata_impl, headers, std::move(body));
}

std::string rest::data::get_text_resources(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_text_resources_impl, headers, std::move(body));
}
std::string rest::data::get_image_resources(net::HTTPHeaders const& headers, std::string body) {
	return error_guard(get_image_resources_impl, headers, std::move(body));
}