//#include<RestFunctions.h>
//#include<boost/json.hpp>
//#include<SQLUtil.h>
//#include<iostream>
//
//namespace {
//	const std::string secret_key = "a8f7af7d81269941f6d78eab35632eb0c603bd3ece95f8df7ab4a41ad187b06a";
//
//	template<typename T>
//	std::optional<T> getOptional(boost::json::value const& obj, std::string_view name) {
//		static_assert("Unknown Type specified as template parameter");
//	}
//
//	template<>
//	std::optional<int64_t> getOptional<int64_t>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return ptr->as_int64();
//		return {};
//	}
//
//	template<>
//	std::optional<double> getOptional<double>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return ptr->as_double();
//		return {};
//	}
//
//	template<>
//	std::optional<bool> getOptional<bool>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return ptr->as_bool();
//		return {};
//	}
//
//	template<>
//	std::optional<char const*> getOptional<char const*>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return ptr->as_string().c_str();
//		return {};
//	}
//
//	auto getOptionalBoolAsInt(boost::json::value const& value, std::string_view name) {
//		auto boolOpt = getOptional<bool>(value, name);
//		if (boolOpt)
//			return std::make_optional<int64_t>(*boolOpt ? 1 : 0);
//		return std::optional<int64_t>{};
//	}
//
//	auto getOptionalString(boost::json::value const& value, std::string_view name) {
//		return getOptional<char const*>(value, name);
//	}
//
//	template<>
//	std::optional<std::reference_wrapper<boost::json::array const>> getOptional<std::reference_wrapper<boost::json::array const>>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return ptr->as_array();
//		return {};
//	}
//
//	auto getOptionalArray(boost::json::value const& value, std::string_view name) {
//		return getOptional<std::reference_wrapper<boost::json::array const>>(value, name);
//	}
//
//	template<>
//	std::optional<std::reference_wrapper<boost::json::value const>> getOptional<std::reference_wrapper<boost::json::value const>>(boost::json::value const& value, std::string_view name) {
//		auto const& obj = value.as_object();
//		if (auto ptr = obj.if_contains(name))
//			return *ptr;
//		return {};
//	}
//
//	auto getOptionalValue(boost::json::value const& value, std::string_view name) {
//		return getOptional<std::reference_wrapper<boost::json::value const>>(value, name);
//	}
//
//	uint64_t submitMetaData(boost::json::value const& metadata, mysqlx::Session& session) {
//		auto const& name = metadata.at("name").as_string().c_str();
//		auto const& version = metadata.at("version").as_double();
//		auto versionString = std::to_string(version);
//		session.sql(R"SQL(
//			update 
//				DATA.MOD
//			set 
//				EXPIRED = current_timestamp
//			where
//				NAME = ?
//				and EXPIRED is null
//		)SQL").bind(name).execute();
//
//		auto resultSet = session.sql(R"SQL(
//			insert into 
//				DATA.MOD
//				(NAME, VERSION)
//			values
//				(?, ?)
//		)SQL").bind(name, versionString.c_str()).execute();
//		return resultSet.getAutoIncrementValue();
//	}
//
//	template<typename T, typename Statement>
//	void bindOptional(std::optional<T> const& opt, Statement & statement) {
//		if (opt)
//			statement.bind(*opt);
//		else
//			statement.bind(nullptr);
//	}
//
//	void submitUnitData(boost::json::value const& unitData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& unit : unitData.as_array()) {
//			auto const& name = unit.at("name").as_string().c_str();
//			auto const& cost = unit.at("cost").as_int64();
//			auto const& maxFuel = unit.at("maxFuel").as_int64();
//			auto const& maxAmmo = unit.at("maxAmmo").as_int64();
//			auto const& visionRange = unit.at("visionRange").as_int64();
//			auto const& movementRange = unit.at("movementRange").as_int64();
//			auto const& movementClass = unit.at("movementClass").as_string().c_str();
//			auto const& classifications = unit.at("classifications").as_array();
//
//			auto fuelPerDay = getOptional<int64_t>(unit, "fuelPerDay");
//			auto fuelPerDayStealth = getOptional<int64_t>(unit, "fuelPerDayStealth");
//			auto weapons = getOptionalArray(unit, "weapons");
//			auto supplyRepair = getOptional<int64_t>(unit, "supplyRepair");
//			auto transportCapacity = getOptional<int64_t>(unit, "transportCapacity");
//			auto transportList = getOptionalArray(unit, "transportList");
//			auto hitPoints = getOptional<int64_t>(unit, "hitPoints");
//			auto captureSpeed = getOptional<int64_t>(unit, "captureSpeed");
//			auto ignoresVisionOcclusion = getOptionalBoolAsInt(unit, "ignoresVisionOcclusion");
//			auto stealthType = getOptionalString(unit, "stealthType");
//
//			auto unitInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.UNIT_TYPE
//					(
//						MOD_ID, NAME, COST, MAXFUEL, MAXAMMO, VISIONRANGE, MOVEMENTCLASS, MOVEMENTRANGE, FUELPERDAY, FUELPERDAYSTEALTH,
//						SUPPLYREPAIR, TRANSPORTCAPACITY, HITPOINTS, CAPTURESPEED, IGNORESVISIONOCCLUSION, STEALTHTYPE
//					)
//				values 
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//			unitInsertStatement.bind(mod_id, name, cost, maxFuel, maxAmmo, visionRange, movementClass, movementRange);
//			bindOptional(fuelPerDay, unitInsertStatement);
//			bindOptional(fuelPerDayStealth, unitInsertStatement);
//			bindOptional(supplyRepair, unitInsertStatement);
//			bindOptional(transportCapacity, unitInsertStatement);
//			bindOptional(hitPoints, unitInsertStatement);
//			bindOptional(captureSpeed, unitInsertStatement);
//			bindOptional(ignoresVisionOcclusion, unitInsertStatement);
//			bindOptional(stealthType, unitInsertStatement);
//
//			unitInsertStatement.execute();
//
//			if (weapons) {
//				auto const& weaponsArray = weapons->get();
//				int order = 0;
//				for (auto const& weapon : weaponsArray) {
//					auto unitWeaponInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.UNIT_WEAPON_REFERENCE
//							(
//								MOD_ID, UNITTYPE_NAME, WEAPONTYPE_NAME, WEAPONORDER
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					auto const& weaponName = weapon.as_string().c_str();
//					unitWeaponInsertStatement.bind(mod_id, name, weaponName, order);
//					unitWeaponInsertStatement.execute();
//					order++;
//				}
//			}
//
//			if (transportList) {
//				auto const& transportListArray = transportList->get();
//				for (auto const& transportableUnit : transportListArray) {
//					auto unitTransportListInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.UNIT_TRANSPORTLIST_REFERENCE
//							(
//								MOD_ID, UNITTYPE_NAME, TRANSPORTED_NAME
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//					auto const& transportedName = transportableUnit.as_string().c_str();
//					unitTransportListInsertStatement.bind(mod_id, name, transportedName);
//					unitTransportListInsertStatement.execute();
//				}
//			}
//
//			for (auto const& classification : classifications) {
//				auto unitClassificationInsertStatement = session.sql(R"SQL(
//					insert into
//						DATA.UNIT_CLASSIFICATION_REFERENCE
//						(
//							MOD_ID, UNITTYPE_NAME, CLASSIFICATION
//						)
//					values
//						(
//							?, ?, ?
//						)
//				)SQL");
//				auto const& classificationString = classification.as_string().c_str();
//				unitClassificationInsertStatement.bind(mod_id, name, classificationString);
//				unitClassificationInsertStatement.execute();
//			}
//		}
//	}
//
//	void submitWeaponData(boost::json::value const& weaponData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& weapon : weaponData.as_array()) {
//			auto const& name = weapon.at("name").as_string().c_str();
//			auto const& ammoConsumed = weapon.at("ammoConsumed").as_int64();
//			auto baseDamage = getOptionalValue(weapon, "baseDamage");
//			auto const& maxRange = weapon.at("maxRange").as_int64();
//			auto minRange = getOptional<int64_t>(weapon, "minRange");
//			auto selfTarget = getOptionalBoolAsInt(weapon, "selfTarget");
//			auto affectedByLuck = getOptionalBoolAsInt(weapon, "affectedByLuck");
//			auto nonLethal = getOptionalBoolAsInt(weapon, "nonLethal");
//			auto areaOfEffect = getOptional<int64_t>(weapon, "areaOfEffect");
//			auto targetsStealth = getOptionalArray(weapon, "targetsStealth");
//			auto flatDamage = getOptional<int64_t>(weapon, "flatDamage");
//
//			auto weaponInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.WEAPON_TYPE
//					(
//						MOD_ID, NAME, AMMOCONSUMED, MAXRANGE, MINRANGE, SELFTARGET, AFFECTEDBYLUCK, NONLETHAL, AREAOFEFFECT, FLATDAMAGE
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//
//			weaponInsertStatement.bind(mod_id, name, ammoConsumed, maxRange);
//			bindOptional(minRange, weaponInsertStatement);
//			bindOptional(selfTarget, weaponInsertStatement);
//			bindOptional(affectedByLuck, weaponInsertStatement);
//			bindOptional(nonLethal, weaponInsertStatement);
//			bindOptional(areaOfEffect, weaponInsertStatement);
//			bindOptional(flatDamage, weaponInsertStatement);
//			weaponInsertStatement.execute();
//
//			if (baseDamage) {
//				auto const& baseDamageObject = baseDamage->get().as_object();
//				for (auto const& entry : baseDamageObject) {
//					auto const& unitName = entry.key_c_str();
//					auto const& damage = entry.value().as_int64();
//					auto weaponBaseDamageInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.WEAPON_BASEDAMAGE_REFERENCE
//							(
//								MOD_ID, WEAPONTYPE_NAME, UNITTYPE_NAME, DAMAGE
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					weaponBaseDamageInsertStatement.bind(mod_id, name, unitName, damage);
//					weaponBaseDamageInsertStatement.execute();
//				}
//			}
//
//			if (targetsStealth) {
//				auto const& targetsStealthArray = targetsStealth->get();
//				for (auto const& targetsStealthValue : targetsStealthArray) {
//					auto const& valueString = targetsStealthValue.as_string().c_str();
//					auto weaponTargetsStealthInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.WEAPON_STEALTHTARGET_REFERENCE
//							(
//								MOD_ID, WEAPONTYPE_NAME, STEALTHTYPE
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//					weaponTargetsStealthInsertStatement.bind(mod_id, name, valueString);
//					weaponTargetsStealthInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitTerrainData(boost::json::value const& terrainData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& terrain : terrainData.as_array()) {
//			auto const& name = terrain.at("name").as_string().c_str();
//			auto const& stars = terrain.at("stars").as_int64();
//			auto maxCapturePoints = getOptional<int64_t>(terrain, "maxCapturePoints");
//			auto sameAs = getOptionalString(terrain, "sameAs");
//			auto buildList = getOptionalArray(terrain, "buildList");
//			auto income = getOptional<int64_t>(terrain, "income");
//			auto repair = getOptional<int64_t>(terrain, "repair");
//			auto repairList = getOptionalArray(terrain, "repairList");
//			auto occludesVision = getOptionalBoolAsInt(terrain, "occludesVision");
//			auto hitPoints = getOptional<int64_t>(terrain, "hitPoints");
//			auto destroyed = getOptionalString(terrain, "destroyed");
//			auto damagedLike = getOptionalString(terrain, "damagedLike");
//
//			auto terrainInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.TERRAIN_TYPE
//					(
//						MOD_ID, NAME, STARS, MAXCAPTUREPOINTS, SAMEAS, INCOME, REPAIR, OCCLUDESVISION, HITPOINTS, DESTROYED, DAMAGEDLIKE
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//
//			terrainInsertStatement.bind(mod_id, name, stars);
//			bindOptional(maxCapturePoints, terrainInsertStatement);
//			bindOptional(sameAs, terrainInsertStatement);
//			bindOptional(income, terrainInsertStatement);
//			bindOptional(repair, terrainInsertStatement);
//			bindOptional(occludesVision, terrainInsertStatement);
//			bindOptional(hitPoints, terrainInsertStatement);
//			bindOptional(destroyed, terrainInsertStatement);
//			bindOptional(damagedLike, terrainInsertStatement);
//			terrainInsertStatement.execute();
//
//			if (buildList) {
//				int order = 0;
//				for (auto const& buildUnit : buildList->get()) {
//					auto const& unitName = buildUnit.as_string().c_str();
//					auto terrainBuildInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.TERRAIN_BUILDREPAIR_REFERENCE
//							(
//								MOD_ID, TERRAINTYPE_NAME, UNITTYPE_NAME, UNITORDER, BUILDREPAIR
//							)
//						values
//							(
//								?, ?, ?, ?, 1
//							)
//					)SQL");
//					terrainBuildInsertStatement.bind(mod_id, name, unitName, order);
//					terrainBuildInsertStatement.execute();
//					order++;
//				}
//			}
//
//			if (repairList) {
//				for (auto const& repairUnit : repairList->get()) {
//					auto const& unitName = repairUnit.as_string().c_str();
//					auto terrainRepairInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.TERRAIN_BUILDREPAIR_REFERENCE
//							(
//								MOD_ID, TERRAINTYPE_NAME, UNITTYPE_NAME, BUILDREPAIR
//							)
//						values
//							(
//								?, ?, ?, 2
//							)
//					)SQL");
//					terrainRepairInsertStatement.bind(mod_id, name, unitName);
//					terrainRepairInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitMovementData(boost::json::value const& movementData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& movementClass : movementData.as_array()) {
//			auto const& name = movementClass.at("name").as_string().c_str();
//			auto const& movementCosts = movementClass.at("movementCosts");
//			auto variantMods = getOptionalValue(movementClass, "variantMods");
//
//			auto movementInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.MOVEMENT_TYPE
//					(
//						MOD_ID, NAME
//					)
//				values
//					(
//						?, ?
//					)
//			)SQL");
//			movementInsertStatement.bind(mod_id, name);
//			movementInsertStatement.execute();
//
//			for (auto const& entry : movementCosts.as_object()) {
//				auto const& terrainName = entry.key_c_str();
//				auto const& cost = entry.value().as_int64();
//
//				auto movementCostInsertStatement = session.sql(R"SQL(
//					insert into
//						DATA.MOVEMENT_COST_REFERENCE
//						(
//							MOD_ID, MOVEMENT_NAME, TERRAINTYPE_NAME, COST
//						)
//					values
//						(
//							?, ?, ?, ?
//						)
//				)SQL");
//				movementCostInsertStatement.bind(mod_id, name, terrainName, cost);
//				movementCostInsertStatement.execute();
//			}
//
//			if (variantMods) {
//				for (auto const& variantEntry : variantMods->get().as_object()) {
//					auto const& variantName = variantEntry.key_c_str();
//					for (auto const& costModEntry : variantEntry.value().as_object()) {
//						auto const& terrainName = costModEntry.key_c_str();
//						auto const& cost = costModEntry.value().as_int64();
//
//						auto movementCostInsertStatement = session.sql(R"SQL(
//							insert into
//								DATA.MOVEMENT_COST_REFERENCE
//								(
//									MOD_ID, MOVEMENT_NAME, TERRAINTYPE_NAME, COST, VARIANT
//								)
//							values
//								(
//									?, ?, ?, ?, ?
//								)
//						)SQL");
//						movementCostInsertStatement.bind(mod_id, name, terrainName, cost, variantName);
//						movementCostInsertStatement.execute();
//					}
//				}
//			}
//		}
//	}
//
//	void submitCommanderData(boost::json::value const& commanderData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& commander : commanderData.as_array()) {
//			auto const& name = commander.at("name").as_string().c_str();
//			auto passiveUnitEffectsD2d = getOptionalArray(commander, "passiveUnitEffectsD2d");
//			auto passiveTerrainEffectsD2d = getOptionalArray(commander, "passiveTerrainEffectsD2d");
//			auto passiveGlobalEffectsD2d = getOptionalArray(commander, "passiveTerrainEffectsD2d");
//
//			auto copCost = getOptional<int64_t>(commander, "copCost");
//			auto passiveUnitEffectsCop = getOptionalArray(commander, "passiveUnitEffectsCop");
//			auto activeUnitEffectsCop = getOptionalArray(commander, "activeUnitEffectsCop");
//			auto passiveTerrainEffectsCop = getOptionalArray(commander, "passiveTerrainEffectsCop");
//			auto activeTerrainEffectsCop = getOptionalArray(commander, "activeTerrainEffectsCop");
//			auto passiveGlobalEffectsCop = getOptionalArray(commander, "passiveGlobalEffectsCop");
//			auto activeGlobalEffectsCop = getOptionalArray(commander, "activeGlobalEffectsCop");
//
//			auto scopCost = getOptional<int64_t>(commander, "scopCost");
//			auto passiveUnitEffectsScop = getOptionalArray(commander, "passiveUnitEffectsScop");
//			auto activeUnitEffectsScop = getOptionalArray(commander, "activeUnitEffectsScop");
//			auto passiveTerrainEffectsScop = getOptionalArray(commander, "passiveTerrainEffectsScop");
//			auto activeTerrainEffectsScop = getOptionalArray(commander, "activeTerrainEffectsScop");
//			auto passiveGlobalEffectsScop = getOptionalArray(commander, "passiveGlobalEffectsScop");
//			auto activeGlobalEffectsScop = getOptionalArray(commander, "activeGlobalEffectsScop");
//
//			auto coMeterMultiplier = getOptional<int64_t>(commander, "coMeterMultiplier");
//
//			auto commanderInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.COMMANDER_TYPE
//					(
//						MOD_ID, NAME, COPCOST, SCOPCOST, COMETERMULTIPLIER
//					)
//				values
//					(
//						?, ?, ?, ?, ?
//					)
//			)SQL");
//			commanderInsertStatement.bind(mod_id, name);
//			bindOptional(copCost, commanderInsertStatement);
//			bindOptional(scopCost, commanderInsertStatement);
//			bindOptional(coMeterMultiplier, commanderInsertStatement);
//			commanderInsertStatement.execute();
//
//			auto insertEffects = [&session, &name, mod_id](boost::json::array const& effects, std::string_view effectType, int effectCategory) {
//				for (auto const& effect : effects) {
//					auto const& effectName = effect.as_string().c_str();
//					auto effectInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.COMMANDER_EFFECT_REFERENCE
//							(
//								MOD_ID, COMMANDER_NAME, EFFECT_NAME, EFFECT_TYPE, EFFECT_CATEGORY
//							)
//						values
//							(
//								?, ?, ?, ?, ?
//							)
//					)SQL");
//					effectInsertStatement.bind(mod_id, name, effectName, effectType.data(), effectCategory);
//					effectInsertStatement.execute();
//				}
//			};
//			if (passiveUnitEffectsD2d)
//				insertEffects(passiveUnitEffectsD2d->get(), "PUE", 0);
//			if (passiveTerrainEffectsD2d)
//				insertEffects(passiveTerrainEffectsD2d->get(), "PTE", 0);
//			if (passiveGlobalEffectsD2d)
//				insertEffects(passiveGlobalEffectsD2d->get(), "PGE", 0);
//
//			if (passiveUnitEffectsCop)
//				insertEffects(passiveUnitEffectsCop->get(), "PUE", 1);
//			if (activeUnitEffectsCop)
//				insertEffects(activeUnitEffectsCop->get(), "AUE", 1);
//			if (passiveTerrainEffectsCop)
//				insertEffects(passiveTerrainEffectsCop->get(), "PTE", 1);
//			if (activeTerrainEffectsCop)
//				insertEffects(activeTerrainEffectsCop->get(), "ATE", 1);
//			if (passiveGlobalEffectsCop)
//				insertEffects(passiveGlobalEffectsCop->get(), "PGE", 1);
//			if (activeGlobalEffectsCop)
//				insertEffects(activeGlobalEffectsCop->get(), "AGE", 1);
//
//			if (passiveUnitEffectsScop)
//				insertEffects(passiveUnitEffectsScop->get(), "PUE", 2);
//			if (activeUnitEffectsScop)
//				insertEffects(activeUnitEffectsScop->get(), "AUE", 2);
//			if (passiveTerrainEffectsScop)
//				insertEffects(passiveTerrainEffectsScop->get(), "PTE", 2);
//			if (activeTerrainEffectsScop)
//				insertEffects(activeTerrainEffectsScop->get(), "ATE", 2);
//			if (passiveGlobalEffectsScop)
//				insertEffects(passiveGlobalEffectsScop->get(), "PGE", 2);
//			if (activeGlobalEffectsScop)
//				insertEffects(activeGlobalEffectsScop->get(), "AGE", 2);
//		}
//	}
//
//	void submitPlayerData(boost::json::value const& playerData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& playerType : playerData.as_array()) {
//			auto const& name = playerType.at("name").as_string().c_str();
//			auto commanderTypeMod = getOptionalString(playerType, "commanderTypeMod");
//			auto permittedPlayerSlots = getOptionalArray(playerType, "permittedPlayerSlots");
//			auto permittedCommanderTypes = getOptionalArray(playerType, "permittedCommanderTypes");
//			auto teamName = getOptionalString(playerType, "teamName");
//
//			auto playerInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.PLAYER_TYPE
//					(
//						MOD_ID, NAME, COMMANDERTYPEMOD, TEAMNAME
//					)
//				values
//					(
//						?, ?, ?, ?
//					)
//			)SQL");
//			playerInsertStatement.bind(mod_id, name);
//			bindOptional(commanderTypeMod, playerInsertStatement);
//			bindOptional(teamName, playerInsertStatement);
//			playerInsertStatement.execute();
//
//			if (permittedPlayerSlots) {
//				for (auto const& playerSlot : permittedPlayerSlots->get()) {
//					auto const& playerSlotName = playerSlot.as_string().c_str();
//
//					auto playerSlotInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PLAYER_PERMITTEDPLAYERSLOT_REFERENCE
//							(
//								MOD_ID, PLAYERTYPE_NAME, PERMITTEDPLAYERSLOT
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//					playerSlotInsertStatement.bind(mod_id, name, playerSlotName);
//					playerSlotInsertStatement.execute();
//				}
//			}
//
//			if (permittedCommanderTypes) {
//				for (auto const& commanderType : permittedCommanderTypes->get()) {
//					auto const& commanderTypeName = commanderType.as_string().c_str();
//
//					auto commanderTypeInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PLAYER_PERMITTEDCOMMANDERTYPE_REFERENCE
//							(
//								MOD_ID, PLAYERTYPE_NAME, PERMITTEDCOMMANDERTYPE
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//					commanderTypeInsertStatement.bind(mod_id, name, commanderTypeName);
//					commanderTypeInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void insertEffectTargets(mysqlx::Session& session, uint64_t mod_id, char const* effectName, char const* effectType, boost::json::array const& targetArray) {
//		for (auto const& target : targetArray) {
//			auto const& targetName = target.as_string().c_str();
//
//			auto effectTargetInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.EFFECT_TARGET_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, EFFECT_TYPE, TARGET
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//			effectTargetInsertStatement.bind(mod_id, effectName, effectType, targetName);
//			effectTargetInsertStatement.execute();
//		}
//	}
//	void insertEffectAffects(mysqlx::Session& session, uint64_t mod_id, char const* effectName, char const* effectType, boost::json::array const& affectArray) {
//		for (auto const& affect : affectArray) {
//			auto const& affectName = affect.as_string().c_str();
//
//			auto effectTargetInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.EFFECT_AFFECT_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, EFFECT_TYPE, AFFECTS
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//			effectTargetInsertStatement.bind(mod_id, effectName, effectType, affectName);
//			effectTargetInsertStatement.execute();
//		}
//	}
//
//	void insertEffectUnitTypeRequired(mysqlx::Session& session, uint64_t mod_id, char const* effectName, char const* effectType, boost::json::array const& unitTypeArray) {
//		for (auto const& unitType : unitTypeArray) {
//			auto const& unitTypeName = unitType.as_string().c_str();
//
//			auto effectUnitTypeInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.EFFECT_UNITTYPEREQUIRED_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, EFFECT_TYPE, UNITTYPE_NAME
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//			effectUnitTypeInsertStatement.bind(mod_id, effectName, effectType, unitTypeName);
//			effectUnitTypeInsertStatement.execute();
//		}
//	}
//
//	void insertEffectClassification(mysqlx::Session& session, uint64_t mod_id, char const* effectName, char const* effectType, boost::json::array const& classificationArray) {
//		for (auto const& classification : classificationArray) {
//			auto const& classificationName = classification.as_string().c_str();
//
//			auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.EFFECT_CLASSIFICATION_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, EFFECT_TYPE, CLASSIFICATION
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//			insertStatement.bind(mod_id, effectName, effectType, classificationName);
//			insertStatement.execute();
//		}
//	}
//
//	void insertEffectTerrainRequired(mysqlx::Session& session, uint64_t mod_id, char const* effectName, char const* effectType, boost::json::array const& terrainArray) {
//		for (auto const& terrain : terrainArray) {
//			auto const& terrainName = terrain.as_string().c_str();
//
//			auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.EFFECT_TERRAIN_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, EFFECT_TYPE, TERRAINTYPE_NAME
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//			insertStatement.bind(mod_id, effectName, effectType, terrainName);
//			insertStatement.execute();
//		}
//	}
//
//	void submitPassiveUnitEffectData(boost::json::value const& passiveUnitEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& passiveUnitEffect : passiveUnitEffectData.as_array()) {
//			auto const& name = passiveUnitEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(passiveUnitEffect, "targets");
//			auto unitTypeRequired = getOptionalArray(passiveUnitEffect, "unitTypeRequired");
//			auto classificationRequired = getOptionalArray(passiveUnitEffect, "classificationRequired");
//			auto terrainRequired = getOptionalArray(passiveUnitEffect, "terrainRequired");
//			auto firepowerMod = getOptional<int64_t>(passiveUnitEffect, "firepowerMod");
//			auto defenseMod = getOptional<int64_t>(passiveUnitEffect, "defenseMod");
//			auto indirectDefenseMod = getOptional<int64_t>(passiveUnitEffect, "indirectDefenseMod");
//			auto minRangeMod = getOptional<int64_t>(passiveUnitEffect, "minRangeMod");
//			auto maxRangeMod = getOptional<int64_t>(passiveUnitEffect, "maxRangeMod");
//			auto fuelUseMod = getOptional<int64_t>(passiveUnitEffect, "fuelUseMod");
//			auto ammoUseMod = getOptional<int64_t>(passiveUnitEffect, "ammoUseMod");
//			auto goodLuckMod = getOptional<int64_t>(passiveUnitEffect, "goodLuckMod");
//			auto badLuckMod = getOptional<int64_t>(passiveUnitEffect, "badLuckMod");
//			auto movementMod = getOptional<int64_t>(passiveUnitEffect, "movementMod");
//			auto visionMod = getOptional<int64_t>(passiveUnitEffect, "visionMod");
//			auto terrainStarsMod = getOptional<int64_t>(passiveUnitEffect, "terrainStarsMod");
//			auto terrainStarsFlatMod = getOptional<int64_t>(passiveUnitEffect, "terrainStarsFlatMod");
//			auto terrainStarsDefense = getOptional<int64_t>(passiveUnitEffect, "terrainStarsDefense");
//			auto terrainStarsFirepower = getOptional<int64_t>(passiveUnitEffect, "terrainStarsFirepower");
//			auto counterfireMod = getOptional<int64_t>(passiveUnitEffect, "counterfireMod");
//			auto counterFirst = getOptionalBoolAsInt(passiveUnitEffect, "counterFirst");
//			auto captureRateMod = getOptional<int64_t>(passiveUnitEffect, "captureRateMod");
//			auto unitCostMod = getOptional<int64_t>(passiveUnitEffect, "unitCostMod");
//			auto hiddenHitPoints = getOptionalBoolAsInt(passiveUnitEffect, "hiddenHitPoints");
//			auto firepowerFromFunds = getOptional<int64_t>(passiveUnitEffect, "firepowerFromFunds");
//			auto defenseFromFunds = getOptional<int64_t>(passiveUnitEffect, "defenseFromFunds");
//			auto fundsFromDamage = getOptional<int64_t>(passiveUnitEffect, "fundsFromDamage");
//			auto firepowerFromOwnedTerrain = getOptionalValue(passiveUnitEffect, "firepowerFromOwnedTerrain");
//			auto defenseFromOwnedTerrain = getOptionalValue(passiveUnitEffect, "defenseFromOwnedTerrain");
//			auto visionVariantMods = getOptionalValue(passiveUnitEffect, "visionVariantMods");
//			auto firepowerVariantMods = getOptionalValue(passiveUnitEffect, "firepowerVariantMods");
//			auto defenseVariantMods = getOptionalValue(passiveUnitEffect, "defenseVariantMods");
//			auto coMeterChargeFromDealtDamage = getOptional<int64_t>(passiveUnitEffect, "coMeterChargeFromDealtDamage");
//			auto coMeterChargeFromReceivedDamage = getOptional<int64_t>(passiveUnitEffect, "coMeterChargeFromReceivedDamage");
//
//			auto passiveUnitEffectInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.PASSIVE_UNIT_EFFECT
//					(
//						MOD_ID, NAME, FIREPOWERMOD, DEFENSEMOD, INDIRECTDEFENSEMOD, MINRANGEMOD, MAXRANGEMOD, FUELUSEMOD, AMMOUSEMOD, GOODLUCKMOD, BADLUCKMOD,
//						MOVEMENTMOD, VISIONMOD, TERRAINSTARSMOD, TERRAINSTARSFLATMOD, TERRAINSTARSDEFENSE, TERRAINSTARSFIREPOWER, COUNTERFIREMOD, COUNTERFIRST,
//						CAPTURERATEMOD, UNITCOSTMOD, HIDDENHITPOINTS, FIREPOWERFROMFUNDS, DEFENSEFROMFUNDS, FUNDSFROMDAMAGE, COMETERCHARGEFROMDEALTDAMAGE,
//						COMETERCHARGEFROMRECEIVEDDAMAGE
//					)
//				VALUES
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 
//						?, ?, ?, ?, ?, ?, ?, ?,
//						?, ?, ?, ?, ?, ?, ?,
//						?
//					)
//			)SQL");
//
//			passiveUnitEffectInsertStatement.bind(mod_id, name);
//			bindOptional(firepowerMod, passiveUnitEffectInsertStatement);
//			bindOptional(defenseMod, passiveUnitEffectInsertStatement);
//			bindOptional(indirectDefenseMod, passiveUnitEffectInsertStatement);
//			bindOptional(minRangeMod, passiveUnitEffectInsertStatement);
//			bindOptional(maxRangeMod, passiveUnitEffectInsertStatement);
//			bindOptional(fuelUseMod, passiveUnitEffectInsertStatement);
//			bindOptional(ammoUseMod, passiveUnitEffectInsertStatement);
//			bindOptional(goodLuckMod, passiveUnitEffectInsertStatement);
//			bindOptional(badLuckMod, passiveUnitEffectInsertStatement);
//
//			bindOptional(movementMod, passiveUnitEffectInsertStatement);
//			bindOptional(visionMod, passiveUnitEffectInsertStatement);
//			bindOptional(terrainStarsMod, passiveUnitEffectInsertStatement);
//			bindOptional(terrainStarsFlatMod, passiveUnitEffectInsertStatement);
//			bindOptional(terrainStarsDefense, passiveUnitEffectInsertStatement);
//			bindOptional(terrainStarsFirepower, passiveUnitEffectInsertStatement);
//			bindOptional(counterfireMod, passiveUnitEffectInsertStatement);
//			bindOptional(counterFirst, passiveUnitEffectInsertStatement);
//
//			bindOptional(captureRateMod, passiveUnitEffectInsertStatement);
//			bindOptional(unitCostMod, passiveUnitEffectInsertStatement);
//			bindOptional(hiddenHitPoints, passiveUnitEffectInsertStatement);
//			bindOptional(firepowerFromFunds, passiveUnitEffectInsertStatement);
//			bindOptional(defenseFromFunds, passiveUnitEffectInsertStatement);
//			bindOptional(fundsFromDamage, passiveUnitEffectInsertStatement);
//			bindOptional(coMeterChargeFromDealtDamage, passiveUnitEffectInsertStatement);
//
//			bindOptional(coMeterChargeFromReceivedDamage, passiveUnitEffectInsertStatement);
//
//			passiveUnitEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "PUE", targets->get());
//			}
//			if (unitTypeRequired) {
//				insertEffectUnitTypeRequired(session, mod_id, name, "PUE", unitTypeRequired->get());
//			}
//			if (classificationRequired) {
//				insertEffectClassification(session, mod_id, name, "PUE", classificationRequired->get());
//			}
//			if (terrainRequired) {
//				insertEffectTerrainRequired(session, mod_id, name, "PUE", terrainRequired->get());
//			}
//
//			if (firepowerFromOwnedTerrain) {
//				for (auto const& entry : firepowerFromOwnedTerrain->get().as_object()) {
//					auto const& terrainName = entry.key_c_str();
//					auto const& firepowerMod = entry.value().as_int64();
//
//					auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PUE_FIREPOWERFROMTERRAIN_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, TERRAINTYPE_NAME, FIREPOWERMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					insertStatement.bind(mod_id, name, terrainName, firepowerMod);
//					insertStatement.execute();
//				}
//			}
//			if (defenseFromOwnedTerrain) {
//				for (auto const& entry : defenseFromOwnedTerrain->get().as_object()) {
//					auto const& terrainName = entry.key_c_str();
//					auto const& defenseMod = entry.value().as_int64();
//
//					auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PUE_DEFENSEFROMTERRAIN_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, TERRAINTYPE_NAME, DEFENSEMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					insertStatement.bind(mod_id, name, terrainName, defenseMod);
//					insertStatement.execute();
//				}
//			}
//			if (visionVariantMods) {
//				for (auto const& entry : visionVariantMods->get().as_object()) {
//					auto const& variantName = entry.key_c_str();
//					auto const& visionMod = entry.value().as_int64();
//
//					auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PUE_VISIONVARIANT_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, VARIANT, VISIONMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					insertStatement.bind(mod_id, name, variantName, visionMod);
//					insertStatement.execute();
//				}
//			}
//			if (firepowerVariantMods) {
//				for (auto const& entry : firepowerVariantMods->get().as_object()) {
//					auto const& variantName = entry.key_c_str();
//					auto const& firepowerMod = entry.value().as_int64();
//
//					auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PUE_FIREPOWERVARIANT_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, VARIANT, FIREPOWERMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					insertStatement.bind(mod_id, name, variantName, firepowerMod);
//					insertStatement.execute();
//				}
//			}
//			if (defenseVariantMods) {
//				for (auto const& entry : defenseVariantMods->get().as_object()) {
//					auto const& variantName = entry.key_c_str();
//					auto const& defenseMod = entry.value().as_int64();
//
//					auto insertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PUE_DEFENSEVARIANT_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, VARIANT, DEFENSEMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					insertStatement.bind(mod_id, name, variantName, defenseMod);
//					insertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitActiveUnitEffectData(boost::json::value const& activeUnitEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& activeUnitEffect : activeUnitEffectData.as_array()) {
//			auto const& name = activeUnitEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(activeUnitEffect, "targets");
//			auto unitTypeRequired = getOptionalArray(activeUnitEffect, "unitTypeRequired");
//			auto classificationRequired = getOptionalArray(activeUnitEffect, "classificationRequired");
//			auto terrainRequired = getOptionalArray(activeUnitEffect, "terrainRequired");
//			auto hitPointMod = getOptional<int64_t>(activeUnitEffect, "hitPointMod");
//			auto roundHitPoints = getOptional<int64_t>(activeUnitEffect, "roundHitPoints");
//			auto resupply = getOptionalBoolAsInt(activeUnitEffect, "resupply");
//			auto halveFuel = getOptionalBoolAsInt(activeUnitEffect, "halveFuel");
//			auto makeActive = getOptionalBoolAsInt(activeUnitEffect, "makeActive");
//			auto stunDuration = getOptional<int64_t>(activeUnitEffect, "stunDuration");
//
//			auto activeUnitEffectInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.ACTIVE_UNIT_EFFECT
//					(
//						MOD_ID, NAME, HITPOINTMOD, ROUNDHITPOINTS, RESUPPLY, HALVEFUEL, MAKEACTIVE, STUNDURATION
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//
//			activeUnitEffectInsertStatement.bind(mod_id, name);
//			bindOptional(hitPointMod, activeUnitEffectInsertStatement);
//			bindOptional(roundHitPoints, activeUnitEffectInsertStatement);
//			bindOptional(resupply, activeUnitEffectInsertStatement);
//			bindOptional(halveFuel, activeUnitEffectInsertStatement);
//			bindOptional(makeActive, activeUnitEffectInsertStatement);
//			bindOptional(stunDuration, activeUnitEffectInsertStatement);
//
//			activeUnitEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "AUE", targets->get());
//			}
//			if (unitTypeRequired) {
//				insertEffectUnitTypeRequired(session, mod_id, name, "AUE", unitTypeRequired->get());
//			}
//			if (classificationRequired) {
//				insertEffectClassification(session, mod_id, name, "AUE", classificationRequired->get());
//			}
//			if (terrainRequired) {
//				insertEffectTerrainRequired(session, mod_id, name, "AUE", terrainRequired->get());
//			}
//		}
//	}
//
//	void submitPassiveTerrainEffectData(boost::json::value const& passiveTerrainEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& passiveTerrainEffect : passiveTerrainEffectData.as_array()) {
//			auto const& name = passiveTerrainEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(passiveTerrainEffect, "targets");
//			auto affects = getOptionalArray(passiveTerrainEffect, "affects");
//			auto terrainRequired = getOptionalArray(passiveTerrainEffect, "terrainRequired");
//			auto classificationRequired = getOptionalArray(passiveTerrainEffect, "classificationRequired");
//			auto incomeMod = getOptional<int64_t>(passiveTerrainEffect, "incomeMod");
//			auto incomeFlatMod = getOptional<int64_t>(passiveTerrainEffect, "incomeFlatMod");
//			auto buildListMod = getOptionalArray(passiveTerrainEffect, "buildListMod");
//			auto repairMod = getOptional<int64_t>(passiveTerrainEffect, "repairMod");
//			auto occludesVisionMod = getOptionalBoolAsInt(passiveTerrainEffect, "occludesVisionMod");
//			auto visionModBoost = getOptional<int64_t>(passiveTerrainEffect, "visionModBoost");
//			auto buildCostMod = getOptional<int64_t>(passiveTerrainEffect, "buildCostMod");
//
//			auto passiveTerrainEffectInsertStatement = session.sql(R"SQL(
//				insert into 
//					DATA.PASSIVE_TERRAIN_EFFECT
//					(
//						MOD_ID, NAME, INCOMEMOD, INCOMEFLATMOD, REPAIRMOD, OCCLUDESVISIONMOD, VISIONMODBOOST, BUILDCOSTMOD
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//
//			passiveTerrainEffectInsertStatement.bind(mod_id, name);
//			bindOptional(incomeMod, passiveTerrainEffectInsertStatement);
//			bindOptional(incomeFlatMod, passiveTerrainEffectInsertStatement);
//			bindOptional(repairMod, passiveTerrainEffectInsertStatement);
//			bindOptional(occludesVisionMod, passiveTerrainEffectInsertStatement);
//			bindOptional(visionModBoost, passiveTerrainEffectInsertStatement);
//			bindOptional(buildCostMod, passiveTerrainEffectInsertStatement);
//
//			passiveTerrainEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "PTE", targets->get());
//			}
//			if (affects) {
//				insertEffectAffects(session, mod_id, name, "PTE", affects->get());
//			}
//			if (classificationRequired) {
//				insertEffectClassification(session, mod_id, name, "PTE", classificationRequired->get());
//			}
//			if (terrainRequired) {
//				insertEffectTerrainRequired(session, mod_id, name, "PTE", terrainRequired->get());
//			}
//
//			if (buildListMod) {
//				for (auto const& buildListItem : buildListMod->get()) {
//					auto const& buildListName = buildListItem.as_string().c_str();
//					auto pteBuildListInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PTE_BUILDLISTMOD_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, UNITTYPE_NAME
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//
//					pteBuildListInsertStatement.bind(mod_id, name, buildListName);
//					pteBuildListInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitActiveTerrainEffectData(boost::json::value const& activeTerrainEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for(auto const& activeTerrainEffect : activeTerrainEffectData.as_array()) {
//			auto const& name = activeTerrainEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(activeTerrainEffect, "targets");
//			auto affects = getOptionalArray(activeTerrainEffect, "affects");
//			auto terrainRequired = getOptionalArray(activeTerrainEffect, "terrainRequired");
//			auto unitSummonedName = getOptionalString(activeTerrainEffect, "unitSummonedName");
//			auto unitSummonedInitialDamage = getOptional<int64_t>(activeTerrainEffect, "unitSummonedInitialDamage");
//			auto unitSummonedActive = getOptionalBoolAsInt(activeTerrainEffect, "unitSummonedActive");
//
//			auto activeTerrainEffectInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.ACTIVE_TERRAIN_EFFECT
//					(
//						MOD_ID, NAME, UNITSUMMONEDNAME, UNITSUMMONEDINITIALDAMAGE, UNITSUMMONEDACTIVE
//					)
//				values
//					(
//						?, ?, ?, ?, ?
//					)
//			)SQL");
//
//			activeTerrainEffectInsertStatement.bind(mod_id, name);
//			bindOptional(unitSummonedName, activeTerrainEffectInsertStatement);
//			bindOptional(unitSummonedInitialDamage, activeTerrainEffectInsertStatement);
//			bindOptional(unitSummonedActive, activeTerrainEffectInsertStatement);
//
//			activeTerrainEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "ATE", targets->get());
//			}
//			if (affects) {
//				insertEffectAffects(session, mod_id, name, "ATE", affects->get());
//			}
//			if (terrainRequired) {
//				insertEffectTerrainRequired(session, mod_id, name, "ATE", terrainRequired->get());
//			}
//		}
//	}
//
//	void submitPassiveGlobalEffectData(boost::json::value const& passiveGlobalEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& passiveGlobalEffect : passiveGlobalEffectData.as_array()) {
//			auto const& name = passiveGlobalEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(passiveGlobalEffect, "targets");
//			auto variantMod = getOptionalString(passiveGlobalEffect, "variantMod");
//			auto variantHintMod = getOptionalValue(passiveGlobalEffect, "variantHintMod");
//			auto movementClassVariantReplace = getOptionalString(passiveGlobalEffect, "movementClassVariantReplace");
//			auto movementClassVariantOverride = getOptionalString(passiveGlobalEffect, "movementClassVariantOverride");
//
//			auto passiveGlobalEffectInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.PASSIVE_GLOBAL_EFFECT
//					(
//						MOD_ID, NAME, VARIANTMOD, MOVEMENTCLASSVARIANTREPLACE, MOVEMENTCLASSVARIANTOVERRIDE
//					)
//				values
//					(
//						?, ?, ?, ?, ?
//					)
//			)SQL");
//			passiveGlobalEffectInsertStatement.bind(mod_id, name);
//			bindOptional(variantMod, passiveGlobalEffectInsertStatement);
//			bindOptional(movementClassVariantReplace, passiveGlobalEffectInsertStatement);
//			bindOptional(movementClassVariantOverride, passiveGlobalEffectInsertStatement);
//
//			passiveGlobalEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "PGE", targets->get());
//			}
//
//			if (variantHintMod) {
//				for (auto const& entry : variantHintMod->get().as_object()) {
//					auto const& variantName = entry.key_c_str();
//					auto const& hint = entry.value().as_int64();
//
//					auto variantHintModInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.PGE_VARIANTHINTMOD_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, VARIANT, HINTMOD
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					variantHintModInsertStatement.bind(mod_id, name, variantName, hint);
//					variantHintModInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitActiveGlobalEffectData(boost::json::value const& activeGlobalEffectData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& activeGlobalEffect : activeGlobalEffectData.as_array()) {
//			auto const& name = activeGlobalEffect.at("name").as_string().c_str();
//			auto targets = getOptionalArray(activeGlobalEffect, "targets");
//			auto fundMod = getOptional<int64_t>(activeGlobalEffect, "fundMod");
//			auto fundFlatMod = getOptional<int64_t>(activeGlobalEffect, "fundFlatMod");
//			auto powerBarMod = getOptional<int64_t>(activeGlobalEffect, "powerBarMod");
//			auto powerBarPerFunds = getOptional<int64_t>(activeGlobalEffect, "powerBarPerFunds");
//			auto missileCount = getOptional<int64_t>(activeGlobalEffect, "missileCount");
//			auto missileTargetMethod = getOptionalArray(activeGlobalEffect, "missileTargetMethod");
//			auto missileDamage = getOptional<int64_t>(activeGlobalEffect, "missileDamage");
//			auto missileAreaOfEffect = getOptional<int64_t>(activeGlobalEffect, "missileAreaOfEffect");
//			auto missileStunDuration = getOptional<int64_t>(activeGlobalEffect, "missileStunDuration");
//
//			auto activeGlobalEffectInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.ACTIVE_GLOBAL_EFFECT
//					(
//						MOD_ID, NAME, FUNDMOD, FUNDFLATMOD, POWERBARMOD, POWERBARPERFUNDS, MISSILECOUNT, MISSILEDAMAGE, MISSILEAREAOFEFFECT, MISSILESTUNDURATION
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//			activeGlobalEffectInsertStatement.bind(mod_id, name);
//			bindOptional(fundMod, activeGlobalEffectInsertStatement);
//			bindOptional(fundFlatMod, activeGlobalEffectInsertStatement);
//			bindOptional(powerBarMod, activeGlobalEffectInsertStatement);
//			bindOptional(powerBarPerFunds, activeGlobalEffectInsertStatement);
//			bindOptional(missileCount, activeGlobalEffectInsertStatement);
//			bindOptional(missileDamage, activeGlobalEffectInsertStatement);
//			bindOptional(missileAreaOfEffect, activeGlobalEffectInsertStatement);
//			bindOptional(missileStunDuration, activeGlobalEffectInsertStatement);
//
//			activeGlobalEffectInsertStatement.execute();
//
//			if (targets) {
//				insertEffectTargets(session, mod_id, name, "AGE", targets->get());
//			}
//
//			if (missileTargetMethod) {
//				for (auto const& targetMethod : missileTargetMethod->get()) {
//					auto const& targetName = targetMethod.as_string().c_str();
//
//					auto targetMethodInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.AGE_MISSILETARGETMETHOD_REFERENCE
//							(
//								MOD_ID, EFFECT_NAME, TARGETMETHOD
//							)
//						values
//							(
//								?, ?, ?
//							)
//					)SQL");
//					targetMethodInsertStatement.bind(mod_id, name, targetName);
//					targetMethodInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void submitDefaultSettingsData(boost::json::value const& defaultSettingsData, mysqlx::Session& session, uint64_t mod_id) {
//		for (auto const& defaultSettings : defaultSettingsData.as_array()) {
//			auto const& name = defaultSettings.at("name").as_string().c_str();
//			auto startingFunds = getOptional<int64_t>(defaultSettings, "startingFunds");
//			auto incomeMultiplier = getOptional<int64_t>(defaultSettings, "incomeMultiplier");
//			auto fogOfWar = getOptionalBoolAsInt(defaultSettings, "fogOfWar");
//			auto variant = getOptionalValue(defaultSettings, "variant");
//			auto coPowers = getOptionalBoolAsInt(defaultSettings, "coPowers");
//			auto teams = getOptionalBoolAsInt(defaultSettings, "teams");
//			auto unitLimit = getOptional<int64_t>(defaultSettings, "unitLimit");
//			auto captureLimit = getOptional<int64_t>(defaultSettings, "captureLimit");
//			auto dayLimit = getOptional<int64_t>(defaultSettings, "dayLimit");
//			auto coMeterSize = getOptional<int64_t>(defaultSettings, "coMeterSize");
//			auto coMeterMultiplier = getOptional<int64_t>(defaultSettings, "coMeterMultiplier");
//
//			auto defaultSettingsInsertStatement = session.sql(R"SQL(
//				insert into
//					DATA.DEFAULT_GAME_SETTINGS
//					(
//						MOD_ID, NAME, FOGOFWAR, COPOWERS, TEAMS, STARTINGFUNDS, INCOMEMULTIPLIER, UNITLIMIT, CAPTURELIMIT, DAYLIMIT, COMETERSIZE, COMETERMULTIPLIER
//					)
//				values
//					(
//						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
//					)
//			)SQL");
//			defaultSettingsInsertStatement.bind(mod_id, name);
//			bindOptional(fogOfWar, defaultSettingsInsertStatement);
//			bindOptional(coPowers, defaultSettingsInsertStatement);
//			bindOptional(teams, defaultSettingsInsertStatement);
//			bindOptional(startingFunds, defaultSettingsInsertStatement);
//			bindOptional(incomeMultiplier, defaultSettingsInsertStatement);
//			bindOptional(unitLimit, defaultSettingsInsertStatement);
//			bindOptional(captureLimit, defaultSettingsInsertStatement);
//			bindOptional(dayLimit, defaultSettingsInsertStatement);
//			bindOptional(coMeterSize, defaultSettingsInsertStatement);
//			bindOptional(coMeterMultiplier, defaultSettingsInsertStatement);
//
//			defaultSettingsInsertStatement.execute();
//
//			if (variant) {
//				for (auto const& entry : variant->get().as_object()) {
//					auto const& variantName = entry.key_c_str();
//					auto const& weight = entry.value().as_int64();
//
//					auto settingsVariantInsertStatement = session.sql(R"SQL(
//						insert into
//							DATA.DEFAULTSETTINGS_VARIANT_REFERENCE
//							(
//								MOD_ID, SETTING_NAME, VARIANT, VALUE
//							)
//						values
//							(
//								?, ?, ?, ?
//							)
//					)SQL");
//					settingsVariantInsertStatement.bind(mod_id, name, variantName, weight);
//					settingsVariantInsertStatement.execute();
//				}
//			}
//		}
//	}
//
//	void extractModData(boost::json::value value) {
//		auto session = sqlutil::getSession();
//		try {
//			session.startTransaction();
//
//			auto const& obj = value;
//
//			std::cerr << "MetaData" << std::endl;
//			auto const& metadata = obj.at("modMetaData");
//			auto mod_id = submitMetaData(metadata, session);
//			std::cerr << "UnitData" << std::endl;
//			auto const& unitData = obj.at("units");
//			submitUnitData(unitData, session, mod_id);
//			std::cerr << "WeaponData" << std::endl;
//			auto const& weaponData = obj.at("weapons");
//			submitWeaponData(weaponData, session, mod_id);
//			std::cerr << "TerrainData" << std::endl;
//			auto const& terrainData = obj.at("terrains");
//			submitTerrainData(terrainData, session, mod_id);
//			std::cerr << "MovementData" << std::endl;
//			auto const& movementData = obj.at("movementClasses");
//			submitMovementData(movementData, session, mod_id);
//			std::cerr << "CommanderData" << std::endl;
//			auto const& commanderData = obj.at("commanders");
//			submitCommanderData(commanderData, session, mod_id);
//			auto const& playerData = obj.at("playerTypes");
//			std::cerr << "PlayerData" << std::endl;
//			submitPlayerData(playerData, session, mod_id);
//			std::cerr << "PassiveUnitEffectData" << std::endl;
//			auto const& passiveUnitEffectData = obj.at("passiveUnitEffects");
//			submitPassiveUnitEffectData(passiveUnitEffectData, session, mod_id);
//			std::cerr << "ActiveUnitEffectData" << std::endl;
//			auto const& activeUnitEffectData = obj.at("activeUnitEffects");
//			submitActiveUnitEffectData(activeUnitEffectData, session, mod_id);
//			std::cerr << "PassiveTerrainEffectData" << std::endl;
//			auto const& passiveTerrainEffectData = obj.at("passiveTerrainEffects");
//			submitPassiveTerrainEffectData(passiveTerrainEffectData, session, mod_id);
//			std::cerr << "ActiveTerrainEffectData" << std::endl;
//			auto const& activeTerrainEffectData = obj.at("activeTerrainEffects");
//			submitActiveTerrainEffectData(activeTerrainEffectData, session, mod_id);
//			std::cerr << "PassiveGlobalEffectData" << std::endl;
//			auto const& passiveGlobalEffectData = obj.at("passiveGlobalEffects");
//			submitPassiveGlobalEffectData(passiveGlobalEffectData, session, mod_id);
//			std::cerr << "ActiveGlobalEffectData" << std::endl;
//			auto const& activeGlobalEffectData = obj.at("activeGlobalEffects");
//			submitActiveGlobalEffectData(activeGlobalEffectData, session, mod_id);
//
//			std::cerr << "DefaultSettingsData" << std::endl;
//			auto const& defaultSettingsData = obj.at("defaultSettings");
//			submitDefaultSettingsData(defaultSettingsData, session, mod_id);
//
//			session.commit();
//		}
//		catch (boost::system::system_error const& e) {
//			std::cerr << "Rollback" << std::endl;
//			session.rollback();
//			std::cerr << e.what() << std::endl;
//			throw e;
//		}
//		catch (std::invalid_argument const& e) {
//			std::cerr << "Rollback" << std::endl;
//			session.rollback();
//			std::cerr << e.what() << std::endl;
//			throw e;
//		}
//		catch (std::out_of_range const& e) {
//			std::cerr << "Rollback" << std::endl;
//			session.rollback();
//			std::cerr << e.what() << std::endl;
//			throw e;
//		}
//		catch (...) {
//			std::cerr << "Rollback" << std::endl;
//			session.rollback();
//			std::rethrow_exception(std::current_exception());
//		}
//	}
//}
//
//std::string rest::data::upload_mod(net::HTTPHeaders const& headers, std::string body) {
//	auto it = headers.httpHeaders.find("cookies");
//	if (it == headers.httpHeaders.end()) {
//		throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
//	}
//	if (auto const& value = it->second; value.find(secret_key) == std::string::npos) {
//		throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
//	}
//	try {
//		extractModData(boost::json::parse(body));
//		return "success";
//	}
//	catch (boost::system::system_error const& e) {
//		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
//	}
//	catch (std::invalid_argument const& e) {
//		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
//	}
//	catch (std::out_of_range const& e) {
//		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
//	}
//	catch (std::exception const& e) {
//		throw net::RestError("Unknown Error: " + std::string(e.what()), net::RestError::Type::INTERNAL_ERROR);
//	}
//	catch (...) {
//		try {
//			std::rethrow_exception(std::current_exception());
//		}
//		catch (std::exception const& e) {
//			throw net::RestError("Unknown Error: " + std::string(e.what()), net::RestError::Type::INTERNAL_ERROR);
//		}
//	}
//}