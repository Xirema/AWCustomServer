#include<RestFunctions.h>
#include<boost/json.hpp>
#include<SQLUtil.h>
#include<jss/ModMetadata.h>
#include<jss/Commander.h>
#include<jss/Effect.h>
#include<jss/Movement.h>
#include<jss/Settings.h>
#include<jss/Terrain.h>
#include<jss/Unit.h>

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

	void submitUnitData(std::vector<dTypes::UnitType> const& unitData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitWeaponData(std::vector<dTypes::WeaponType> const& weaponData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitTerrainData(std::vector<dTypes::TerrainType> const& terrainData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitMovementData(std::vector<dTypes::MovementClass> const& movementData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitCommanderData(std::vector<dTypes::CommanderType> const& commanderData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitPlayerData(std::vector<dTypes::PlayerType> const& playerData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void insertEffectTargets(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& targetArray) {
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
	void insertEffectAffects(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& affectArray) {
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

	void insertEffectUnitTypeRequired(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& unitTypeArray) {
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

	void insertEffectClassification(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& classificationArray) {
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

	void insertEffectTerrainRequired(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& terrainArray) {
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

	void submitPassiveUnitEffectData(std::vector<dTypes::PassiveUnitEffect> const& passiveUnitEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitActiveUnitEffectData(std::vector<dTypes::ActiveUnitEffect> const& activeUnitEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitPassiveTerrainEffectData(std::vector<dTypes::PassiveTerrainEffect> const& passiveTerrainEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitActiveTerrainEffectData(std::vector<dTypes::ActiveTerrainEffect> const& activeTerrainEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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
		for(auto const& ate : activeTerrainEffectData) {
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

	void submitPassiveGlobalEffectData(std::vector<dTypes::PassiveGlobalEffect> const& passiveGlobalEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitActiveGlobalEffectData(std::vector<dTypes::ActiveGlobalEffect> const& activeGlobalEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void submitDefaultSettingsData(std::vector<dTypes::Settings> const& defaultSettingsData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
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

	void extractModData(boost::json::value value) {
		auto const& obj = value.as_object();
		if (auto protocolPtr = obj.if_contains("protocol")) {
			if (auto protocolStringPtr = protocolPtr->if_string()) {
				if (*protocolStringPtr != supportedProtocol) {
					throw net::RestError("Protocol '" + std::string(*protocolStringPtr) + "' is not supported by this version of AWCustom.", net::RestError::Type::INVALID_DATA);
				}
			}
			else {
				throw net::RestError("'protocol' field is improperly specified; expected type 'string'", net::RestError::Type::INVALID_DATA);
			}
		}
		dTypes::ModData modData;
		modData.readFrom(obj);

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
	}
}

std::string rest::data::upload_mod(net::HTTPHeaders const& headers, std::string body) {
	//auto it = headers.httpHeaders.find("cookies");
	//if (it == headers.httpHeaders.end()) {
	//	throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
	//}
	//if (auto const& value = it->second; value.find(secret_key) == std::string::npos) {
	//	throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
	//}
	try {
		extractModData(boost::json::parse(body));
		return "success";
	}
	catch (std::runtime_error const& e) {
		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
	}
}
