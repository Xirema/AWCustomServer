#include<RestFunctions.h>
#include<boost/json.hpp>
#include<SQLUtil.h>
#include<boost/mysql.hpp>
#include<boost/algorithm/string.hpp>
#include<iostream>
#include<jss/ModMetadata.h>
#include<jss/Commander.h>
#include<jss/Effect.h>
#include<jss/Movement.h>
#include<jss/Settings.h>
#include<jss/Terrain.h>
#include<jss/Unit.h>

namespace mysql = boost::mysql;
namespace {
	using ParameterPack = std::vector<mysql::field>;

	mysql::results getLookup(
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
		mysql::results ret;
		connection.execute(
			statement.bind(
				parameters.cbegin(),
				parameters.cend()
			)
			,
			ret
		);
		return ret;
	}

	mysql::results getReferenceLookup(
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
		mysql::results ret;
		connection.execute(
			statement.bind(
				parameters.cbegin(),
				parameters.cend()
			)
			,
			ret
		);
		return ret;
	}

	std::optional<int64_t> getModId(mysql::tcp_ssl_connection& connection, net::HTTPHeaders const& headers) {
		if(auto ret = headers.httpHeaders.find("modid"); ret != headers.httpHeaders.end()) {
			return std::stoll(ret->second);
		}
		std::string_view modName;
		std::optional<std::string_view> modVersion;
		if (auto ret = headers.httpHeaders.find("modname"); ret != headers.httpHeaders.end()) {
			modName = ret->second;
		}
		else {
			throw net::RestError("No ModId or ModName specified for lookup.", net::RestError::Type::INVALID_DATA);
		}
		if (auto ret = headers.httpHeaders.find("modversion"); ret != headers.httpHeaders.end()) {
			modVersion = ret->second;
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
		mysql::results results;
		connection.execute(
			statement.bind(
				parameters.cbegin(),
				parameters.cend()
			)
			,
			results
		);
		if (results.size() == 0) {
			return {};
		}
		return results.rows().at(0).at(0).as_int64();
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
		else if constexpr (std::is_same_v<bool, T>) {
			return static_cast<bool>(value.as_int64());
		}
		throw std::runtime_error("Unsupported type in get<T>");
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

	void assignUnitData(mysql::row_view const& row, dTypes::UnitType& unit) {
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
		set(unit.stationaryFire, row[17]);
	}

	void assignUnitWeaponData(mysql::row_view const& row, dTypes::UnitType& unit) {
		auto& newWeapon = unit.weapons->emplace_back();
		set(newWeapon, row[3]);
	}

	void assignUnitClassificationData(mysql::row_view const& row, dTypes::UnitType& unit) {
		auto& newClassification = unit.classifications.emplace_back();
		set(newClassification, row[3]);
	}

	void assignUnitTransportListData(mysql::row_view const& row, dTypes::UnitType& unit) {
		auto& newTransport = unit.transportList->emplace_back();
		set(newTransport, row[3]);
	}

	void assignWeaponData(mysql::row_view const& row, dTypes::WeaponType& weapon) {
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

	void assignWeaponBaseDamageData(mysql::row_view const& row, dTypes::WeaponType& weapon) {
		std::string key;
		int64_t value;
		set(key, row[3]);
		set(value, row[4]);
		weapon.baseDamage->emplace(key, value);
	}

	void assignWeaponStealthTargetsData(mysql::row_view const& row, dTypes::WeaponType& weapon) {
		auto& newStealth = weapon.targetsStealth->emplace_back();
		set(newStealth, row[3]);
	}

	void assignTerrainData(mysql::row_view const& row, dTypes::TerrainType& terrain) {
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
		set(terrain.destroyedOrientation, row[16]);
	}

	void assignTerrainBuildRepairData(mysql::row_view const& row, dTypes::TerrainType& terrain) {
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

	void assignTerrainActivationListData(mysql::row_view const& row, dTypes::TerrainType& terrain) {
		auto unitName = *get<std::string>(row[3]);
		if (!terrain.activateList)
			terrain.activateList.emplace();
		terrain.activateList->push_back(unitName);
	}
	void assignTerrainActivationEffectData(mysql::row_view const& row, dTypes::TerrainType& terrain) {
		auto effectType = *get<std::string>(row[3]);
		auto effectName = *get<std::string>(row[4]);
		auto getEffectList = [&terrain, &effectType]() -> std::optional<std::vector<std::string>>&{
			if (effectType == "AUE")
				return terrain.activateActiveUnitEffects;
			if (effectType == "ATE")
				return terrain.activateActiveTerrainEffects;
			if (effectType == "AGE")
				return terrain.activateActiveGlobalEffects;
			throw std::runtime_error("There was an invalid entry in the TERRAIN_ACTIVATIONEFFECT_REFERENCE table.");
		};
		auto& effectList = getEffectList();
		if (!effectList)
			effectList.emplace();
		effectList->push_back(effectName);
	}

	void assignMovementData(mysql::row_view const& row, dTypes::MovementClass& movement) {
		movement.name = *get<std::string>(row[2]);
	}

	void assignMovementCostData(mysql::row_view const& row, dTypes::MovementClass& movement) {
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

	void assignPlayerData(mysql::row_view const& row, dTypes::PlayerType& player) {
		player.name = *get<std::string>(row[2]);
		player.commanderTypeMod = get<std::string>(row[3]);
		player.teamName = get<std::string>(row[4]);
	}

	void assignPlayerPermittedSlotData(mysql::row_view const& row, dTypes::PlayerType& player) {
		player.permittedPlayerSlots->push_back(*get<std::string>(row[3]));
	}

	void assignPlayerCommanderTypeData(mysql::row_view const& row, dTypes::PlayerType& player) {
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
			for (auto const& row : results.rows()) {
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
			for (auto const& row : results.rows()) {
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
			for (auto const& row : results.rows()) {
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
			for (auto const& row : results.rows()) {
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
			for (auto const& row : results.rows()) {
				assignUnitRequiredData(row, effect);
			}
		}
	}

	void assignPUEData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
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
		set(effect.firepowerFromFunds, row[22]);
		set(effect.defenseFromFunds, row[23]);
		set(effect.fundsFromDamage, row[24]);
		set(effect.coMeterChargeFromDealtDamage, row[25]);
		set(effect.coMeterChargeFromReceivedDamage, row[26]);
	}

	void assignPUEDefenseFromTerrainData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		effect.defenseFromOwnedTerrain->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEFirepowerFromTerrainData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		effect.firepowerFromOwnedTerrain->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEVisionVariantData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		effect.visionVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEFirepowerVariantData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		effect.firepowerVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEDefenseVariantData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		effect.defenseVariantMods->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignPUEIntelData(mysql::row_view const& row, dTypes::PassiveUnitEffect& effect) {
		auto intelType = *get<std::string>(row[3]);
		auto selector = [&](std::string const& intelType) -> std::optional<std::vector<std::string>>&{
			if (intelType == "hiddenHitPoints")
				return effect.hiddenHitPoints;
			if (intelType == "luckPointsVisible")
				return effect.luckPointsVisible;
			if (intelType == "hpPartVisible")
				return effect.hpPartVisible;
			throw std::runtime_error("Invalid entry in PUE_INTEL_REFERENCE table for INTELTYPE");
		};
		auto& intelOpt = selector(intelType);
		if (!intelOpt) {
			intelOpt.emplace();
		}
		intelOpt->push_back(*get<std::string>(row[4]));
	}

	void assignAUEData(mysql::row_view const& row, dTypes::ActiveUnitEffect& effect) {
		set(effect.name, row[2]);
		set(effect.hitPointMod, row[3]);
		set(effect.roundHitPoints, row[4]);
		set(effect.resupply, row[5]);
		set(effect.halveFuel, row[6]);
		set(effect.makeActive, row[7]);
		set(effect.stunDuration, row[8]);
		set(effect.coChargeFactor, row[9]);
	}

	void assignPTEData(mysql::row_view const& row, dTypes::PassiveTerrainEffect& effect) {
		set(effect.name, row[2]);
		set(effect.incomeMod, row[3]);
		set(effect.incomeFlatMod, row[4]);
		set(effect.repairMod, row[5]);
		set(effect.occludesVisionMod, row[6]);
		set(effect.visionModBoost, row[7]);
		set(effect.buildCostMod, row[8]);
	}

	void assignPTEBuildListData(mysql::row_view const& row, dTypes::PassiveTerrainEffect& effect) {
		effect.buildListMod->push_back(*get<std::string>(row[3]));
	}

	void assignATEData(mysql::row_view const& row, dTypes::ActiveTerrainEffect& effect) {
		set(effect.name, row[2]);
		set(effect.unitSummonedName, row[3]);
		set(effect.unitSummonedInitialDamage, row[4]);
		set(effect.unitSummonedActive, row[5]);
	}

	void assignPGEData(mysql::row_view const& row, dTypes::PassiveGlobalEffect& effect) {
		set(effect.name, row[2]);
		set(effect.variantMod, row[3]);
		set(effect.movementClassVariantReplace, row[4]);
		set(effect.movementClassVariantOverride, row[5]);
		set(effect.minimumVisionMod, row[6]);
	}

	void assignPGEVariantHintData(mysql::row_view const& row, dTypes::PassiveGlobalEffect& effect) {
		effect.variantHintMod->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	void assignAGEData(mysql::row_view const& row, dTypes::ActiveGlobalEffect& effect) {
		set(effect.name, row[2]);
		set(effect.fundMod, row[3]);
		set(effect.fundFlatMod, row[4]);
		set(effect.powerBarMod, row[5]);
		set(effect.powerBarPerFunds, row[6]);
		set(effect.missileCount, row[7]);
		set(effect.missileDamage, row[8]);
		set(effect.missileAreaOfEffect, row[9]);
		set(effect.missileStunDuration, row[10]);
		set(effect.coChargeFactor, row[11]);
	}

	void assignAGEMissileTargetData(mysql::row_view const& row, dTypes::ActiveGlobalEffect& effect) {
		effect.missileTargetMethod->push_back(*get<std::string>(row[3]));
	}

	void assignCommanderData(mysql::row_view const& row, dTypes::CommanderType& commander) {
		set(commander.name, row[2]);
		set(commander.copCost, row[3]);
		set(commander.scopCost, row[4]);
		set(commander.coMeterMultiplier, row[5]);
		set(commander.playable, row[6]);
	}

	void assignCommanderEffectData(mysql::row_view const& row, dTypes::CommanderType& commander) {
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
			throw std::runtime_error("There was an invalid entry in the COMMANDER_EFFECT_REFERENCE table.");
		};

		auto& vec = selector();
		if (!vec) {
			vec.emplace();
		}
		vec->push_back(*get<std::string>(row[3]));
	}

	void assignSettingsData(mysql::row_view const& row, dTypes::Settings& setting) {
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

	void assignSettingsVariantData(mysql::row_view const& row, dTypes::Settings& setting) {
		setting.variant->emplace(*get<std::string>(row[3]), *get<int64_t>(row[4]));
	}

	std::string error_guard(net::GETFunc func, net::HTTPHeaders const& headers) {
		try {
			return func(headers);
		}
		catch (mysql::error_with_diagnostics const& e) {
			auto const& diagnostics = e.get_diagnostics();
			throw net::RestError("Internal SQL Error: '" + std::string(e.get_diagnostics().client_message()) + "'/'" + std::string(e.get_diagnostics().server_message()) + "'", net::RestError::Type::INTERNAL_ERROR);
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

	std::string get_weapons_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> weaponName;
		if (auto ret = headers.httpHeaders.find("weaponName"); ret != headers.httpHeaders.end()) {
			weaponName = ret->second;
		}
		auto results = getLookup(
			session.connection,
			"WEAPON_TYPE",
			*modId,
			weaponName
		);
		std::vector<dTypes::WeaponType> weapons;
		for (auto const& row : results.rows()) {
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
				for (auto const& baseDamageRow : baseDamageResults.rows()) {
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
				for (auto const& stealthTargetsRow : stealthTargetsResults.rows()) {
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

	std::string get_units_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> unitName;
		if (auto ret = headers.httpHeaders.find("unitName"); ret != headers.httpHeaders.end()) {
			unitName = ret->second;
		}
		auto results = getLookup(session.connection, "UNIT_TYPE", *modId, unitName);
		std::vector<dTypes::UnitType> units;
		for (auto const& row : results.rows()) {
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
				for (auto const& weaponRow : weaponResults.rows()) {
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
			for (auto const& classificationRow : classificationResults.rows()) {
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
				for (auto const& transportListRow : transportListResults.rows()) {
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

	std::string get_terrains_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> terrainName;
		if (auto ret = headers.httpHeaders.find("terrainName"); ret != headers.httpHeaders.end()) {
			terrainName = ret->second;
		}
		std::vector<dTypes::TerrainType> terrains;
		auto results = getLookup(
			session.connection,
			"TERRAIN_TYPE",
			*modId,
			terrainName
		);
		for (auto const& row : results.rows()) {
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
			for (auto const& buildRepairRow : buildRepairResults.rows()) {
				assignTerrainBuildRepairData(buildRepairRow, newTerrain);
			}

			auto activateListResults = getReferenceLookup(
				session.connection,
				"TERRAIN_ACTIVATIONLIST_REFERENCE",
				"TERRAINTYPE_NAME",
				*modId,
				newTerrain.name
			);
			for (auto const& activateListRow : activateListResults.rows()) {
				assignTerrainActivationListData(activateListRow, newTerrain);
			}

			auto activateEffectResults = getReferenceLookup(
				session.connection,
				"TERRAIN_ACTIVATIONEFFECT_REFERENCE",
				"TERRAINTYPE_NAME",
				*modId,
				newTerrain.name
			);
			for (auto const& activateEffectRow : activateEffectResults.rows()) {
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

	std::string get_commanders_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> commanderName;
		if (auto ret = headers.httpHeaders.find("commanderName"); ret != headers.httpHeaders.end()) {
			commanderName = ret->second;
		}
		std::vector<dTypes::CommanderType> commanders;
		auto results = getLookup(
			session.connection,
			"COMMANDER_TYPE",
			*modId,
			commanderName
		);
		for (auto const& row : results.rows()) {
			auto& newCommander = commanders.emplace_back();
			assignCommanderData(row, newCommander);
			auto commanderEffectResults = getReferenceLookup(
				session.connection,
				"COMMANDER_EFFECT_REFERENCE",
				"COMMANDER_NAME",
				*modId,
				newCommander.name
			);
			for (auto const& commanderEffectRow : commanderEffectResults.rows()) {
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

	std::string get_movements_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> movementName;
		if (auto ret = headers.httpHeaders.find("movementName"); ret != headers.httpHeaders.end()) {
			movementName = ret->second;
		}
		std::vector<dTypes::MovementClass> movements;
		auto results = getLookup(
			session.connection,
			"MOVEMENT_TYPE",
			*modId,
			movementName
		);
		for (auto const& row : results.rows()) {
			auto& newMovement = movements.emplace_back();
			assignMovementData(row, newMovement);
			auto movementCostResults = getReferenceLookup(
				session.connection,
				"MOVEMENT_COST_REFERENCE",
				"MOVEMENT_NAME",
				*modId,
				newMovement.name
			);
			for (auto const& movementCostRow : movementCostResults.rows()) {
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

	std::string get_players_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> playerName;
		if (auto ret = headers.httpHeaders.find("playerName"); ret != headers.httpHeaders.end()) {
			playerName = ret->second;
		}
		std::vector<dTypes::PlayerType> players;
		auto results = getLookup(
			session.connection,
			"PLAYER_TYPE",
			*modId,
			playerName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& permittedPlayerSlotRow : permittedPlayerSlotsResults.rows()) {
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
				for (auto const& permittedCommanderRow : permittedCommanderResults.rows()) {
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

	std::string get_pues_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::PassiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_UNIT_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& sRow : firepowerTerrainResults.rows()) {
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
				for (auto const& sRow : defenseTerrainResults.rows()) {
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
				for (auto const& sRow : visionVariantResults.rows()) {
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
				for (auto const& sRow : firepowerVariantResults.rows()) {
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
				for (auto const& sRow : defenseVariantResults.rows()) {
					assignPUEDefenseVariantData(sRow, newEffect);
				}
			}

			auto intelResults = getReferenceLookup(
				session.connection,
				"PUE_INTEL_REFERENCE",
				"EFFECT_NAME",
				*modId,
				newEffect.name
			);
			if (intelResults.size() > 0) {
				for (auto const& row : intelResults.rows()) {
					assignPUEIntelData(row, newEffect);
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

	std::string get_aues_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::ActiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_UNIT_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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

	std::string get_ptes_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::PassiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_TERRAIN_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& sRow : buildListResults.rows()) {
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

	std::string get_ates_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::ActiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_TERRAIN_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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

	std::string get_pges_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::PassiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_GLOBAL_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& sRow : variantHintResults.rows()) {
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

	std::string get_ages_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> effectName;
		if (auto ret = headers.httpHeaders.find("effectName"); ret != headers.httpHeaders.end()) {
			effectName = ret->second;
		}
		std::vector<dTypes::ActiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_GLOBAL_EFFECT",
			*modId,
			effectName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& sRow : missileTargetResults.rows()) {
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

	std::string get_settings_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> settingsName;
		if (auto ret = headers.httpHeaders.find("settingsName"); ret != headers.httpHeaders.end()) {
			settingsName = ret->second;
		}
		std::vector<dTypes::Settings> settings;
		auto results = getLookup(
			session.connection,
			"DEFAULT_GAME_SETTINGS",
			*modId,
			settingsName
		);
		for (auto const& row : results.rows()) {
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
				for (auto const& sRow : settingsVariantResults.rows()) {
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

	std::string get_mod_metadata_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
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
		session.connection.execute(statement.bind(*modId), results);
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

	/*std::string get_text_resources_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> key;
		if (auto ret = headers.httpHeaders.find("key"); ret != headers.httpHeaders.end()) {
			key = ret->second;
		}
		std::optional<std::string_view> type;
		if (auto ret = headers.httpHeaders.find("type"); ret != headers.httpHeaders.end()) {
			type = ret->second;
		}
		std::optional<std::string_view> language;
		if (auto ret = headers.httpHeaders.find("language"); ret != headers.httpHeaders.end()) {
			language = ret->second;
		}
		std::map<std::tuple<std::string, std::string, std::optional<std::string>>, dTypes::TextResource> resources;
		
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
			dTypes::TextResource* ptr;
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

	std::string get_image_resources_impl(net::HTTPHeaders const& headers) {
		sqlutil::Session session;
		auto modId = getModId(session.connection, headers);
		if (!modId) {
			throw net::RestError("Unable to find Mod.", net::RestError::Type::INVALID_DATA);
		}
		std::optional<std::string_view> key;
		if (auto ret = headers.httpHeaders.find("key"); ret != headers.httpHeaders.end()) {
			key = ret->second;
		}
		std::optional<std::string_view> type;
		if (auto ret = headers.httpHeaders.find("type"); ret != headers.httpHeaders.end()) {
			type = ret->second;
		}
		std::optional<std::string_view> army;
		if (auto ret = headers.httpHeaders.find("army"); ret != headers.httpHeaders.end()) {
			army = ret->second;
		}
		std::map<std::tuple<std::string, std::string, std::optional<std::string>, std::optional<int64_t>>, dTypes::ImageResource> resources;

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
			std::optional<int64_t> orientation;

			set(key, row[2]);
			set(type, row[3]);
			set(armyColor, row[4]);
			set(orientation, row[7]);
			auto it = resources.find(std::make_tuple(key, type, armyColor, orientation));
			dTypes::ImageResource* ptr;
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
			set(smallImagePart, row[5]);
			set(largeImagePart, row[6]);
			if (smallImagePart)
				newResource.smallImage += *smallImagePart;
			if (largeImagePart)
				newResource.largeImage += *largeImagePart;
			set(newResource.variant, row[8]);
		}
		boost::json::array ret;
		for (auto const& [key, resource] : resources) {
			boost::json::object obj;
			resource.writeTo(obj);
			ret.push_back(std::move(obj));
		}
		return boost::json::serialize(ret);
	}*/
}

std::string rest::data::get_weapons(net::HTTPHeaders const& headers) {
	return error_guard(get_weapons_impl, headers);
}

std::string rest::data::get_units(net::HTTPHeaders const& headers) {
	return error_guard(get_units_impl, headers);
}

std::string rest::data::get_terrains(net::HTTPHeaders const& headers) {
	return error_guard(get_terrains_impl, headers);
}

std::string rest::data::get_commanders(net::HTTPHeaders const& headers) {
	return error_guard(get_commanders_impl, headers);
}

std::string rest::data::get_movements(net::HTTPHeaders const& headers) {
	return error_guard(get_movements_impl, headers);
}

std::string rest::data::get_players(net::HTTPHeaders const& headers) {
	return error_guard(get_players_impl, headers);
}

std::string rest::data::get_pues(net::HTTPHeaders const& headers) {
	return error_guard(get_pues_impl, headers);
}

std::string rest::data::get_aues(net::HTTPHeaders const& headers) {
	return error_guard(get_aues_impl, headers);
}

std::string rest::data::get_ptes(net::HTTPHeaders const& headers) {
	return error_guard(get_ptes_impl, headers);
}

std::string rest::data::get_ates(net::HTTPHeaders const& headers) {
	return error_guard(get_ates_impl, headers);
}

std::string rest::data::get_pges(net::HTTPHeaders const& headers) {
	return error_guard(get_pges_impl, headers);
}

std::string rest::data::get_ages(net::HTTPHeaders const& headers) {
	return error_guard(get_ages_impl, headers);
}

std::string rest::data::get_settings(net::HTTPHeaders const& headers) {
	return error_guard(get_settings_impl, headers);
}

std::string rest::data::get_mod_metadata(net::HTTPHeaders const& headers) {
	return error_guard(get_mod_metadata_impl, headers);
}

//std::string rest::data::get_text_resources(net::HTTPHeaders const& headers) {
//	return error_guard(get_text_resources_impl, headers);
//}
//std::string rest::data::get_image_resources(net::HTTPHeaders const& headers) {
//	return error_guard(get_image_resources_impl, headers);
//}
