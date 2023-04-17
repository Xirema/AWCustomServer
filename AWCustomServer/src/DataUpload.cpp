#include<RestFunctions.h>
#include<boost/json.hpp>
#include<SQLUtil.h>
#include<DataTypes.h>

namespace {
	const std::string secret_key = "a8f7af7d81269941f6d78eab35632eb0c603bd3ece95f8df7ab4a41ad187b06a";
	namespace mysql = boost::mysql;
	using ParameterPack = std::vector<mysql::field>;

	std::vector<mysql::field_view> asView(ParameterPack const& parameters) {
		std::vector<mysql::field_view> ret;
		for (auto const& field : parameters) {
			ret.emplace_back(field);
		}
		return ret;
	}
	uint64_t submitMetaData(boost::json::object const& obj, mysql::tcp_ssl_connection& connection) {
		datatypes::ModMetadata metadata;
		metadata.readFrom(obj);
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
		connection.execute_statement(statement, std::make_tuple(metadata.name), results);

		statement = connection.prepare_statement(R"SQL(
			insert into 
				DATA.MOD
				(NAME, VERSION)
			values
				(?, ?)
		)SQL");
		results = {};
		connection.execute_statement(statement, std::make_tuple(metadata.name, metadata.version), results);
		return results.last_insert_id();
	}

	template<typename T>
	void sbind(T const& t, ParameterPack& parameters) {
		if constexpr (std::is_same_v<bool, T>) {
			if (t) {
				parameters.emplace_back(1);
			}
			else {
				parameters.emplace_back(0);
			}
		}
		else {
			parameters.emplace_back(t);
		}
	}

	template<typename T>
	void sbind(std::optional<T> const& opt, ParameterPack & parameters) {
		if (opt) {
			sbind(*opt, parameters);
		}
		else
			parameters.emplace_back(nullptr);
	}

	template<typename ... T>
	void bindAll(ParameterPack & parameters, T... vals) {
		(sbind(vals, parameters), ...);
	}

	void submitUnitData(boost::json::array const& unitData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& unit : unitData) {
			datatypes::UnitType unitType;
			unitType.readFrom(unit.as_object());

			auto unitInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.UNIT_TYPE
					(
						MOD_ID, NAME, COST, MAXFUEL, MAXAMMO, VISIONRANGE, MOVEMENTCLASS, MOVEMENTRANGE, FUELPERDAY, FUELPERDAYSTEALTH,
						SUPPLYREPAIR, TRANSPORTCAPACITY, HITPOINTS, CAPTURESPEED, IGNORESVISIONOCCLUSION, STEALTHTYPE
					)
				values 
					(
						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
					)
			)SQL");
			ParameterPack parameters;
			bindAll(parameters, 
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
				unitType.stealthType
			);

			mysql::results results;
			mysql::execution_state state;

			auto parametersView = asView(parameters);

			connection.start_statement_execution(unitInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (unitType.weapons) {
				int order = 0;
				for (auto const& weapon : *unitType.weapons) {
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
					mysql::results results;
					connection.execute_statement(unitWeaponInsertStatement, std::make_tuple(mod_id, unitType.name, weapon, order), results);
					order++;
				}
			}

			if (unitType.transportList) {
				for (auto const& transportableUnit : *unitType.transportList) {
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
					mysql::results results;
					connection.execute_statement(unitTransportListInsertStatement, std::make_tuple(mod_id, unitType.name, transportableUnit), results);
				}
			}

			for (auto const& classification : unitType.classifications) {
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
				mysql::results results;
				connection.execute_statement(unitClassificationInsertStatement, std::make_tuple(mod_id, unitType.name, classification), results);
			}
		}
	}

	void submitWeaponData(boost::json::array const& weaponData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& weapon : weaponData) {
			datatypes::WeaponType weaponType;
			weaponType.readFrom(weapon.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
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
			);
			mysql::execution_state state;

			auto parametersView = asView(parameters);

			connection.start_statement_execution(weaponInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (weaponType.baseDamage) {
				for (auto const& entry : *weaponType.baseDamage) {
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
					mysql::results results;
					connection.execute_statement(weaponBaseDamageInsertStatement, std::make_tuple(mod_id, weaponType.name, entry.first, entry.second), results);
				}
			}

			if (weaponType.targetsStealth) {
				for (auto const& targetsStealthValue : *weaponType.targetsStealth) {
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
					mysql::results results;
					connection.execute_statement(weaponTargetsStealthInsertStatement, std::make_tuple(mod_id, weaponType.name, targetsStealthValue), results);
				}
			}
		}
	}

	void submitTerrainData(boost::json::array const& terrainData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& terrain : terrainData) {
			datatypes::TerrainType terrainType;
			terrainType.readFrom(terrain.as_object());

			auto terrainInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.TERRAIN_TYPE
					(
						MOD_ID, NAME, STARS, MAXCAPTUREPOINTS, SAMEAS, INCOME, REPAIR, OCCLUDESVISION, HITPOINTS, DESTROYED, 
						DAMAGEDLIKE, ACTIVATIONMAX, ACTIVATIONCHANGE, LOSEIFCAPTURED, LOSEIFALLCAPTURED
					)
				values
					(
						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?
					)
			)SQL");
			ParameterPack parameters;
			bindAll(
				parameters,
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
				terrainType.loseIfAllCaptured
			);
			mysql::execution_state state;

			auto parametersView = asView(parameters);

			connection.start_statement_execution(terrainInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (terrainType.buildList) {
				int order = 0;
				for (auto const& buildUnit : *terrainType.buildList) {
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
					mysql::results results;
					connection.execute_statement(terrainBuildInsertStatement, std::make_tuple(mod_id, terrainType.name, buildUnit, order), results);
					order++;
				}
			}

			if (terrainType.repairList) {
				for (auto const& repairUnit : *terrainType.repairList) {
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
					mysql::results results;
					connection.execute_statement(terrainRepairInsertStatement, std::make_tuple(mod_id, terrainType.name, repairUnit), results);
				}
			}

			if (terrainType.activateList) {
				for (auto const& activateUnit : *terrainType.activateList) {
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
					mysql::results results;
					connection.execute_statement(terrainActivateListInsertStatement, std::make_tuple(mod_id, terrainType.name, activateUnit), results);
				}
			}

			auto bindEffects = [&connection, &mod_id, &terrainType](std::vector<std::string> const& effectList, std::string_view effectType) {
				for (auto const& activateEffect : effectList) {
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
					mysql::results results;
					connection.execute_statement(terrainActivateEffectInsertStatement, std::make_tuple(mod_id, terrainType.name, effectType, activateEffect), results);
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

	void submitMovementData(boost::json::array const& movementData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& movement : movementData) {
			datatypes::MovementClass movementClass;
			movementClass.readFrom(movement.as_object());

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
			mysql::results results;
			connection.execute_statement(movementInsertStatement, std::make_tuple(mod_id, movementClass.name), results);

			for (auto const& entry : movementClass.movementCosts) {
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
				mysql::results results;
				connection.execute_statement(movementCostInsertStatement, std::make_tuple(mod_id, movementClass.name, entry.first, entry.second), results);
			}

			if (movementClass.variantMods) {
				for (auto const& [variantName, map] : *movementClass.variantMods) {
					for (auto const& [terrainName, costMod] : map) {

						auto movementCostInsertStatement = connection.prepare_statement(R"SQL(
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
						mysql::results results;
						connection.execute_statement(movementCostInsertStatement, std::make_tuple(mod_id, movementClass.name, terrainName, costMod, variantName), results);
					}
				}
			}
		}
	}

	void submitCommanderData(boost::json::array const& commanderData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& commander : commanderData) {
			datatypes::CommanderType commanderType;
			commanderType.readFrom(commander.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				commanderType.name,
				commanderType.copCost,
				commanderType.scopCost,
				commanderType.coMeterMultiplier,
				commanderType.playable
			);
			mysql::execution_state state;

			auto parametersView = asView(parameters);

			connection.start_statement_execution(commanderInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			auto insertEffects = [&connection, &commanderType, mod_id](std::vector<std::string> const& effects, std::string_view effectType, int effectCategory) {
				for (auto const& effect : effects) {
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
					mysql::results results;
					connection.execute_statement(effectInsertStatement, std::make_tuple(mod_id, commanderType.name, effect, effectType, effectCategory), results);
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

	void submitPlayerData(boost::json::array const& playerData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& player : playerData) {
			datatypes::PlayerType playerType;
			playerType.readFrom(player.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				playerType.name,
				playerType.commanderTypeMod,
				playerType.teamName
			);
			mysql::execution_state state;

			auto parametersView = asView(parameters);

			connection.start_statement_execution(playerInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (playerType.permittedPlayerSlots) {
				for (auto const& playerSlot : *playerType.permittedPlayerSlots) {
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
					mysql::results results;
					connection.execute_statement(playerSlotInsertStatement, std::make_tuple(mod_id, playerType.name, playerSlot), results);
				}
			}

			if (playerType.permittedCommanderTypes) {
				for (auto const& commanderType : *playerType.permittedCommanderTypes) {
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
					mysql::results results;
					connection.execute_statement(commanderTypeInsertStatement, std::make_tuple(mod_id, playerType.name, commanderType), results);
				}
			}
		}
	}

	void insertEffectTargets(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& targetArray) {
		for (auto const& target : targetArray) {
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
			mysql::results results;
			connection.execute_statement(effectTargetInsertStatement, std::make_tuple(mod_id, effectName, effectType, target), results);
		}
	}
	void insertEffectAffects(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& affectArray) {
		for (auto const& affect : affectArray) {
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
			mysql::results results;
			connection.execute_statement(effectTargetInsertStatement, std::make_tuple(mod_id, effectName, effectType, affect), results);
		}
	}

	void insertEffectUnitTypeRequired(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& unitTypeArray) {
		for (auto const& unitType : unitTypeArray) {
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
			mysql::results results;
			connection.execute_statement(effectUnitTypeInsertStatement, std::make_tuple(mod_id, effectName, effectType, unitType), results);
		}
	}

	void insertEffectClassification(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& classificationArray) {
		for (auto const& classification : classificationArray) {
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
			mysql::results results;
			connection.execute_statement(insertStatement, std::make_tuple(mod_id, effectName, effectType, classification), results);
		}
	}

	void insertEffectTerrainRequired(mysql::tcp_ssl_connection & connection, uint64_t mod_id, std::string_view effectName, std::string_view effectType, std::vector<std::string> const& terrainArray) {
		for (auto const& terrain : terrainArray) {
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
			mysql::results results;
			connection.execute_statement(insertStatement, std::make_tuple(mod_id, effectName, effectType, terrain), results);
		}
	}

	void submitPassiveUnitEffectData(boost::json::array const& passiveUnitEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& passiveUnitEffect : passiveUnitEffectData) {
			datatypes::PassiveUnitEffect pue;
			pue.readFrom(passiveUnitEffect.as_object());

			auto passiveUnitEffectInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.PASSIVE_UNIT_EFFECT
					(
						MOD_ID, NAME, FIREPOWERMOD, DEFENSEMOD, INDIRECTDEFENSEMOD, MINRANGEMOD, MAXRANGEMOD, FUELUSEMOD, AMMOUSEMOD, GOODLUCKMOD, BADLUCKMOD,
						MOVEMENTMOD, VISIONMOD, TERRAINSTARSMOD, TERRAINSTARSFLATMOD, TERRAINSTARSDEFENSE, TERRAINSTARSFIREPOWER, COUNTERFIREMOD, COUNTERFIRST,
						CAPTURERATEMOD, UNITCOSTMOD, HIDDENHITPOINTS, FIREPOWERFROMFUNDS, DEFENSEFROMFUNDS, FUNDSFROMDAMAGE, COMETERCHARGEFROMDEALTDAMAGE,
						COMETERCHARGEFROMRECEIVEDDAMAGE
					)
				VALUES
					(
						?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, 
						?, ?, ?, ?, ?, ?, ?, ?,
						?, ?, ?, ?, ?, ?, ?,
						?
					)
			)SQL");
			ParameterPack parameters;
			bindAll(
				parameters,
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
				pue.hiddenHitPoints,
				pue.firepowerFromFunds,
				pue.defenseFromFunds,
				pue.fundsFromDamage,
				pue.coMeterChargeFromDealtDamage,

				pue.coMeterChargeFromReceivedDamage
			);
			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(passiveUnitEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

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
					auto insertStatement = connection.prepare_statement(R"SQL(
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
					mysql::results results;
					connection.execute_statement(insertStatement, std::make_tuple(mod_id, pue.name, terrainName, firepowerMod), results);
				}
			}
			if (pue.defenseFromOwnedTerrain) {
				for (auto const& [terrainName, defenseMod] : *pue.defenseFromOwnedTerrain) {

					auto insertStatement = connection.prepare_statement(R"SQL(
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
					mysql::results results;
					connection.execute_statement(insertStatement, std::make_tuple(mod_id, pue.name, terrainName, defenseMod), results);
				}
			}
			if (pue.visionVariantMods) {
				for (auto const& [variantName, visionMod] : *pue.visionVariantMods) {
					auto insertStatement = connection.prepare_statement(R"SQL(
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
					mysql::results results;
					connection.execute_statement(insertStatement, std::make_tuple(mod_id, pue.name, variantName, visionMod), results);
				}
			}
			if (pue.firepowerVariantMods) {
				for (auto const& [variantName, firepowerMod] : *pue.firepowerVariantMods) {
					auto insertStatement = connection.prepare_statement(R"SQL(
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
					mysql::results results;
					connection.execute_statement(insertStatement, std::make_tuple(mod_id, pue.name, variantName, firepowerMod), results);
				}
			}
			if (pue.defenseVariantMods) {
				for (auto const& [variantName, defenseMod] : *pue.defenseVariantMods) {
					auto insertStatement = connection.prepare_statement(R"SQL(
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
					mysql::results results;
					connection.execute_statement(insertStatement, std::make_tuple(mod_id, pue.name, variantName, defenseMod), results);
				}
			}
		}
	}

	void submitActiveUnitEffectData(boost::json::array const& activeUnitEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& activeUnitEffect : activeUnitEffectData) {
			datatypes::ActiveUnitEffect aue;
			aue.readFrom(activeUnitEffect.as_object());

			auto activeUnitEffectInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.ACTIVE_UNIT_EFFECT
					(
						MOD_ID, NAME, HITPOINTMOD, ROUNDHITPOINTS, RESUPPLY, HALVEFUEL, MAKEACTIVE, STUNDURATION
					)
				values
					(
						?, ?, ?, ?, ?, ?, ?, ?
					)
			)SQL");
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				aue.name,
				aue.hitPointMod,
				aue.roundHitPoints,
				aue.resupply,
				aue.halveFuel,
				aue.makeActive,
				aue.stunDuration
			);

			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(activeUnitEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

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

	void submitPassiveTerrainEffectData(boost::json::array const& passiveTerrainEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& passiveTerrainEffect : passiveTerrainEffectData) {
			datatypes::PassiveTerrainEffect pte;
			pte.readFrom(passiveTerrainEffect.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				pte.name,
				pte.incomeMod,
				pte.incomeFlatMod,
				pte.repairMod,
				pte.occludesVisionMod,
				pte.visionModBoost,
				pte.buildCostMod
			);

			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(passiveTerrainEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

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

					mysql::results results;
					connection.execute_statement(pteBuildListInsertStatement, std::make_tuple(mod_id, pte.name, buildListItem), results);
				}
			}
		}
	}

	void submitActiveTerrainEffectData(boost::json::array const& activeTerrainEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for(auto const& activeTerrainEffect : activeTerrainEffectData) {
			datatypes::ActiveTerrainEffect ate;
			ate.readFrom(activeTerrainEffect.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				ate.name,
				ate.unitSummonedName,
				ate.unitSummonedInitialDamage,
				ate.unitSummonedActive
			);

			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(activeTerrainEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

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

	void submitPassiveGlobalEffectData(boost::json::array const& passiveGlobalEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& passiveGlobalEffect : passiveGlobalEffectData) {
			datatypes::PassiveGlobalEffect pge;
			pge.readFrom(passiveGlobalEffect.as_object());

			auto passiveGlobalEffectInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.PASSIVE_GLOBAL_EFFECT
					(
						MOD_ID, NAME, VARIANTMOD, MOVEMENTCLASSVARIANTREPLACE, MOVEMENTCLASSVARIANTOVERRIDE
					)
				values
					(
						?, ?, ?, ?, ?
					)
			)SQL");
			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				pge.name,
				pge.variantMod,
				pge.movementClassVariantReplace,
				pge.movementClassVariantOverride
			);

			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(passiveGlobalEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (pge.targets) {
				insertEffectTargets(connection, mod_id, pge.name, "PGE", *pge.targets);
			}

			if (pge.variantHintMod) {
				for (auto const& [variantName, hint] : *pge.variantHintMod) {
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
					mysql::results results;
					connection.execute_statement(variantHintModInsertStatement, std::make_tuple(mod_id, pge.name, variantName, hint), results);
				}
			}
		}
	}

	void submitActiveGlobalEffectData(boost::json::array const& activeGlobalEffectData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& activeGlobalEffect : activeGlobalEffectData) {
			datatypes::ActiveGlobalEffect age;
			age.readFrom(activeGlobalEffect.as_object());

			auto activeGlobalEffectInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.ACTIVE_GLOBAL_EFFECT
					(
						MOD_ID, NAME, FUNDMOD, FUNDFLATMOD, POWERBARMOD, POWERBARPERFUNDS, MISSILECOUNT, MISSILEDAMAGE, MISSILEAREAOFEFFECT, MISSILESTUNDURATION
					)
				values
					(
						?, ?, ?, ?, ?, ?, ?, ?, ?, ?
					)
			)SQL");

			ParameterPack parameters;
			bindAll(
				parameters,
				mod_id,
				age.name,
				age.fundMod,
				age.fundFlatMod,
				age.powerBarMod,
				age.powerBarPerFunds,
				age.missileCount,
				age.missileDamage,
				age.missileAreaOfEffect,
				age.missileStunDuration
			);

			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(activeGlobalEffectInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (age.targets) {
				insertEffectTargets(connection, mod_id, age.name, "AGE", *age.targets);
			}

			if (age.missileTargetMethod) {
				for (auto const& targetMethod : *age.missileTargetMethod) {
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
					mysql::results results;
					connection.execute_statement(targetMethodInsertStatement, std::make_tuple(mod_id, age.name, targetMethod), results);
				}
			}
		}
	}

	void submitDefaultSettingsData(boost::json::array const& defaultSettingsData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& defaultSettings : defaultSettingsData) {
			datatypes::Settings settings;
			settings.readFrom(defaultSettings.as_object());

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
			ParameterPack parameters;
			bindAll(
				parameters,
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
			);


			mysql::execution_state state;
			auto parametersView = asView(parameters);

			connection.start_statement_execution(defaultSettingsInsertStatement, parametersView.begin(), parametersView.end(), state);
			while (!state.complete()) {
				connection.read_some_rows(state);
			}

			if (settings.variant) {
				for (auto const& [variantName, weight] : *settings.variant) {
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
					mysql::results results;
					connection.execute_statement(settingsVariantInsertStatement, std::make_tuple(mod_id, settings.name, variantName, weight), results);
				}
			}
		}
	}

	void submitTextResourcesData(boost::json::array const& textResourcesData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& textResource : textResourcesData) {
			datatypes::TextResource resource;
			resource.readFrom(textResource.as_object());

			auto textResourceInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.TEXT_RESOURCE
					(
						MOD_ID, `KEY`, `TYPE`, SHORTNAME, LONGNAME, DESCRIPTION, LANGUAGE, `ORDER`
					)
				values
				(
					?, ?, ?, ?, ?, ?, ?, ?
				)
			)SQL");
			size_t i = 0;
			do {
				ParameterPack parameters;
				bindAll(
					parameters,
					mod_id,
					resource.key,
					resource.type,
					resource.shortName,
					resource.longName
				);
				std::string descPart{
					resource.description.begin() + i,
					(i + 512 <= resource.description.size()) ? 
						(resource.description.begin() + i + 512) :
						(resource.description.end())
				};
				sbind(descPart, parameters);
				sbind(resource.language, parameters);
				sbind(i / 512, parameters);

				mysql::execution_state state;
				auto parametersView = asView(parameters);

				connection.start_statement_execution(textResourceInsertStatement, parametersView.begin(), parametersView.end(), state);
				while (!state.complete()) {
					connection.read_some_rows(state);
				}
				i += 512;
			} while (i < resource.description.size());
		}
	}

	void submitImageResourcesData(boost::json::array const& imageResourcesData, mysql::tcp_ssl_connection & connection, uint64_t mod_id) {
		for (auto const& imageResource : imageResourcesData) {
			datatypes::ImageResource resource;
			resource.readFrom(imageResource.as_object());

			auto imageResourceInsertStatement = connection.prepare_statement(R"SQL(
				insert into
					DATA.IMAGE_RESOURCE
					(
						MOD_ID, `KEY`, `TYPE`, SMALLIMAGE, LARGEIMAGE, ARMYCOLOR, `ORDER`, ORIENTATION, VARIANT
					)
				values
				(
					?, ?, ?, ?, ?, ?, ?, ?, ?
				)
			)SQL");
			size_t i = 0;
			constexpr size_t BLOCK_SIZE = 512;
			do {
				ParameterPack parameters;
				bindAll(
					parameters,
					mod_id,
					resource.key,
					resource.type
				);
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

				sbind(smallPart, parameters);
				sbind(largePart, parameters);
				sbind(resource.armyColor, parameters);
				sbind(i / BLOCK_SIZE, parameters);
				sbind(resource.orientation, parameters);
				sbind(resource.variant, parameters);

				mysql::execution_state state;
				auto parametersView = asView(parameters);

				connection.start_statement_execution(imageResourceInsertStatement, parametersView.begin(), parametersView.end(), state);
				while (!state.complete()) {
					connection.read_some_rows(state);
				}
				i += BLOCK_SIZE;
			} while (i < std::max(resource.smallImage.size(), resource.largeImage.size()));
		}
	}

	void extractModData(boost::json::value value) {
		sqlutil::Session session;
		try {
			mysql::results results;
			session.connection.query("START TRANSACTION", results);

			auto const& obj = value;

			auto const& metadata = obj.at("modMetaData");
			auto mod_id = submitMetaData(metadata.as_object(), session.connection);
			auto const& unitData = obj.at("units");
			submitUnitData(unitData.as_array(), session.connection, mod_id);
			auto const& weaponData = obj.at("weapons");
			submitWeaponData(weaponData.as_array(), session.connection, mod_id);
			auto const& terrainData = obj.at("terrains");
			submitTerrainData(terrainData.as_array(), session.connection, mod_id);
			auto const& movementData = obj.at("movementClasses");
			submitMovementData(movementData.as_array(), session.connection, mod_id);
			auto const& commanderData = obj.at("commanders");
			submitCommanderData(commanderData.as_array(), session.connection, mod_id);
			auto const& playerData = obj.at("playerTypes");
			submitPlayerData(playerData.as_array(), session.connection, mod_id);
			auto const& passiveUnitEffectData = obj.at("passiveUnitEffects");
			submitPassiveUnitEffectData(passiveUnitEffectData.as_array(), session.connection, mod_id);
			auto const& activeUnitEffectData = obj.at("activeUnitEffects");
			submitActiveUnitEffectData(activeUnitEffectData.as_array(), session.connection, mod_id);
			auto const& passiveTerrainEffectData = obj.at("passiveTerrainEffects");
			submitPassiveTerrainEffectData(passiveTerrainEffectData.as_array(), session.connection, mod_id);
			auto const& activeTerrainEffectData = obj.at("activeTerrainEffects");
			submitActiveTerrainEffectData(activeTerrainEffectData.as_array(), session.connection, mod_id);
			auto const& passiveGlobalEffectData = obj.at("passiveGlobalEffects");
			submitPassiveGlobalEffectData(passiveGlobalEffectData.as_array(), session.connection, mod_id);
			auto const& activeGlobalEffectData = obj.at("activeGlobalEffects");
			submitActiveGlobalEffectData(activeGlobalEffectData.as_array(), session.connection, mod_id);

			auto const& defaultSettingsData = obj.at("defaultSettings");
			submitDefaultSettingsData(defaultSettingsData.as_array(), session.connection, mod_id);
			auto const& textResourcesData = obj.at("textResources");
			submitTextResourcesData(textResourcesData.as_array(), session.connection, mod_id);
			auto const& imageResourcesData = obj.at("imageResources");
			submitImageResourcesData(imageResourcesData.as_array(), session.connection, mod_id);

			session.connection.query("COMMIT", results);
		}
		catch (boost::system::system_error const& e) {
			mysql::results results;
			session.connection.query("ROLLBACK", results);
			throw e;
		}
		catch (std::invalid_argument const& e) {
			mysql::results results;
			session.connection.query("ROLLBACK", results);
			throw e;
		}
		catch (std::out_of_range const& e) {
			mysql::results results;
			session.connection.query("ROLLBACK", results);
			throw e;
		}
		catch (...) {
			mysql::results results;
			session.connection.query("ROLLBACK", results);
			std::rethrow_exception(std::current_exception());
		}
	}
}

std::string rest::data::upload_mod(net::HTTPHeaders const& headers, std::string body) {
	auto it = headers.httpHeaders.find("cookies");
	if (it == headers.httpHeaders.end()) {
		throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
	}
	if (auto const& value = it->second; value.find(secret_key) == std::string::npos) {
		throw net::RestError("This Request is not Authorized", net::RestError::Type::INVALID_DATA);
	}
	try {
		extractModData(boost::json::parse(body));
		return "success";
	}
	catch (boost::system::system_error const& e) {
		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
	}
	catch (std::invalid_argument const& e) {
		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
	}
	catch (std::out_of_range const& e) {
		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
	}
	catch (std::exception const& e) {
		throw net::RestError("There was a problem parsing the Mod Data: " + std::string(e.what()), net::RestError::Type::INVALID_DATA);
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