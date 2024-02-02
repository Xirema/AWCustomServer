#include<RestFunctions.h>
#include<SQLUtil.h>
#include<boost/algorithm/string.hpp>

namespace db {
	namespace {
		//const std::string secret_key = "a8f7af7d81269941f6d78eab35632eb0c603bd3ece95f8df7ab4a41ad187b06a";
		const std::string supportedProtocol = "AWC000001";
		namespace mysql = boost::mysql;

		uint64_t submitMetaData(dTypes::ModMetadata const& metadata, mysql::tcp_ssl_connection& connection) {
			auto statement = connection.prepare_statement(R"SQL(
			update 
				DATA.MOD
			set 
				EXPIRED = current_timestamp
			where
				NAME = ?
				and EXPIRED is null
		)SQL");
			mysql::results results;
			connection.execute(statement.bind(metadata.name), results);

			statement = connection.prepare_statement(R"SQL(
			insert into 
				DATA.MOD
				(NAME, VERSION)
			values
				(?, ?)
		)SQL");
			results = {};
			connection.execute(statement.bind(metadata.name, metadata.version), results);
			uint64_t modId = results.last_insert_id();

			if (metadata.defaultResourcePacks) {
				int order = 0;
				statement = connection.prepare_statement(R"SQL(
				insert into
					DATA.MOD_DEFAULTPACK_REFERENCE
					(MOD_ID, NAME, `ORDER`, VERSION)
				values
					(?, ?, ?, ?)
			)SQL");
				for (auto const& defaultResourcePack : *metadata.defaultResourcePacks) {
					results = {};
					connection.execute(statement.bind(modId, defaultResourcePack.name, order, defaultResourcePack.version), results);
					order++;
				}
			}
			return modId;
		}

		void submitUnitData(std::vector<dTypes::UnitType> const& unitData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto unitInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.UNIT_TYPE
				(
					MOD_ID, NAME, COST, MAXFUEL, MAXAMMO, VISIONRANGE, MOVEMENTCLASS, MOVEMENTRANGE, FUELPERDAY, FUELPERDAYSTEALTH,
					SUPPLYREPAIR, TRANSPORTCAPACITY, HITPOINTS, CAPTURESPEED, IGNORESVISIONOCCLUSION, STEALTHTYPE, STATIONARYFIRE
				)
			values 
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto unitWeaponInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.UNIT_WEAPON_REFERENCE
				(
					MOD_ID, UNITTYPE_NAME, WEAPONTYPE_NAME, WEAPONORDER
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto unitTransportListInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.UNIT_TRANSPORTLIST_REFERENCE
				(
					MOD_ID, UNITTYPE_NAME, TRANSPORTED_NAME
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			auto unitClassificationInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.UNIT_CLASSIFICATION_REFERENCE
				(
					MOD_ID, UNITTYPE_NAME, CLASSIFICATION
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			for (auto const& unitType : unitData) {
				mysql::results results;
				connection.execute(
					unitInsertStatement.bind(
						mod_id,
						unitType.name,
						unitType.cost,
						unitType.maxFuel,
						unitType.maxAmmo,
						unitType.visionRange,
						unitType.movementClass,
						unitType.movementRange,
						unitType.fuelPerDay,
						unitType.fuelPerDayStealth,
						unitType.supplyRepair,
						unitType.transportCapacity,
						unitType.hitPoints,
						unitType.captureSpeed,
						unitType.ignoresVisionOcclusion,
						unitType.stealthType,
						unitType.stationaryFire
					)
					,
					results
				);

				if (unitType.weapons) {
					int order = 0;
					for (auto const& weapon : *unitType.weapons) {
						mysql::results results;
						connection.execute(unitWeaponInsertStatement.bind(mod_id, unitType.name, weapon, order), results);
						order++;
					}
				}

				if (unitType.transportList) {
					for (auto const& transportableUnit : *unitType.transportList) {
						mysql::results results;
						connection.execute(unitTransportListInsertStatement.bind(mod_id, unitType.name, transportableUnit), results);
					}
				}

				for (auto const& classification : unitType.classifications) {
					mysql::results results;
					connection.execute(unitClassificationInsertStatement.bind(mod_id, unitType.name, classification), results);
				}
			}
		}

		void submitWeaponData(std::vector<dTypes::WeaponType> const& weaponData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto weaponInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.WEAPON_TYPE
				(
					MOD_ID, NAME, AMMOCONSUMED, MAXRANGE, MINRANGE, SELFTARGET, AFFECTEDBYLUCK, NONLETHAL, AREAOFEFFECT, FLATDAMAGE
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto weaponBaseDamageInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.WEAPON_BASEDAMAGE_REFERENCE
				(
					MOD_ID, WEAPONTYPE_NAME, UNITTYPE_NAME, DAMAGE
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto weaponTargetsStealthInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.WEAPON_STEALTHTARGET_REFERENCE
				(
					MOD_ID, WEAPONTYPE_NAME, STEALTHTYPE
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			for (auto const& weaponType : weaponData) {
				mysql::results results;
				connection.execute(
					weaponInsertStatement.bind(
						mod_id,
						weaponType.name,
						weaponType.ammoConsumed,
						weaponType.maxRange,
						weaponType.minRange,
						weaponType.selfTarget,
						weaponType.affectedByLuck,
						weaponType.nonLethal,
						weaponType.areaOfEffect,
						weaponType.flatDamage
					)
					,
					results
				);

				if (weaponType.baseDamage) {
					for (auto const& entry : *weaponType.baseDamage) {
						mysql::results results;
						connection.execute(weaponBaseDamageInsertStatement.bind(mod_id, weaponType.name, entry.first, entry.second), results);
					}
				}

				if (weaponType.targetsStealth) {
					for (auto const& targetsStealthValue : *weaponType.targetsStealth) {
						mysql::results results;
						connection.execute(weaponTargetsStealthInsertStatement.bind(mod_id, weaponType.name, targetsStealthValue), results);
					}
				}
			}
		}

		void submitTerrainData(std::vector<dTypes::TerrainType> const& terrainData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto terrainInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.TERRAIN_TYPE
				(
					MOD_ID, NAME, STARS, MAXCAPTUREPOINTS, SAMEAS, INCOME, REPAIR, OCCLUDESVISION, HITPOINTS, DESTROYED, 
					DAMAGEDLIKE, ACTIVATIONMAX, ACTIVATIONCHANGE, LOSEIFCAPTURED, LOSEIFALLCAPTURED, DESTROYEDORIENTATION
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto terrainBuildInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.TERRAIN_BUILDREPAIR_REFERENCE
				(
					MOD_ID, TERRAINTYPE_NAME, UNITTYPE_NAME, UNITORDER, BUILDREPAIR
				)
			values
				(
					?, ?, ?, ?, 1
				)
		)SQL");
			auto terrainRepairInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.TERRAIN_BUILDREPAIR_REFERENCE
				(
					MOD_ID, TERRAINTYPE_NAME, UNITTYPE_NAME, BUILDREPAIR
				)
			values
				(
					?, ?, ?, 2
				)
		)SQL");
			auto terrainActivateListInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.TERRAIN_ACTIVATIONLIST_REFERENCE
				(
					MOD_ID, TERRAINTYPE_NAME, UNITTYPE_NAME
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			auto terrainActivateEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.TERRAIN_ACTIVATIONEFFECT_REFERENCE
				(
					MOD_ID, TERRAINTYPE_NAME, EFFECT_TYPE, EFFECT_NAME
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& terrainType : terrainData) {
				mysql::results results;
				connection.execute(
					terrainInsertStatement.bind(
						mod_id,
						terrainType.name,
						terrainType.stars,
						terrainType.maxCapturePoints,
						terrainType.sameAs,
						terrainType.income,
						terrainType.repair,
						terrainType.occludesVision,
						terrainType.hitPoints,
						terrainType.destroyed,
						terrainType.damagedLike,
						terrainType.activationMax,
						terrainType.activationChange,
						terrainType.loseIfCaptured,
						terrainType.loseIfAllCaptured,
						terrainType.destroyedOrientation
					)
					,
					results
				);

				if (terrainType.buildList) {
					int order = 0;
					for (auto const& buildUnit : *terrainType.buildList) {
						mysql::results results;
						connection.execute(terrainBuildInsertStatement.bind(mod_id, terrainType.name, buildUnit, order), results);
						order++;
					}
				}

				if (terrainType.repairList) {
					for (auto const& repairUnit : *terrainType.repairList) {
						mysql::results results;
						connection.execute(terrainRepairInsertStatement.bind(mod_id, terrainType.name, repairUnit), results);
					}
				}

				if (terrainType.activateList) {
					for (auto const& activateUnit : *terrainType.activateList) {
						mysql::results results;
						connection.execute(terrainActivateListInsertStatement.bind(mod_id, terrainType.name, activateUnit), results);
					}
				}

				auto bindEffects = [&connection, &mod_id, &terrainType, &terrainActivateEffectInsertStatement](std::vector<std::string> const& effectList, std::string_view effectType) {
					for (auto const& activateEffect : effectList) {
						mysql::results results;
						connection.execute(terrainActivateEffectInsertStatement.bind(mod_id, terrainType.name, effectType, activateEffect), results);
					}
				};

				if (terrainType.activateActiveUnitEffects) {
					bindEffects(*terrainType.activateActiveUnitEffects, "AUE");
				}
				if (terrainType.activateActiveTerrainEffects) {
					bindEffects(*terrainType.activateActiveTerrainEffects, "ATE");
				}
				if (terrainType.activateActiveGlobalEffects) {
					bindEffects(*terrainType.activateActiveGlobalEffects, "AGE");
				}
			}
		}

		void submitMovementData(std::vector<dTypes::MovementClass> const& movementData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto movementInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.MOVEMENT_TYPE
				(
					MOD_ID, NAME
				)
			values
				(
					?, ?
				)
		)SQL");
			auto movementCostInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.MOVEMENT_COST_REFERENCE
				(
					MOD_ID, MOVEMENT_NAME, TERRAINTYPE_NAME, COST
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto movementCostVariantInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.MOVEMENT_COST_REFERENCE
				(
					MOD_ID, MOVEMENT_NAME, TERRAINTYPE_NAME, COST, VARIANT
				)
			values
				(
					?, ?, ?, ?, ?
				)
		)SQL");
			for (auto const& movementClass : movementData) {
				mysql::results results;
				connection.execute(movementInsertStatement.bind(mod_id, movementClass.name), results);

				for (auto const& entry : movementClass.movementCosts) {
					mysql::results results;
					connection.execute(movementCostInsertStatement.bind(mod_id, movementClass.name, entry.first, entry.second), results);
				}

				if (movementClass.variantMods) {
					for (auto const& [variantName, map] : *movementClass.variantMods) {
						for (auto const& [terrainName, costMod] : map) {
							mysql::results results;
							connection.execute(movementCostVariantInsertStatement.bind(mod_id, movementClass.name, terrainName, costMod, variantName), results);
						}
					}
				}
			}
		}

		void submitCommanderData(std::vector<dTypes::CommanderType> const& commanderData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto commanderInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.COMMANDER_TYPE
				(
					MOD_ID, NAME, COPCOST, SCOPCOST, COMETERMULTIPLIER, PLAYABLE
				)
			values
				(
					?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto effectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.COMMANDER_EFFECT_REFERENCE
				(
					MOD_ID, COMMANDER_NAME, EFFECT_NAME, EFFECT_TYPE, EFFECT_CATEGORY
				)
			values
				(
					?, ?, ?, ?, ?
				)
		)SQL");
			for (auto const& commanderType : commanderData) {
				mysql::results results;
				connection.execute(
					commanderInsertStatement.bind(
						mod_id,
						commanderType.name,
						commanderType.copCost,
						commanderType.scopCost,
						commanderType.coMeterMultiplier,
						commanderType.playable
					)
					,
					results
				);

				auto insertEffects = [&connection, &commanderType, mod_id, &effectInsertStatement](std::vector<std::string> const& effects, std::string_view effectType, int effectCategory) {
					for (auto const& effect : effects) {
						mysql::results results;
						connection.execute(effectInsertStatement.bind(mod_id, commanderType.name, effect, effectType, effectCategory), results);
					}
				};
				if (commanderType.passiveUnitEffectsD2d)
					insertEffects(*commanderType.passiveUnitEffectsD2d, "PUE", 0);
				if (commanderType.passiveTerrainEffectsD2d)
					insertEffects(*commanderType.passiveTerrainEffectsD2d, "PTE", 0);
				if (commanderType.passiveGlobalEffectsD2d)
					insertEffects(*commanderType.passiveGlobalEffectsD2d, "PGE", 0);

				if (commanderType.passiveUnitEffectsCop)
					insertEffects(*commanderType.passiveUnitEffectsCop, "PUE", 1);
				if (commanderType.activeUnitEffectsCop)
					insertEffects(*commanderType.activeUnitEffectsCop, "AUE", 1);
				if (commanderType.passiveTerrainEffectsCop)
					insertEffects(*commanderType.passiveTerrainEffectsCop, "PTE", 1);
				if (commanderType.activeTerrainEffectsCop)
					insertEffects(*commanderType.activeTerrainEffectsCop, "ATE", 1);
				if (commanderType.passiveGlobalEffectsCop)
					insertEffects(*commanderType.passiveGlobalEffectsCop, "PGE", 1);
				if (commanderType.activeGlobalEffectsCop)
					insertEffects(*commanderType.activeGlobalEffectsCop, "AGE", 1);

				if (commanderType.passiveUnitEffectsScop)
					insertEffects(*commanderType.passiveUnitEffectsScop, "PUE", 2);
				if (commanderType.activeUnitEffectsScop)
					insertEffects(*commanderType.activeUnitEffectsScop, "AUE", 2);
				if (commanderType.passiveTerrainEffectsScop)
					insertEffects(*commanderType.passiveTerrainEffectsScop, "PTE", 2);
				if (commanderType.activeTerrainEffectsScop)
					insertEffects(*commanderType.activeTerrainEffectsScop, "ATE", 2);
				if (commanderType.passiveGlobalEffectsScop)
					insertEffects(*commanderType.passiveGlobalEffectsScop, "PGE", 2);
				if (commanderType.activeGlobalEffectsScop)
					insertEffects(*commanderType.activeGlobalEffectsScop, "AGE", 2);
			}
		}

		void submitPlayerData(std::vector<dTypes::PlayerType> const& playerData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto playerInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PLAYER_TYPE
				(
					MOD_ID, NAME, COMMANDERTYPEMOD, TEAMNAME
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto playerSlotInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PLAYER_PERMITTEDPLAYERSLOT_REFERENCE
				(
					MOD_ID, PLAYERTYPE_NAME, PERMITTEDPLAYERSLOT
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			auto commanderTypeInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PLAYER_PERMITTEDCOMMANDERTYPE_REFERENCE
				(
					MOD_ID, PLAYERTYPE_NAME, PERMITTEDCOMMANDERTYPE
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			for (auto const& playerType : playerData) {
				mysql::results results;
				connection.execute(
					playerInsertStatement.bind(
						mod_id,
						playerType.name,
						playerType.commanderTypeMod,
						playerType.teamName
					)
					,
					results
				);

				if (playerType.permittedPlayerSlots) {
					for (auto const& playerSlot : *playerType.permittedPlayerSlots) {
						mysql::results results;
						connection.execute(playerSlotInsertStatement.bind(mod_id, playerType.name, playerSlot), results);
					}
				}

				if (playerType.permittedCommanderTypes) {
					for (auto const& commanderType : *playerType.permittedCommanderTypes) {
						mysql::results results;
						connection.execute(commanderTypeInsertStatement.bind(mod_id, playerType.name, commanderType), results);
					}
				}
			}
		}

		void insertEffectTargets(mysql::tcp_ssl_connection& connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& targetArray) {
			auto effectTargetInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.EFFECT_TARGET_REFERENCE
				(
					MOD_ID, EFFECT_NAME, EFFECT_TYPE, TARGET
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& target : targetArray) {
				mysql::results results;
				connection.execute(effectTargetInsertStatement.bind(mod_id, effectName, effectType, target), results);
			}
		}
		void insertEffectAffects(mysql::tcp_ssl_connection& connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& affectArray) {
			auto effectTargetInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.EFFECT_AFFECT_REFERENCE
				(
					MOD_ID, EFFECT_NAME, EFFECT_TYPE, AFFECTS
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& affect : affectArray) {
				mysql::results results;
				connection.execute(effectTargetInsertStatement.bind(mod_id, effectName, effectType, affect), results);
			}
		}

		void insertEffectUnitTypeRequired(mysql::tcp_ssl_connection& connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& unitTypeArray) {
			auto effectUnitTypeInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.EFFECT_UNITTYPEREQUIRED_REFERENCE
				(
					MOD_ID, EFFECT_NAME, EFFECT_TYPE, UNITTYPE_NAME
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& unitType : unitTypeArray) {
				mysql::results results;
				connection.execute(effectUnitTypeInsertStatement.bind(mod_id, effectName, effectType, unitType), results);
			}
		}

		void insertEffectClassification(mysql::tcp_ssl_connection& connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& classificationArray) {
			auto insertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.EFFECT_CLASSIFICATION_REFERENCE
				(
					MOD_ID, EFFECT_NAME, EFFECT_TYPE, CLASSIFICATION
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& classification : classificationArray) {
				mysql::results results;
				connection.execute(insertStatement.bind(mod_id, effectName, effectType, classification), results);
			}
		}

		void insertEffectTerrainRequired(mysql::tcp_ssl_connection& connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& terrainArray) {
			auto insertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.EFFECT_TERRAIN_REFERENCE
				(
					MOD_ID, EFFECT_NAME, EFFECT_TYPE, TERRAINTYPE_NAME
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& terrain : terrainArray) {
				mysql::results results;
				connection.execute(insertStatement.bind(mod_id, effectName, effectType, terrain), results);
			}
		}

		void submitPassiveUnitEffectData(std::vector<dTypes::PassiveUnitEffect> const& passiveUnitEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto passiveUnitEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PASSIVE_UNIT_EFFECT
				(
					MOD_ID, NAME, FIREPOWERMOD, DEFENSEMOD, INDIRECTDEFENSEMOD, MINRANGEMOD, MAXRANGEMOD, FUELUSEMOD, AMMOUSEMOD, GOODLUCKMOD, BADLUCKMOD,
					MOVEMENTMOD, VISIONMOD, TERRAINSTARSMOD, TERRAINSTARSFLATMOD, TERRAINSTARSDEFENSE, TERRAINSTARSFIREPOWER, COUNTERFIREMOD, COUNTERFIRST,
					CAPTURERATEMOD, UNITCOSTMOD, FIREPOWERFROMFUNDS, DEFENSEFROMFUNDS, FUNDSFROMDAMAGE, COMETERCHARGEFROMDEALTDAMAGE,
					COMETERCHARGEFROMRECEIVEDDAMAGE
				)
			VALUES
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 
					?, ?, ?, ?, ?, ?, ?, ?,
					?, ?, ?, ?, ?, ?,
					?
				)
		)SQL");
			auto firepowerTerrainInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_FIREPOWERFROMTERRAIN_REFERENCE
				(
					MOD_ID, EFFECT_NAME, TERRAINTYPE_NAME, FIREPOWERMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto defenseTerrainInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_DEFENSEFROMTERRAIN_REFERENCE
				(
					MOD_ID, EFFECT_NAME, TERRAINTYPE_NAME, DEFENSEMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto visionVariantInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_VISIONVARIANT_REFERENCE
				(
					MOD_ID, EFFECT_NAME, VARIANT, VISIONMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto firepowerVariantInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_FIREPOWERVARIANT_REFERENCE
				(
					MOD_ID, EFFECT_NAME, VARIANT, FIREPOWERMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto defenseVariantInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_DEFENSEVARIANT_REFERENCE
				(
					MOD_ID, EFFECT_NAME, VARIANT, DEFENSEMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			auto intelInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PUE_INTEL_REFERENCE
				(
					MOD_ID, EFFECT_NAME, INTEL_TYPE, TARGET
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& pue : passiveUnitEffectData) {
				mysql::results results;
				connection.execute(
					passiveUnitEffectInsertStatement.bind(
						mod_id,
						pue.name,
						pue.firepowerMod,
						pue.defenseMod,
						pue.indirectDefenseMod,
						pue.minRangeMod,
						pue.maxRangeMod,
						pue.fuelUseMod,
						pue.ammoUseMod,
						pue.goodLuckMod,
						pue.badLuckMod,

						pue.movementMod,
						pue.visionMod,
						pue.terrainStarsMod,
						pue.terrainStarsFlatMod,
						pue.terrainStarsDefense,
						pue.terrainStarsFirepower,
						pue.counterfireMod,
						pue.counterFirst,

						pue.captureRateMod,
						pue.unitCostMod,
						pue.firepowerFromFunds,
						pue.defenseFromFunds,
						pue.fundsFromDamage,
						pue.coMeterChargeFromDealtDamage,

						pue.coMeterChargeFromReceivedDamage
					)
					,
					results
				);

				if (pue.targets) {
					insertEffectTargets(connection, mod_id, pue.name, "PUE", *pue.targets);
				}
				if (pue.unitTypeRequired) {
					insertEffectUnitTypeRequired(connection, mod_id, pue.name, "PUE", *pue.unitTypeRequired);
				}
				if (pue.classificationRequired) {
					insertEffectClassification(connection, mod_id, pue.name, "PUE", *pue.classificationRequired);
				}
				if (pue.terrainRequired) {
					insertEffectTerrainRequired(connection, mod_id, pue.name, "PUE", *pue.terrainRequired);
				}

				if (pue.firepowerFromOwnedTerrain) {
					for (auto const& [terrainName, firepowerMod] : *pue.firepowerFromOwnedTerrain) {
						mysql::results results;
						connection.execute(firepowerTerrainInsertStatement.bind(mod_id, pue.name, terrainName, firepowerMod), results);
					}
				}
				if (pue.defenseFromOwnedTerrain) {
					for (auto const& [terrainName, defenseMod] : *pue.defenseFromOwnedTerrain) {
						mysql::results results;
						connection.execute(defenseTerrainInsertStatement.bind(mod_id, pue.name, terrainName, defenseMod), results);
					}
				}
				if (pue.visionVariantMods) {
					for (auto const& [variantName, visionMod] : *pue.visionVariantMods) {
						mysql::results results;
						connection.execute(visionVariantInsertStatement.bind(mod_id, pue.name, variantName, visionMod), results);
					}
				}
				if (pue.firepowerVariantMods) {
					for (auto const& [variantName, firepowerMod] : *pue.firepowerVariantMods) {
						mysql::results results;
						connection.execute(firepowerVariantInsertStatement.bind(mod_id, pue.name, variantName, firepowerMod), results);
					}
				}
				if (pue.defenseVariantMods) {
					for (auto const& [variantName, defenseMod] : *pue.defenseVariantMods) {
						mysql::results results;
						connection.execute(defenseVariantInsertStatement.bind(mod_id, pue.name, variantName, defenseMod), results);
					}
				}
				if (pue.hiddenHitPoints) {
					for (auto const& hidden : *pue.hiddenHitPoints) {
						mysql::results results;
						connection.execute(intelInsertStatement.bind(mod_id, pue.name, "hiddenHitPoints", hidden), results);
					}
				}
				if (pue.luckPointsVisible) {
					for (auto const& luckPoints : *pue.luckPointsVisible) {
						mysql::results results;
						connection.execute(intelInsertStatement.bind(mod_id, pue.name, "luckPointsVisible", luckPoints), results);
					}
				}
				if (pue.hpPartVisible) {
					for (auto const& hpPart : *pue.hpPartVisible) {
						mysql::results results;
						connection.execute(intelInsertStatement.bind(mod_id, pue.name, "hpPartVisible", hpPart), results);
					}
				}
			}
		}

		void submitActiveUnitEffectData(std::vector<dTypes::ActiveUnitEffect> const& activeUnitEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto activeUnitEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.ACTIVE_UNIT_EFFECT
				(
					MOD_ID, NAME, HITPOINTMOD, ROUNDHITPOINTS, SETFUEL, SETAMMO, ADDFUEL, ADDAMMO, MULTIPLYFUEL, MULTIPLYAMMO, MAKEACTIVE, STUNDURATION, COCHARGEFACTOR
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			for (auto const& aue : activeUnitEffectData) {
				mysql::results results;
				connection.execute(
					activeUnitEffectInsertStatement.bind(
						mod_id,
						aue.name,
						aue.hitPointMod,
						aue.roundHitPoints,
						aue.setFuel,
						aue.setAmmo,
						aue.addFuel,
						aue.addAmmo,
						aue.multiplyFuel,
						aue.multiplyAmmo,
						aue.makeActive,
						aue.stunDuration,
						aue.coChargeFactor
					)
					,
					results
				);

				if (aue.targets) {
					insertEffectTargets(connection, mod_id, aue.name, "AUE", *aue.targets);
				}
				if (aue.unitTypeRequired) {
					insertEffectUnitTypeRequired(connection, mod_id, aue.name, "AUE", *aue.unitTypeRequired);
				}
				if (aue.classificationRequired) {
					insertEffectClassification(connection, mod_id, aue.name, "AUE", *aue.classificationRequired);
				}
				if (aue.terrainRequired) {
					insertEffectTerrainRequired(connection, mod_id, aue.name, "AUE", *aue.terrainRequired);
				}
			}
		}

		void submitPassiveTerrainEffectData(std::vector<dTypes::PassiveTerrainEffect> const& passiveTerrainEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto passiveTerrainEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into 
				DATA.PASSIVE_TERRAIN_EFFECT
				(
					MOD_ID, NAME, INCOMEMOD, INCOMEFLATMOD, REPAIRMOD, OCCLUDESVISIONMOD, VISIONMODBOOST, BUILDCOSTMOD
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto pteBuildListInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PTE_BUILDLISTMOD_REFERENCE
				(
					MOD_ID, EFFECT_NAME, UNITTYPE_NAME
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			for (auto const& pte : passiveTerrainEffectData) {
				mysql::results results;
				connection.execute(
					passiveTerrainEffectInsertStatement.bind(
						mod_id,
						pte.name,
						pte.incomeMod,
						pte.incomeFlatMod,
						pte.repairMod,
						pte.occludesVisionMod,
						pte.visionModBoost,
						pte.buildCostMod
					)
					,
					results
				);

				if (pte.targets) {
					insertEffectTargets(connection, mod_id, pte.name, "PTE", *pte.targets);
				}
				if (pte.affects) {
					insertEffectAffects(connection, mod_id, pte.name, "PTE", *pte.affects);
				}
				if (pte.classificationRequired) {
					insertEffectClassification(connection, mod_id, pte.name, "PTE", *pte.classificationRequired);
				}
				if (pte.terrainRequired) {
					insertEffectTerrainRequired(connection, mod_id, pte.name, "PTE", *pte.terrainRequired);
				}

				if (pte.buildListMod) {
					for (auto const& buildListItem : *pte.buildListMod) {
						mysql::results results;
						connection.execute(pteBuildListInsertStatement.bind(mod_id, pte.name, buildListItem), results);
					}
				}
			}
		}

		void submitActiveTerrainEffectData(std::vector<dTypes::ActiveTerrainEffect> const& activeTerrainEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto activeTerrainEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.ACTIVE_TERRAIN_EFFECT
				(
					MOD_ID, NAME, UNITSUMMONEDNAME, UNITSUMMONEDINITIALDAMAGE, UNITSUMMONEDACTIVE
				)
			values
				(
					?, ?, ?, ?, ?
				)
		)SQL");
			for (auto const& ate : activeTerrainEffectData) {
				mysql::results results;
				connection.execute(
					activeTerrainEffectInsertStatement.bind(
						mod_id,
						ate.name,
						ate.unitSummonedName,
						ate.unitSummonedInitialDamage,
						ate.unitSummonedActive
					)
					,
					results
				);

				if (ate.targets) {
					insertEffectTargets(connection, mod_id, ate.name, "ATE", *ate.targets);
				}
				if (ate.affects) {
					insertEffectAffects(connection, mod_id, ate.name, "ATE", *ate.affects);
				}
				if (ate.terrainRequired) {
					insertEffectTerrainRequired(connection, mod_id, ate.name, "ATE", *ate.terrainRequired);
				}
			}
		}

		void submitPassiveGlobalEffectData(std::vector<dTypes::PassiveGlobalEffect> const& passiveGlobalEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto passiveGlobalEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PASSIVE_GLOBAL_EFFECT
				(
					MOD_ID, NAME, VARIANTMOD, MOVEMENTCLASSVARIANTREPLACE, MOVEMENTCLASSVARIANTOVERRIDE, MINIMUMVISIONMOD
				)
			values
				(
					?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto variantHintModInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.PGE_VARIANTHINTMOD_REFERENCE
				(
					MOD_ID, EFFECT_NAME, VARIANT, HINTMOD
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& pge : passiveGlobalEffectData) {
				mysql::results results;
				connection.execute(
					passiveGlobalEffectInsertStatement.bind(
						mod_id,
						pge.name,
						pge.variantMod,
						pge.movementClassVariantReplace,
						pge.movementClassVariantOverride,
						pge.minimumVisionMod
					)
					,
					results
				);

				if (pge.targets) {
					insertEffectTargets(connection, mod_id, pge.name, "PGE", *pge.targets);
				}

				if (pge.variantHintMod) {
					for (auto const& [variantName, hint] : *pge.variantHintMod) {
						mysql::results results;
						connection.execute(variantHintModInsertStatement.bind(mod_id, pge.name, variantName, hint), results);
					}
				}
			}
		}

		void submitActiveGlobalEffectData(std::vector<dTypes::ActiveGlobalEffect> const& activeGlobalEffectData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto activeGlobalEffectInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.ACTIVE_GLOBAL_EFFECT
				(
					MOD_ID, NAME, FUNDMOD, FUNDFLATMOD, POWERBARMOD, POWERBARPERFUNDS, MISSILECOUNT, MISSILEDAMAGE, MISSILEAREAOFEFFECT, MISSILESTUNDURATION, COCHARGEFACTOR
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto targetMethodInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.AGE_MISSILETARGETMETHOD_REFERENCE
				(
					MOD_ID, EFFECT_NAME, TARGETMETHOD
				)
			values
				(
					?, ?, ?
				)
		)SQL");
			for (auto const& age : activeGlobalEffectData) {
				mysql::results results;
				connection.execute(
					activeGlobalEffectInsertStatement.bind(
						mod_id,
						age.name,
						age.fundMod,
						age.fundFlatMod,
						age.powerBarMod,
						age.powerBarPerFunds,
						age.missileCount,
						age.missileDamage,
						age.missileAreaOfEffect,
						age.missileStunDuration,
						age.coChargeFactor
					)
					,
					results
				);

				if (age.targets) {
					insertEffectTargets(connection, mod_id, age.name, "AGE", *age.targets);
				}

				if (age.missileTargetMethod) {
					for (auto const& targetMethod : *age.missileTargetMethod) {
						mysql::results results;
						connection.execute(targetMethodInsertStatement.bind(mod_id, age.name, targetMethod), results);
					}
				}
			}
		}

		void submitDefaultSettingsData(std::vector<dTypes::Settings> const& defaultSettingsData, mysql::tcp_ssl_connection& connection, uint64_t mod_id) {
			auto defaultSettingsInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.DEFAULT_GAME_SETTINGS
				(
					MOD_ID, NAME, FOGOFWAR, COPOWERS, TEAMS, STARTINGFUNDS, INCOMEMULTIPLIER, UNITLIMIT, CAPTURELIMIT, DAYLIMIT, COMETERSIZE, COMETERMULTIPLIER
				)
			values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
				)
		)SQL");
			auto settingsVariantInsertStatement = connection.prepare_statement(R"SQL(
			insert into
				DATA.DEFAULTSETTINGS_VARIANT_REFERENCE
				(
					MOD_ID, SETTING_NAME, VARIANT, VALUE
				)
			values
				(
					?, ?, ?, ?
				)
		)SQL");
			for (auto const& settings : defaultSettingsData) {
				mysql::results results;
				connection.execute(
					defaultSettingsInsertStatement.bind(
						mod_id,
						settings.name,
						settings.fogOfWar,
						settings.coPowers,
						settings.teams,
						settings.startingFunds,
						settings.incomeMultiplier,
						settings.unitLimit,
						settings.captureLimit,
						settings.dayLimit,
						settings.coMeterSize,
						settings.coMeterMultiplier
					)
					,
					results
				);

				if (settings.variant) {
					for (auto const& [variantName, weight] : *settings.variant) {
						mysql::results results;
						connection.execute(settingsVariantInsertStatement.bind(mod_id, settings.name, variantName, weight), results);
					}
				}
			}
		}
	}

	bool upload_mod(dTypes::ModData const& modData) {
		try {
			sqlutil::Session session;
			sqlutil::Transaction transaction(session);
			auto modId = submitMetaData(modData.modMetadata, session.connection);
			submitUnitData(modData.units, session.connection, modId);
			submitWeaponData(modData.weapons, session.connection, modId);
			submitTerrainData(modData.terrains, session.connection, modId);
			submitMovementData(modData.movements, session.connection, modId);
			submitCommanderData(modData.commanders, session.connection, modId);
			submitPlayerData(modData.players, session.connection, modId);
			submitPassiveUnitEffectData(modData.passiveUnitEffects, session.connection, modId);
			submitPassiveTerrainEffectData(modData.passiveTerrainEffects, session.connection, modId);
			submitPassiveGlobalEffectData(modData.passiveGlobalEffects, session.connection, modId);
			submitActiveUnitEffectData(modData.activeUnitEffects, session.connection, modId);
			submitActiveTerrainEffectData(modData.activeTerrainEffects, session.connection, modId);
			submitActiveGlobalEffectData(modData.activeGlobalEffects, session.connection, modId);
			submitDefaultSettingsData(modData.defaultSettings, session.connection, modId);

			transaction.commit();
			return true;
		}
		catch (std::runtime_error const& e) {
			throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
		}
	}

	namespace {
		namespace mysql = boost::mysql;
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
			set(terrain.name, row[2]);
			set(terrain.stars, row[3]);
			set(terrain.maxCapturePoints, row[4]);
			set(terrain.sameAs, row[5]);
			set(terrain.income, row[6]);
			set(terrain.repair, row[7]);
			set(terrain.occludesVision, row[8]);
			set(terrain.hitPoints, row[9]);
			set(terrain.destroyed, row[10]);
			set(terrain.damagedLike, row[11]);
			set(terrain.activationMax, row[12]);
			set(terrain.activationChange, row[13]);
			set(terrain.loseIfCaptured, row[14]);
			set(terrain.loseIfAllCaptured, row[15]);
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
			set(effect.setFuel, row[5]);
			set(effect.setAmmo, row[6]);
			set(effect.addFuel, row[7]);
			set(effect.addAmmo, row[8]);
			set(effect.multiplyFuel, row[9]);
			set(effect.multiplyAmmo, row[10]);
			set(effect.makeActive, row[11]);
			set(effect.stunDuration, row[12]);
			set(effect.coChargeFactor, row[13]);
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
	}

	std::vector<dTypes::WeaponType> get_weapons(int64_t modId, std::optional<std::string_view> const& weaponName) {
		sqlutil::Session session;
		auto results = getLookup(
			session.connection,
			"WEAPON_TYPE",
			modId,
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
				modId,
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
				modId,
				newWeapon.name
			);
			if (stealthTargetsResults.size() > 0) {
				newWeapon.targetsStealth.emplace();
				for (auto const& stealthTargetsRow : stealthTargetsResults.rows()) {
					assignWeaponStealthTargetsData(stealthTargetsRow, newWeapon);
				}
			}
		}
		return weapons;
	}

	std::vector<dTypes::UnitType> get_units(int64_t modId, std::optional<std::string_view> const& unitName) {
		sqlutil::Session session;
		auto results = getLookup(session.connection, "UNIT_TYPE", modId, unitName);
		std::vector<dTypes::UnitType> units;
		for (auto const& row : results.rows()) {
			auto& newUnit = units.emplace_back();
			assignUnitData(row, newUnit);
			auto weaponResults = getReferenceLookup(
				session.connection,
				"UNIT_WEAPON_REFERENCE",
				"UNITTYPE_NAME",
				modId,
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
				modId,
				newUnit.name
			);
			for (auto const& classificationRow : classificationResults.rows()) {
				assignUnitClassificationData(classificationRow, newUnit);
			}
			auto transportListResults = getReferenceLookup(
				session.connection,
				"UNIT_TRANSPORTLIST_REFERENCE",
				"UNITTYPE_NAME",
				modId,
				newUnit.name
			);
			if (transportListResults.size() > 0) {
				newUnit.transportList.emplace();
				for (auto const& transportListRow : transportListResults.rows()) {
					assignUnitTransportListData(transportListRow, newUnit);
				}
			}
		}
		return units;
	}

	std::vector<dTypes::TerrainType> get_terrains(int64_t modId, std::optional<std::string_view> const& terrainName) {
		sqlutil::Session session;
		std::vector<dTypes::TerrainType> terrains;
		auto results = getLookup(
			session.connection,
			"TERRAIN_TYPE",
			modId,
			terrainName
		);
		for (auto const& row : results.rows()) {
			auto& newTerrain = terrains.emplace_back();
			assignTerrainData(row, newTerrain);
			auto buildRepairResults = getReferenceLookup(
				session.connection,
				"TERRAIN_BUILDREPAIR_REFERENCE",
				"TERRAINTYPE_NAME",
				modId,
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
				modId,
				newTerrain.name
			);
			for (auto const& activateListRow : activateListResults.rows()) {
				assignTerrainActivationListData(activateListRow, newTerrain);
			}

			auto activateEffectResults = getReferenceLookup(
				session.connection,
				"TERRAIN_ACTIVATIONEFFECT_REFERENCE",
				"TERRAINTYPE_NAME",
				modId,
				newTerrain.name
			);
			for (auto const& activateEffectRow : activateEffectResults.rows()) {
				assignTerrainActivationEffectData(activateEffectRow, newTerrain);
			}
		}
		return terrains;
	}

	std::vector<dTypes::CommanderType> get_commanders(int64_t modId, std::optional<std::string_view> const& commanderName) {
		sqlutil::Session session;
		std::vector<dTypes::CommanderType> commanders;
		auto results = getLookup(
			session.connection,
			"COMMANDER_TYPE",
			modId,
			commanderName
		);
		for (auto const& row : results.rows()) {
			auto& newCommander = commanders.emplace_back();
			assignCommanderData(row, newCommander);
			auto commanderEffectResults = getReferenceLookup(
				session.connection,
				"COMMANDER_EFFECT_REFERENCE",
				"COMMANDER_NAME",
				modId,
				newCommander.name
			);
			for (auto const& commanderEffectRow : commanderEffectResults.rows()) {
				assignCommanderEffectData(commanderEffectRow, newCommander);
			}
		}
		return commanders;
	}

	std::vector<dTypes::MovementClass> get_movements(int64_t modId, std::optional<std::string_view> const& movementName) {
		sqlutil::Session session;
		std::vector<dTypes::MovementClass> movements;
		auto results = getLookup(
			session.connection,
			"MOVEMENT_TYPE",
			modId,
			movementName
		);
		for (auto const& row : results.rows()) {
			auto& newMovement = movements.emplace_back();
			assignMovementData(row, newMovement);
			auto movementCostResults = getReferenceLookup(
				session.connection,
				"MOVEMENT_COST_REFERENCE",
				"MOVEMENT_NAME",
				modId,
				newMovement.name
			);
			for (auto const& movementCostRow : movementCostResults.rows()) {
				assignMovementCostData(movementCostRow, newMovement);
			}
		}
		return movements;
	}

	std::vector<dTypes::PlayerType> get_players(int64_t modId, std::optional<std::string_view> const& playerName) {
		sqlutil::Session session;
		std::vector<dTypes::PlayerType> players;
		auto results = getLookup(
			session.connection,
			"PLAYER_TYPE",
			modId,
			playerName
		);
		for (auto const& row : results.rows()) {
			auto& newPlayer = players.emplace_back();
			assignPlayerData(row, newPlayer);
			auto permittedPlayerSlotsResults = getReferenceLookup(
				session.connection,
				"PLAYER_PERMITTEDPLAYERSLOT_REFERENCE",
				"PLAYERTYPE_NAME",
				modId,
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
				modId,
				newPlayer.name
			);
			if (permittedCommanderResults.size() > 0) {
				newPlayer.permittedCommanderTypes.emplace();
				for (auto const& permittedCommanderRow : permittedCommanderResults.rows()) {
					assignPlayerCommanderTypeData(permittedCommanderRow, newPlayer);
				}
			}
		}
		return players;
	}

	std::vector<dTypes::PassiveUnitEffect> get_pues(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::PassiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_UNIT_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignPUEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "PUE");
			getEffectUnitRequired(session.connection, newEffect, modId, "PUE");
			getEffectClassifications(session.connection, newEffect, modId, "PUE");
			getEffectTerrainRequired(session.connection, newEffect, modId, "PUE");

			auto firepowerTerrainResults = getReferenceLookup(
				session.connection,
				"PUE_FIREPOWERFROMTERRAIN_REFERENCE",
				"EFFECT_NAME",
				modId,
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
				modId,
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
				modId,
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
				modId,
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
				modId,
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
				modId,
				newEffect.name
			);
			if (intelResults.size() > 0) {
				for (auto const& row : intelResults.rows()) {
					assignPUEIntelData(row, newEffect);
				}
			}
		}
		return effects;
	}

	std::vector<dTypes::ActiveUnitEffect> get_aues(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::ActiveUnitEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_UNIT_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignAUEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "AUE");
			getEffectUnitRequired(session.connection, newEffect, modId, "AUE");
			getEffectClassifications(session.connection, newEffect, modId, "AUE");
			getEffectTerrainRequired(session.connection, newEffect, modId, "AUE");
		}
		return effects;
	}

	std::vector<dTypes::PassiveTerrainEffect> get_ptes(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::PassiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_TERRAIN_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignPTEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "PTE");
			getEffectAffects(session.connection, newEffect, modId, "PTE");
			getEffectClassifications(session.connection, newEffect, modId, "PTE");
			getEffectTerrainRequired(session.connection, newEffect, modId, "PTE");

			auto buildListResults = getReferenceLookup(
				session.connection,
				"PTE_BUILDLISTMOD_REFERENCE",
				"EFFECT_NAME",
				modId,
				newEffect.name
			);
			if (buildListResults.size() > 0) {
				newEffect.buildListMod.emplace();
				for (auto const& sRow : buildListResults.rows()) {
					assignPTEBuildListData(sRow, newEffect);
				}
			}
		}
		return effects;
	}

	std::vector<dTypes::ActiveTerrainEffect> get_ates(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::ActiveTerrainEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_TERRAIN_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignATEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "ATE");
			getEffectAffects(session.connection, newEffect, modId, "ATE");
			getEffectTerrainRequired(session.connection, newEffect, modId, "ATE");
		}
		return effects;
	}

	std::vector<dTypes::PassiveGlobalEffect> get_pges(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::PassiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"PASSIVE_GLOBAL_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignPGEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "PGE");

			auto variantHintResults = getReferenceLookup(
				session.connection,
				"PGE_VARIANTHINTMOD_REFERENCE",
				"EFFECT_NAME",
				modId,
				newEffect.name
			);
			if (variantHintResults.size() > 0) {
				newEffect.variantHintMod.emplace();
				for (auto const& sRow : variantHintResults.rows()) {
					assignPGEVariantHintData(sRow, newEffect);
				}
			}
		}
		return effects;
	}

	std::vector<dTypes::ActiveGlobalEffect> get_ages(int64_t modId, std::optional<std::string_view> const& effectName) {
		sqlutil::Session session;
		std::vector<dTypes::ActiveGlobalEffect> effects;
		auto results = getLookup(
			session.connection,
			"ACTIVE_GLOBAL_EFFECT",
			modId,
			effectName
		);
		for (auto const& row : results.rows()) {
			auto& newEffect = effects.emplace_back();
			assignAGEData(row, newEffect);
			getEffectTargets(session.connection, newEffect, modId, "AGE");

			auto missileTargetResults = getReferenceLookup(
				session.connection,
				"AGE_MISSILETARGETMETHOD_REFERENCE",
				"EFFECT_NAME",
				modId,
				newEffect.name
			);
			if (missileTargetResults.size() > 0) {
				newEffect.missileTargetMethod.emplace();
				for (auto const& sRow : missileTargetResults.rows()) {
					assignAGEMissileTargetData(sRow, newEffect);
				}
			}
		}
		return effects;
	}

	std::vector<dTypes::Settings> get_settings(int64_t modId, std::optional<std::string_view> const& settingsName) {
		sqlutil::Session session;
		std::vector<dTypes::Settings> settings;
		auto results = getLookup(
			session.connection,
			"DEFAULT_GAME_SETTINGS",
			modId,
			settingsName
		);
		for (auto const& row : results.rows()) {
			auto& newSettings = settings.emplace_back();
			assignSettingsData(row, newSettings);

			auto settingsVariantResults = getReferenceLookup(
				session.connection,
				"DEFAULTSETTINGS_VARIANT_REFERENCE",
				"SETTING_NAME",
				modId,
				newSettings.name
			);
			if (settingsVariantResults.size() > 0) {
				newSettings.variant.emplace();
				for (auto const& sRow : settingsVariantResults.rows()) {
					assignSettingsVariantData(sRow, newSettings);
				}
			}
		}
		return settings;
	}

	dTypes::ModMetadata get_mod_metadata(std::optional<int64_t> const& modId, std::optional<std::string_view> const& name, std::optional<std::string_view> const& version) {
		sqlutil::Session session;
		std::string sql = R"SQL(
			select 
				*
			from 
				DATA.MOD
			where 
				1 = 1
		)SQL";
		ParameterPack parameters;
		if (modId) {
			parameters.emplace_back(*modId);
			sql += R"SQL(
				and ID = ?
			)SQL";
		}
		else {
			if (name) {
				parameters.emplace_back(*name);
				sql += R"SQL(
				and NAME = ?
				)SQL";

				if (version) {
					parameters.emplace_back(*version);
					sql += R"SQL(
						and VERSION = ?
					)SQL";
				}
				else {
					sql += R"SQL(
						and EXPIRED is null
					)SQL";
				}
			}
			else {
				throw net::RestError("No ModId or Name provided in request", net::RestError::Type::BAD_REQUEST);
			}
		}
		sql += R"SQL(
			order by ID desc
		)SQL";
		auto statement = session.connection.prepare_statement(sql);
		mysql::results results;
		session.connection.execute(
			statement.bind(parameters.begin(), parameters.end())
			, 
			results);
		if (results.rows().size() > 0) {
			dTypes::ModMetadata metadata;
			auto row = results.rows().at(0);
			metadata.modId = std::to_string(row.at(0).as_int64());
			metadata.name = row.at(1).as_string();
			metadata.version = row.at(2).as_string();
			metadata.expired = !row.at(3).is_null();
			return metadata;
		}
		else {
			throw net::RestError("No Mod Found", net::RestError::Type::INVALID_DATA);
		}
	}
}
