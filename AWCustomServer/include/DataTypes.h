#pragma once
#include<boost/json.hpp>
#include<string>
#include<optional>
#include<map>

namespace datatypes {
	namespace json = boost::json;
	struct CommanderType {
		std::string name;
		std::optional<std::vector<std::string>> passiveUnitEffectsD2d;
		std::optional<std::vector<std::string>> passiveTerrainEffectsD2d;
		std::optional<std::vector<std::string>> passiveGlobalEffectsD2d;
		std::optional<int64_t> copCost;
		std::optional<std::vector<std::string>> passiveUnitEffectsCop;
		std::optional<std::vector<std::string>> activeUnitEffectsCop;
		std::optional<std::vector<std::string>> passiveTerrainEffectsCop;
		std::optional<std::vector<std::string>> activeTerrainEffectsCop;
		std::optional<std::vector<std::string>> passiveGlobalEffectsCop;
		std::optional<std::vector<std::string>> activeGlobalEffectsCop;
		std::optional<int64_t> scopCost;
		std::optional<std::vector<std::string>> passiveUnitEffectsScop;
		std::optional<std::vector<std::string>> activeUnitEffectsScop;
		std::optional<std::vector<std::string>> passiveTerrainEffectsScop;
		std::optional<std::vector<std::string>> activeTerrainEffectsScop;
		std::optional<std::vector<std::string>> passiveGlobalEffectsScop;
		std::optional<std::vector<std::string>> activeGlobalEffectsScop;
		std::optional<int64_t> coMeterMultiplier;
		std::optional<bool> playable;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in CommanderType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in CommanderType");
			}
			if (auto ptr = obj.if_contains("passiveUnitEffectsD2d")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveUnitEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveUnitEffectsD2d = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveUnitEffectsD2d' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveTerrainEffectsD2d")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveTerrainEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveTerrainEffectsD2d = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveTerrainEffectsD2d' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveGlobalEffectsD2d")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveGlobalEffectsD2d' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveGlobalEffectsD2d = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveGlobalEffectsD2d' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("copCost")) {
				if (auto tPtr = ptr->if_int64()) {
					copCost = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'copCost' as int64_t in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveUnitEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveUnitEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveUnitEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveUnitEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeUnitEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeUnitEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeUnitEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeUnitEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveTerrainEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveTerrainEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveTerrainEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveTerrainEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeTerrainEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeTerrainEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeTerrainEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeTerrainEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveGlobalEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveGlobalEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveGlobalEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveGlobalEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeGlobalEffectsCop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeGlobalEffectsCop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeGlobalEffectsCop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeGlobalEffectsCop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("scopCost")) {
				if (auto tPtr = ptr->if_int64()) {
					scopCost = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'scopCost' as int64_t in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveUnitEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveUnitEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveUnitEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveUnitEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeUnitEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeUnitEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeUnitEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeUnitEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveTerrainEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveTerrainEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveTerrainEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveTerrainEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeTerrainEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeTerrainEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeTerrainEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeTerrainEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("passiveGlobalEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'passiveGlobalEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					passiveGlobalEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'passiveGlobalEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activeGlobalEffectsScop")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activeGlobalEffectsScop' in CommanderType was expected to be string, but was of wrong type.");
						}
					}
					activeGlobalEffectsScop = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activeGlobalEffectsScop' as array in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coMeterMultiplier")) {
				if (auto tPtr = ptr->if_int64()) {
					coMeterMultiplier = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in CommanderType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("playable")) {
				if (auto tPtr = ptr->if_bool()) {
					playable = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'playable' as bool in CommanderType, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (passiveUnitEffectsD2d) {
				json::array arr;
				for (auto const& val : *passiveUnitEffectsD2d) {
					arr.push_back(json::string{ val });
				}
				obj["passiveUnitEffectsD2d"] = std::move(arr);
			}
			if (passiveTerrainEffectsD2d) {
				json::array arr;
				for (auto const& val : *passiveTerrainEffectsD2d) {
					arr.push_back(json::string{ val });
				}
				obj["passiveTerrainEffectsD2d"] = std::move(arr);
			}
			if (passiveGlobalEffectsD2d) {
				json::array arr;
				for (auto const& val : *passiveGlobalEffectsD2d) {
					arr.push_back(json::string{ val });
				}
				obj["passiveGlobalEffectsD2d"] = std::move(arr);
			}
			if (copCost) {
				obj["copCost"] = *copCost;
			}
			if (passiveUnitEffectsCop) {
				json::array arr;
				for (auto const& val : *passiveUnitEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveUnitEffectsCop"] = std::move(arr);
			}
			if (activeUnitEffectsCop) {
				json::array arr;
				for (auto const& val : *activeUnitEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["activeUnitEffectsCop"] = std::move(arr);
			}
			if (passiveTerrainEffectsCop) {
				json::array arr;
				for (auto const& val : *passiveTerrainEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveTerrainEffectsCop"] = std::move(arr);
			}
			if (activeTerrainEffectsCop) {
				json::array arr;
				for (auto const& val : *activeTerrainEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["activeTerrainEffectsCop"] = std::move(arr);
			}
			if (passiveGlobalEffectsCop) {
				json::array arr;
				for (auto const& val : *passiveGlobalEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveGlobalEffectsCop"] = std::move(arr);
			}
			if (activeGlobalEffectsCop) {
				json::array arr;
				for (auto const& val : *activeGlobalEffectsCop) {
					arr.push_back(json::string{ val });
				}
				obj["activeGlobalEffectsCop"] = std::move(arr);
			}
			if (scopCost) {
				obj["scopCost"] = *scopCost;
			}
			if (passiveUnitEffectsScop) {
				json::array arr;
				for (auto const& val : *passiveUnitEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveUnitEffectsScop"] = std::move(arr);
			}
			if (activeUnitEffectsScop) {
				json::array arr;
				for (auto const& val : *activeUnitEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["activeUnitEffectsScop"] = std::move(arr);
			}
			if (passiveTerrainEffectsScop) {
				json::array arr;
				for (auto const& val : *passiveTerrainEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveTerrainEffectsScop"] = std::move(arr);
			}
			if (activeTerrainEffectsScop) {
				json::array arr;
				for (auto const& val : *activeTerrainEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["activeTerrainEffectsScop"] = std::move(arr);
			}
			if (passiveGlobalEffectsScop) {
				json::array arr;
				for (auto const& val : *passiveGlobalEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["passiveGlobalEffectsScop"] = std::move(arr);
			}
			if (activeGlobalEffectsScop) {
				json::array arr;
				for (auto const& val : *activeGlobalEffectsScop) {
					arr.push_back(json::string{ val });
				}
				obj["activeGlobalEffectsScop"] = std::move(arr);
			}
			if (coMeterMultiplier) {
				obj["coMeterMultiplier"] = *coMeterMultiplier;
			}
			if (playable) {
				obj["playable"] = *playable;
			}
		}
	};
	struct PlayerType {
		std::string name;
		std::optional<std::string> commanderTypeMod;
		std::optional<std::vector<std::string>> permittedPlayerSlots;
		std::optional<std::vector<std::string>> permittedCommanderTypes;
		std::optional<std::string> teamName;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in PlayerType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in PlayerType");
			}
			if (auto ptr = obj.if_contains("commanderTypeMod")) {
				if (auto tPtr = ptr->if_string()) {
					commanderTypeMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'commanderTypeMod' as std::string in PlayerType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("permittedPlayerSlots")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'permittedPlayerSlots' in PlayerType was expected to be string, but was of wrong type.");
						}
					}
					permittedPlayerSlots = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'permittedPlayerSlots' as array in PlayerType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("permittedCommanderTypes")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'permittedCommanderTypes' in PlayerType was expected to be string, but was of wrong type.");
						}
					}
					permittedCommanderTypes = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'permittedCommanderTypes' as array in PlayerType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("teamName")) {
				if (auto tPtr = ptr->if_string()) {
					teamName = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'teamName' as std::string in PlayerType, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (commanderTypeMod) {
				obj["commanderTypeMod"] = *commanderTypeMod;
			}
			if (permittedPlayerSlots) {
				json::array arr;
				for (auto const& val : *permittedPlayerSlots) {
					arr.push_back(json::string{ val });
				}
				obj["permittedPlayerSlots"] = std::move(arr);
			}
			if (permittedCommanderTypes) {
				json::array arr;
				for (auto const& val : *permittedCommanderTypes) {
					arr.push_back(json::string{ val });
				}
				obj["permittedCommanderTypes"] = std::move(arr);
			}
			if (teamName) {
				obj["teamName"] = *teamName;
			}
		}
	};
	struct PassiveUnitEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<std::vector<std::string>> unitTypeRequired;
		std::optional<std::vector<std::string>> classificationRequired;
		std::optional<std::vector<std::string>> terrainRequired;
		std::optional<int64_t> firepowerMod;
		std::optional<int64_t> defenseMod;
		std::optional<int64_t> indirectDefenseMod;
		std::optional<int64_t> minRangeMod;
		std::optional<int64_t> maxRangeMod;
		std::optional<int64_t> fuelUseMod;
		std::optional<int64_t> ammoUseMod;
		std::optional<int64_t> goodLuckMod;
		std::optional<int64_t> badLuckMod;
		std::optional<int64_t> movementMod;
		std::optional<int64_t> visionMod;
		std::optional<int64_t> terrainStarsMod;
		std::optional<int64_t> terrainStarsFlatMod;
		std::optional<int64_t> terrainStarsDefense;
		std::optional<int64_t> terrainStarsFirepower;
		std::optional<int64_t> counterfireMod;
		std::optional<bool> counterFirst;
		std::optional<int64_t> captureRateMod;
		std::optional<int64_t> unitCostMod;
		std::optional<bool> hiddenHitPoints;
		std::optional<int64_t> firepowerFromFunds;
		std::optional<int64_t> defenseFromFunds;
		std::optional<int64_t> fundsFromDamage;
		std::optional<std::map<std::string, int64_t>> firepowerFromOwnedTerrain;
		std::optional<std::map<std::string, int64_t>> defenseFromOwnedTerrain;
		std::optional<std::map<std::string, int64_t>> visionVariantMods;
		std::optional<std::map<std::string, int64_t>> firepowerVariantMods;
		std::optional<std::map<std::string, int64_t>> defenseVariantMods;
		std::optional<int64_t> coMeterChargeFromDealtDamage;
		std::optional<int64_t> coMeterChargeFromReceivedDamage;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in PassiveUnitEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in PassiveUnitEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in PassiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitTypeRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'unitTypeRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					unitTypeRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'unitTypeRequired' as array in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("classificationRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'classificationRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					classificationRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'classificationRequired' as array in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'terrainRequired' in PassiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					terrainRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'terrainRequired' as array in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("firepowerMod")) {
				if (auto tPtr = ptr->if_int64()) {
					firepowerMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'firepowerMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("defenseMod")) {
				if (auto tPtr = ptr->if_int64()) {
					defenseMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'defenseMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("indirectDefenseMod")) {
				if (auto tPtr = ptr->if_int64()) {
					indirectDefenseMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'indirectDefenseMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("minRangeMod")) {
				if (auto tPtr = ptr->if_int64()) {
					minRangeMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'minRangeMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("maxRangeMod")) {
				if (auto tPtr = ptr->if_int64()) {
					maxRangeMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'maxRangeMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fuelUseMod")) {
				if (auto tPtr = ptr->if_int64()) {
					fuelUseMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fuelUseMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("ammoUseMod")) {
				if (auto tPtr = ptr->if_int64()) {
					ammoUseMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'ammoUseMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("goodLuckMod")) {
				if (auto tPtr = ptr->if_int64()) {
					goodLuckMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'goodLuckMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("badLuckMod")) {
				if (auto tPtr = ptr->if_int64()) {
					badLuckMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'badLuckMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("movementMod")) {
				if (auto tPtr = ptr->if_int64()) {
					movementMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'movementMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("visionMod")) {
				if (auto tPtr = ptr->if_int64()) {
					visionMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'visionMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainStarsMod")) {
				if (auto tPtr = ptr->if_int64()) {
					terrainStarsMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'terrainStarsMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainStarsFlatMod")) {
				if (auto tPtr = ptr->if_int64()) {
					terrainStarsFlatMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'terrainStarsFlatMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainStarsDefense")) {
				if (auto tPtr = ptr->if_int64()) {
					terrainStarsDefense = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'terrainStarsDefense' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainStarsFirepower")) {
				if (auto tPtr = ptr->if_int64()) {
					terrainStarsFirepower = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'terrainStarsFirepower' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("counterfireMod")) {
				if (auto tPtr = ptr->if_int64()) {
					counterfireMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'counterfireMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("counterFirst")) {
				if (auto tPtr = ptr->if_bool()) {
					counterFirst = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'counterFirst' as bool in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("captureRateMod")) {
				if (auto tPtr = ptr->if_int64()) {
					captureRateMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'captureRateMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitCostMod")) {
				if (auto tPtr = ptr->if_int64()) {
					unitCostMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'unitCostMod' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("hiddenHitPoints")) {
				if (auto tPtr = ptr->if_bool()) {
					hiddenHitPoints = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'hiddenHitPoints' as bool in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("firepowerFromFunds")) {
				if (auto tPtr = ptr->if_int64()) {
					firepowerFromFunds = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'firepowerFromFunds' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("defenseFromFunds")) {
				if (auto tPtr = ptr->if_int64()) {
					defenseFromFunds = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'defenseFromFunds' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fundsFromDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					fundsFromDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fundsFromDamage' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("firepowerFromOwnedTerrain")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'firepowerFromOwnedTerrain' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
						}
					}
					firepowerFromOwnedTerrain = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'firepowerFromOwnedTerrain' as object in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("defenseFromOwnedTerrain")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'defenseFromOwnedTerrain' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
						}
					}
					defenseFromOwnedTerrain = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'defenseFromOwnedTerrain' as object in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("visionVariantMods")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'visionVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
						}
					}
					visionVariantMods = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'visionVariantMods' as object in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("firepowerVariantMods")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'firepowerVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
						}
					}
					firepowerVariantMods = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'firepowerVariantMods' as object in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("defenseVariantMods")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'defenseVariantMods' in PassiveUnitEffect was expected to be int64, but was of wrong type.");
						}
					}
					defenseVariantMods = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'defenseVariantMods' as object in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coMeterChargeFromDealtDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					coMeterChargeFromDealtDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coMeterChargeFromDealtDamage' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coMeterChargeFromReceivedDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					coMeterChargeFromReceivedDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coMeterChargeFromReceivedDamage' as int64_t in PassiveUnitEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (unitTypeRequired) {
				json::array arr;
				for (auto const& val : *unitTypeRequired) {
					arr.push_back(json::string{ val });
				}
				obj["unitTypeRequired"] = std::move(arr);
			}
			if (classificationRequired) {
				json::array arr;
				for (auto const& val : *classificationRequired) {
					arr.push_back(json::string{ val });
				}
				obj["classificationRequired"] = std::move(arr);
			}
			if (terrainRequired) {
				json::array arr;
				for (auto const& val : *terrainRequired) {
					arr.push_back(json::string{ val });
				}
				obj["terrainRequired"] = std::move(arr);
			}
			if (firepowerMod) {
				obj["firepowerMod"] = *firepowerMod;
			}
			if (defenseMod) {
				obj["defenseMod"] = *defenseMod;
			}
			if (indirectDefenseMod) {
				obj["indirectDefenseMod"] = *indirectDefenseMod;
			}
			if (minRangeMod) {
				obj["minRangeMod"] = *minRangeMod;
			}
			if (maxRangeMod) {
				obj["maxRangeMod"] = *maxRangeMod;
			}
			if (fuelUseMod) {
				obj["fuelUseMod"] = *fuelUseMod;
			}
			if (ammoUseMod) {
				obj["ammoUseMod"] = *ammoUseMod;
			}
			if (goodLuckMod) {
				obj["goodLuckMod"] = *goodLuckMod;
			}
			if (badLuckMod) {
				obj["badLuckMod"] = *badLuckMod;
			}
			if (movementMod) {
				obj["movementMod"] = *movementMod;
			}
			if (visionMod) {
				obj["visionMod"] = *visionMod;
			}
			if (terrainStarsMod) {
				obj["terrainStarsMod"] = *terrainStarsMod;
			}
			if (terrainStarsFlatMod) {
				obj["terrainStarsFlatMod"] = *terrainStarsFlatMod;
			}
			if (terrainStarsDefense) {
				obj["terrainStarsDefense"] = *terrainStarsDefense;
			}
			if (terrainStarsFirepower) {
				obj["terrainStarsFirepower"] = *terrainStarsFirepower;
			}
			if (counterfireMod) {
				obj["counterfireMod"] = *counterfireMod;
			}
			if (counterFirst) {
				obj["counterFirst"] = *counterFirst;
			}
			if (captureRateMod) {
				obj["captureRateMod"] = *captureRateMod;
			}
			if (unitCostMod) {
				obj["unitCostMod"] = *unitCostMod;
			}
			if (hiddenHitPoints) {
				obj["hiddenHitPoints"] = *hiddenHitPoints;
			}
			if (firepowerFromFunds) {
				obj["firepowerFromFunds"] = *firepowerFromFunds;
			}
			if (defenseFromFunds) {
				obj["defenseFromFunds"] = *defenseFromFunds;
			}
			if (fundsFromDamage) {
				obj["fundsFromDamage"] = *fundsFromDamage;
			}
			if (firepowerFromOwnedTerrain) {
				json::object m;
				for (auto const& entry : *firepowerFromOwnedTerrain) {
					m[entry.first] = entry.second;
				}
				obj["firepowerFromOwnedTerrain"] = std::move(m);
			}
			if (defenseFromOwnedTerrain) {
				json::object m;
				for (auto const& entry : *defenseFromOwnedTerrain) {
					m[entry.first] = entry.second;
				}
				obj["defenseFromOwnedTerrain"] = std::move(m);
			}
			if (visionVariantMods) {
				json::object m;
				for (auto const& entry : *visionVariantMods) {
					m[entry.first] = entry.second;
				}
				obj["visionVariantMods"] = std::move(m);
			}
			if (firepowerVariantMods) {
				json::object m;
				for (auto const& entry : *firepowerVariantMods) {
					m[entry.first] = entry.second;
				}
				obj["firepowerVariantMods"] = std::move(m);
			}
			if (defenseVariantMods) {
				json::object m;
				for (auto const& entry : *defenseVariantMods) {
					m[entry.first] = entry.second;
				}
				obj["defenseVariantMods"] = std::move(m);
			}
			if (coMeterChargeFromDealtDamage) {
				obj["coMeterChargeFromDealtDamage"] = *coMeterChargeFromDealtDamage;
			}
			if (coMeterChargeFromReceivedDamage) {
				obj["coMeterChargeFromReceivedDamage"] = *coMeterChargeFromReceivedDamage;
			}
		}
	};
	struct ActiveUnitEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<std::vector<std::string>> unitTypeRequired;
		std::optional<std::vector<std::string>> classificationRequired;
		std::optional<std::vector<std::string>> terrainRequired;
		std::optional<int64_t> hitPointMod;
		std::optional<int64_t> roundHitPoints;
		std::optional<bool> resupply;
		std::optional<bool> halveFuel;
		std::optional<bool> makeActive;
		std::optional<int64_t> stunDuration;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in ActiveUnitEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in ActiveUnitEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in ActiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitTypeRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'unitTypeRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					unitTypeRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'unitTypeRequired' as array in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("classificationRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'classificationRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					classificationRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'classificationRequired' as array in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'terrainRequired' in ActiveUnitEffect was expected to be string, but was of wrong type.");
						}
					}
					terrainRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'terrainRequired' as array in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("hitPointMod")) {
				if (auto tPtr = ptr->if_int64()) {
					hitPointMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'hitPointMod' as int64_t in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("roundHitPoints")) {
				if (auto tPtr = ptr->if_int64()) {
					roundHitPoints = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'roundHitPoints' as int64_t in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("resupply")) {
				if (auto tPtr = ptr->if_bool()) {
					resupply = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'resupply' as bool in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("halveFuel")) {
				if (auto tPtr = ptr->if_bool()) {
					halveFuel = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'halveFuel' as bool in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("makeActive")) {
				if (auto tPtr = ptr->if_bool()) {
					makeActive = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'makeActive' as bool in ActiveUnitEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("stunDuration")) {
				if (auto tPtr = ptr->if_int64()) {
					stunDuration = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'stunDuration' as int64_t in ActiveUnitEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (unitTypeRequired) {
				json::array arr;
				for (auto const& val : *unitTypeRequired) {
					arr.push_back(json::string{ val });
				}
				obj["unitTypeRequired"] = std::move(arr);
			}
			if (classificationRequired) {
				json::array arr;
				for (auto const& val : *classificationRequired) {
					arr.push_back(json::string{ val });
				}
				obj["classificationRequired"] = std::move(arr);
			}
			if (terrainRequired) {
				json::array arr;
				for (auto const& val : *terrainRequired) {
					arr.push_back(json::string{ val });
				}
				obj["terrainRequired"] = std::move(arr);
			}
			if (hitPointMod) {
				obj["hitPointMod"] = *hitPointMod;
			}
			if (roundHitPoints) {
				obj["roundHitPoints"] = *roundHitPoints;
			}
			if (resupply) {
				obj["resupply"] = *resupply;
			}
			if (halveFuel) {
				obj["halveFuel"] = *halveFuel;
			}
			if (makeActive) {
				obj["makeActive"] = *makeActive;
			}
			if (stunDuration) {
				obj["stunDuration"] = *stunDuration;
			}
		}
	};
	struct PassiveTerrainEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<std::vector<std::string>> affects;
		std::optional<std::vector<std::string>> terrainRequired;
		std::optional<std::vector<std::string>> classificationRequired;
		std::optional<int64_t> incomeMod;
		std::optional<int64_t> incomeFlatMod;
		std::optional<std::vector<std::string>> buildListMod;
		std::optional<int64_t> repairMod;
		std::optional<bool> occludesVisionMod;
		std::optional<int64_t> visionModBoost;
		std::optional<int64_t> buildCostMod;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in PassiveTerrainEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("affects")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'affects' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					affects = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'affects' as array in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'terrainRequired' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					terrainRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'terrainRequired' as array in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("classificationRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'classificationRequired' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					classificationRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'classificationRequired' as array in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("incomeMod")) {
				if (auto tPtr = ptr->if_int64()) {
					incomeMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'incomeMod' as int64_t in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("incomeFlatMod")) {
				if (auto tPtr = ptr->if_int64()) {
					incomeFlatMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'incomeFlatMod' as int64_t in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("buildListMod")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'buildListMod' in PassiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					buildListMod = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'buildListMod' as array in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("repairMod")) {
				if (auto tPtr = ptr->if_int64()) {
					repairMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'repairMod' as int64_t in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("occludesVisionMod")) {
				if (auto tPtr = ptr->if_bool()) {
					occludesVisionMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'occludesVisionMod' as bool in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("visionModBoost")) {
				if (auto tPtr = ptr->if_int64()) {
					visionModBoost = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'visionModBoost' as int64_t in PassiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("buildCostMod")) {
				if (auto tPtr = ptr->if_int64()) {
					buildCostMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'buildCostMod' as int64_t in PassiveTerrainEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (affects) {
				json::array arr;
				for (auto const& val : *affects) {
					arr.push_back(json::string{ val });
				}
				obj["affects"] = std::move(arr);
			}
			if (terrainRequired) {
				json::array arr;
				for (auto const& val : *terrainRequired) {
					arr.push_back(json::string{ val });
				}
				obj["terrainRequired"] = std::move(arr);
			}
			if (classificationRequired) {
				json::array arr;
				for (auto const& val : *classificationRequired) {
					arr.push_back(json::string{ val });
				}
				obj["classificationRequired"] = std::move(arr);
			}
			if (incomeMod) {
				obj["incomeMod"] = *incomeMod;
			}
			if (incomeFlatMod) {
				obj["incomeFlatMod"] = *incomeFlatMod;
			}
			if (buildListMod) {
				json::array arr;
				for (auto const& val : *buildListMod) {
					arr.push_back(json::string{ val });
				}
				obj["buildListMod"] = std::move(arr);
			}
			if (repairMod) {
				obj["repairMod"] = *repairMod;
			}
			if (occludesVisionMod) {
				obj["occludesVisionMod"] = *occludesVisionMod;
			}
			if (visionModBoost) {
				obj["visionModBoost"] = *visionModBoost;
			}
			if (buildCostMod) {
				obj["buildCostMod"] = *buildCostMod;
			}
		}
	};
	struct ActiveTerrainEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<std::vector<std::string>> affects;
		std::optional<std::vector<std::string>> terrainRequired;
		std::optional<std::string> unitSummonedName;
		std::optional<int64_t> unitSummonedInitialDamage;
		std::optional<bool> unitSummonedActive;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in ActiveTerrainEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("affects")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'affects' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					affects = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'affects' as array in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("terrainRequired")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'terrainRequired' in ActiveTerrainEffect was expected to be string, but was of wrong type.");
						}
					}
					terrainRequired = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'terrainRequired' as array in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitSummonedName")) {
				if (auto tPtr = ptr->if_string()) {
					unitSummonedName = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'unitSummonedName' as std::string in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitSummonedInitialDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					unitSummonedInitialDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'unitSummonedInitialDamage' as int64_t in ActiveTerrainEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitSummonedActive")) {
				if (auto tPtr = ptr->if_bool()) {
					unitSummonedActive = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'unitSummonedActive' as bool in ActiveTerrainEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (affects) {
				json::array arr;
				for (auto const& val : *affects) {
					arr.push_back(json::string{ val });
				}
				obj["affects"] = std::move(arr);
			}
			if (terrainRequired) {
				json::array arr;
				for (auto const& val : *terrainRequired) {
					arr.push_back(json::string{ val });
				}
				obj["terrainRequired"] = std::move(arr);
			}
			if (unitSummonedName) {
				obj["unitSummonedName"] = *unitSummonedName;
			}
			if (unitSummonedInitialDamage) {
				obj["unitSummonedInitialDamage"] = *unitSummonedInitialDamage;
			}
			if (unitSummonedActive) {
				obj["unitSummonedActive"] = *unitSummonedActive;
			}
		}
	};
	struct PassiveGlobalEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<std::string> variantMod;
		std::optional<std::map<std::string, int64_t>> variantHintMod;
		std::optional<std::string> movementClassVariantReplace;
		std::optional<std::string> movementClassVariantOverride;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in PassiveGlobalEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in PassiveGlobalEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in PassiveGlobalEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in PassiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("variantMod")) {
				if (auto tPtr = ptr->if_string()) {
					variantMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'variantMod' as std::string in PassiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("variantHintMod")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'variantHintMod' in PassiveGlobalEffect was expected to be int64, but was of wrong type.");
						}
					}
					variantHintMod = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'variantHintMod' as object in PassiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("movementClassVariantReplace")) {
				if (auto tPtr = ptr->if_string()) {
					movementClassVariantReplace = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'movementClassVariantReplace' as std::string in PassiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("movementClassVariantOverride")) {
				if (auto tPtr = ptr->if_string()) {
					movementClassVariantOverride = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'movementClassVariantOverride' as std::string in PassiveGlobalEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (variantMod) {
				obj["variantMod"] = *variantMod;
			}
			if (variantHintMod) {
				json::object m;
				for (auto const& entry : *variantHintMod) {
					m[entry.first] = entry.second;
				}
				obj["variantHintMod"] = std::move(m);
			}
			if (movementClassVariantReplace) {
				obj["movementClassVariantReplace"] = *movementClassVariantReplace;
			}
			if (movementClassVariantOverride) {
				obj["movementClassVariantOverride"] = *movementClassVariantOverride;
			}
		}
	};
	struct ActiveGlobalEffect {
		std::string name;
		std::optional<std::vector<std::string>> targets;
		std::optional<int64_t> fundMod;
		std::optional<int64_t> fundFlatMod;
		std::optional<int64_t> powerBarMod;
		std::optional<int64_t> powerBarPerFunds;
		std::optional<int64_t> missileCount;
		std::optional<std::vector<std::string>> missileTargetMethod;
		std::optional<int64_t> missileDamage;
		std::optional<int64_t> missileAreaOfEffect;
		std::optional<int64_t> missileStunDuration;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in ActiveGlobalEffect");
			}
			if (auto ptr = obj.if_contains("targets")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targets' in ActiveGlobalEffect was expected to be string, but was of wrong type.");
						}
					}
					targets = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targets' as array in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fundMod")) {
				if (auto tPtr = ptr->if_int64()) {
					fundMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fundMod' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fundFlatMod")) {
				if (auto tPtr = ptr->if_int64()) {
					fundFlatMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fundFlatMod' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("powerBarMod")) {
				if (auto tPtr = ptr->if_int64()) {
					powerBarMod = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'powerBarMod' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("powerBarPerFunds")) {
				if (auto tPtr = ptr->if_int64()) {
					powerBarPerFunds = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'powerBarPerFunds' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("missileCount")) {
				if (auto tPtr = ptr->if_int64()) {
					missileCount = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'missileCount' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("missileTargetMethod")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'missileTargetMethod' in ActiveGlobalEffect was expected to be string, but was of wrong type.");
						}
					}
					missileTargetMethod = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'missileTargetMethod' as array in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("missileDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					missileDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'missileDamage' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("missileAreaOfEffect")) {
				if (auto tPtr = ptr->if_int64()) {
					missileAreaOfEffect = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'missileAreaOfEffect' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("missileStunDuration")) {
				if (auto tPtr = ptr->if_int64()) {
					missileStunDuration = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'missileStunDuration' as int64_t in ActiveGlobalEffect, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (targets) {
				json::array arr;
				for (auto const& val : *targets) {
					arr.push_back(json::string{ val });
				}
				obj["targets"] = std::move(arr);
			}
			if (fundMod) {
				obj["fundMod"] = *fundMod;
			}
			if (fundFlatMod) {
				obj["fundFlatMod"] = *fundFlatMod;
			}
			if (powerBarMod) {
				obj["powerBarMod"] = *powerBarMod;
			}
			if (powerBarPerFunds) {
				obj["powerBarPerFunds"] = *powerBarPerFunds;
			}
			if (missileCount) {
				obj["missileCount"] = *missileCount;
			}
			if (missileTargetMethod) {
				json::array arr;
				for (auto const& val : *missileTargetMethod) {
					arr.push_back(json::string{ val });
				}
				obj["missileTargetMethod"] = std::move(arr);
			}
			if (missileDamage) {
				obj["missileDamage"] = *missileDamage;
			}
			if (missileAreaOfEffect) {
				obj["missileAreaOfEffect"] = *missileAreaOfEffect;
			}
			if (missileStunDuration) {
				obj["missileStunDuration"] = *missileStunDuration;
			}
		}
	};

	struct ModMetadata {
		std::string name;
		std::string version;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in ModMetadata, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in ModMetadata");
			}
			if (auto ptr = obj.if_contains("version")) {
				if (auto tPtr = ptr->if_string()) {
					version = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'version' as std::string in ModMetadata, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'version' was missing in ModMetadata");
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			obj["version"] = version;
		}
	};

	struct MovementClass {
		std::string name;
		std::map<std::string, int64_t> movementCosts;
		std::optional<std::map<std::string, std::map<std::string, int64_t>>> variantMods;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in MovementClass, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in MovementClass");
			}
			if (auto ptr = obj.if_contains("movementCosts")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'movementCosts' in MovementClass was expected to be int64, but was of wrong type.");
						}
					}
					movementCosts = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'movementCosts' as object in MovementClass, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'movementCosts' was missing in MovementClass");
			}
			if (auto ptr = obj.if_contains("variantMods")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, std::map<std::string, int64_t>> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_object()) {
							std::map<std::string, int64_t> subMap;
							for (auto const& subEntry : *mPtr) {
								if (auto mmPtr = subEntry.value().if_int64()) {
									subMap[subEntry.key_c_str()] = int64_t{ *mmPtr };
								}
								else {
									std::stringstream ss;
									ss << "Entry in map 'variantMods[";
									ss << entry.key_c_str();
									ss << "] in MovementClass was expected to be int64, but was of wrong type.";
									std::string exp = ss.str();
									throw std::runtime_error(exp.c_str());
								}
							}
							map[entry.key_c_str()] = std::move(subMap);
						}
						else {
							throw std::runtime_error("Entry in map 'variantMods' in MovementClass was expected to be object, but was of wrong type.");
						}
					}
					variantMods = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'variantMods' as object in MovementClass, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			{
				json::object m;
				for (auto const& entry : movementCosts) {
					m[entry.first] = entry.second;
				}
				obj["movementCosts"] = std::move(m);
			}
			if (variantMods) {
				json::object m;
				for (auto const& entry : *variantMods) {
					json::object mm;
					for (auto const& subEntry : entry.second) {
						mm[subEntry.first] = subEntry.second;
					}
					m[entry.first] = std::move(mm);
				}
				obj["variantMods"] = std::move(m);
			}
		}
	};
	struct TextResource {
		std::string key;
		std::string type;
		std::string shortName;
		std::string longName;
		std::string description;
		std::optional<std::string> language;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("key")) {
				if (auto tPtr = ptr->if_string()) {
					key = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'key' as std::string in TextResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'key' was missing in TextResource");
			}
			if (auto ptr = obj.if_contains("type")) {
				if (auto tPtr = ptr->if_string()) {
					type = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'type' as std::string in TextResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'type' was missing in TextResource");
			}
			if (auto ptr = obj.if_contains("shortName")) {
				if (auto tPtr = ptr->if_string()) {
					shortName = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'shortName' as std::string in TextResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'shortName' was missing in TextResource");
			}
			if (auto ptr = obj.if_contains("longName")) {
				if (auto tPtr = ptr->if_string()) {
					longName = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'longName' as std::string in TextResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'longName' was missing in TextResource");
			}
			if (auto ptr = obj.if_contains("description")) {
				if (auto tPtr = ptr->if_string()) {
					description = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'description' as std::string in TextResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'description' was missing in TextResource");
			}
			if (auto ptr = obj.if_contains("language")) {
				if (auto tPtr = ptr->if_string()) {
					language = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'language' as std::string in TextResource, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["key"] = key;
			obj["type"] = type;
			obj["shortName"] = shortName;
			obj["longName"] = longName;
			obj["description"] = description;
			if (language) {
				obj["language"] = *language;
			}
		}
	};
	struct ImageResource {
		std::string key;
		std::string type;
		std::string smallImage;
		std::string largeImage;
		std::optional<std::string> armyColor;
		std::optional<int64_t> orientation;
		std::optional<std::string> variant;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("key")) {
				if (auto tPtr = ptr->if_string()) {
					key = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'key' as std::string in ImageResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'key' was missing in ImageResource");
			}
			if (auto ptr = obj.if_contains("type")) {
				if (auto tPtr = ptr->if_string()) {
					type = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'type' as std::string in ImageResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'type' was missing in ImageResource");
			}
			if (auto ptr = obj.if_contains("smallImage")) {
				if (auto tPtr = ptr->if_string()) {
					smallImage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'smallImage' as std::string in ImageResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'smallImage' was missing in ImageResource");
			}
			if (auto ptr = obj.if_contains("largeImage")) {
				if (auto tPtr = ptr->if_string()) {
					largeImage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'largeImage' as std::string in ImageResource, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'largeImage' was missing in ImageResource");
			}
			if (auto ptr = obj.if_contains("armyColor")) {
				if (auto tPtr = ptr->if_string()) {
					armyColor = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'armyColor' as std::string in ImageResource, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("orientation")) {
				if (auto tPtr = ptr->if_int64()) {
					orientation = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'orientation' as int64_t in ImageResource, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("variant")) {
				if (auto tPtr = ptr->if_string()) {
					variant = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'variant' as std::string in ImageResource, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["key"] = key;
			obj["type"] = type;
			obj["smallImage"] = smallImage;
			obj["largeImage"] = largeImage;
			if (armyColor) {
				obj["armyColor"] = *armyColor;
			}
			if (orientation) {
				obj["orientation"] = *orientation;
			}
			if (variant) {
				obj["variant"] = *variant;
			}
		}
	};
	struct Settings {
		std::string name;
		std::optional<int64_t> startingFunds;
		std::optional<int64_t> incomeMultiplier;
		std::optional<bool> fogOfWar;
		std::optional<std::map<std::string, int64_t>> variant;
		std::optional<bool> coPowers;
		std::optional<bool> teams;
		std::optional<int64_t> unitLimit;
		std::optional<int64_t> captureLimit;
		std::optional<int64_t> dayLimit;
		std::optional<int64_t> coMeterSize;
		std::optional<int64_t> coMeterMultiplier;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in Settings, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in Settings");
			}
			if (auto ptr = obj.if_contains("startingFunds")) {
				if (auto tPtr = ptr->if_int64()) {
					startingFunds = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'startingFunds' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("incomeMultiplier")) {
				if (auto tPtr = ptr->if_int64()) {
					incomeMultiplier = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'incomeMultiplier' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fogOfWar")) {
				if (auto tPtr = ptr->if_bool()) {
					fogOfWar = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fogOfWar' as bool in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("variant")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'variant' in Settings was expected to be int64, but was of wrong type.");
						}
					}
					variant = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'variant' as object in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coPowers")) {
				if (auto tPtr = ptr->if_bool()) {
					coPowers = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coPowers' as bool in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("teams")) {
				if (auto tPtr = ptr->if_bool()) {
					teams = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'teams' as bool in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("unitLimit")) {
				if (auto tPtr = ptr->if_int64()) {
					unitLimit = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'unitLimit' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("captureLimit")) {
				if (auto tPtr = ptr->if_int64()) {
					captureLimit = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'captureLimit' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("dayLimit")) {
				if (auto tPtr = ptr->if_int64()) {
					dayLimit = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'dayLimit' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coMeterSize")) {
				if (auto tPtr = ptr->if_int64()) {
					coMeterSize = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coMeterSize' as int64_t in Settings, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("coMeterMultiplier")) {
				if (auto tPtr = ptr->if_int64()) {
					coMeterMultiplier = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'coMeterMultiplier' as int64_t in Settings, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			if (startingFunds) {
				obj["startingFunds"] = *startingFunds;
			}
			if (incomeMultiplier) {
				obj["incomeMultiplier"] = *incomeMultiplier;
			}
			if (fogOfWar) {
				obj["fogOfWar"] = *fogOfWar;
			}
			if (variant) {
				json::object m;
				for (auto const& entry : *variant) {
					m[entry.first] = entry.second;
				}
				obj["variant"] = std::move(m);
			}
			if (coPowers) {
				obj["coPowers"] = *coPowers;
			}
			if (teams) {
				obj["teams"] = *teams;
			}
			if (unitLimit) {
				obj["unitLimit"] = *unitLimit;
			}
			if (captureLimit) {
				obj["captureLimit"] = *captureLimit;
			}
			if (dayLimit) {
				obj["dayLimit"] = *dayLimit;
			}
			if (coMeterSize) {
				obj["coMeterSize"] = *coMeterSize;
			}
			if (coMeterMultiplier) {
				obj["coMeterMultiplier"] = *coMeterMultiplier;
			}
		}
	};
	struct TerrainType {
		std::string name;
		int64_t stars;
		std::optional<int64_t> maxCapturePoints;
		std::optional<std::string> sameAs;
		std::optional<std::vector<std::string>> buildList;
		std::optional<int64_t> income;
		std::optional<int64_t> repair;
		std::optional<std::vector<std::string>> repairList;
		std::optional<bool> occludesVision;
		std::optional<int64_t> hitPoints;
		std::optional<std::string> destroyed;
		std::optional<int64_t> destroyedOrientation;
		std::optional<std::string> damagedLike;
		std::optional<std::vector<std::string>> activateList;
		std::optional<std::vector<std::string>> activateActiveUnitEffects;
		std::optional<std::vector<std::string>> activateActiveTerrainEffects;
		std::optional<std::vector<std::string>> activateActiveGlobalEffects;
		std::optional<int64_t> activationMax;
		std::optional<std::string> activationChange;
		std::optional<bool> loseIfCaptured;
		std::optional<bool> loseIfAllCaptured;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in TerrainType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in TerrainType");
			}
			if (auto ptr = obj.if_contains("stars")) {
				if (auto tPtr = ptr->if_int64()) {
					stars = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'stars' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'stars' was missing in TerrainType");
			}
			if (auto ptr = obj.if_contains("maxCapturePoints")) {
				if (auto tPtr = ptr->if_int64()) {
					maxCapturePoints = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'maxCapturePoints' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("sameAs")) {
				if (auto tPtr = ptr->if_string()) {
					sameAs = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'sameAs' as std::string in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("buildList")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'buildList' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					buildList = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'buildList' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("income")) {
				if (auto tPtr = ptr->if_int64()) {
					income = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'income' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("repair")) {
				if (auto tPtr = ptr->if_int64()) {
					repair = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'repair' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("repairList")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'repairList' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					repairList = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'repairList' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("occludesVision")) {
				if (auto tPtr = ptr->if_bool()) {
					occludesVision = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'occludesVision' as bool in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("hitPoints")) {
				if (auto tPtr = ptr->if_int64()) {
					hitPoints = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'hitPoints' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("destroyed")) {
				if (auto tPtr = ptr->if_string()) {
					destroyed = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'destroyed' as std::string in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("destroyedOrientation")) {
				if (auto tPtr = ptr->if_int64()) {
					destroyedOrientation = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'destroyedOrientation' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("damagedLike")) {
				if (auto tPtr = ptr->if_string()) {
					damagedLike = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'damagedLike' as std::string in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activateList")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activateList' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					activateList = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activateList' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activateActiveUnitEffects")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activateActiveUnitEffects' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					activateActiveUnitEffects = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activateActiveUnitEffects' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activateActiveTerrainEffects")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activateActiveTerrainEffects' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					activateActiveTerrainEffects = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activateActiveTerrainEffects' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activateActiveGlobalEffects")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'activateActiveGlobalEffects' in TerrainType was expected to be string, but was of wrong type.");
						}
					}
					activateActiveGlobalEffects = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'activateActiveGlobalEffects' as array in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activationMax")) {
				if (auto tPtr = ptr->if_int64()) {
					activationMax = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'activationMax' as int64_t in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("activationChange")) {
				if (auto tPtr = ptr->if_string()) {
					activationChange = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'activationChange' as std::string in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("loseIfCaptured")) {
				if (auto tPtr = ptr->if_bool()) {
					loseIfCaptured = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'loseIfCaptured' as bool in TerrainType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("loseIfAllCaptured")) {
				if (auto tPtr = ptr->if_bool()) {
					loseIfAllCaptured = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'loseIfAllCaptured' as bool in TerrainType, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			obj["stars"] = stars;
			if (maxCapturePoints) {
				obj["maxCapturePoints"] = *maxCapturePoints;
			}
			if (sameAs) {
				obj["sameAs"] = *sameAs;
			}
			if (buildList) {
				json::array arr;
				for (auto const& val : *buildList) {
					arr.push_back(json::string{ val });
				}
				obj["buildList"] = std::move(arr);
			}
			if (income) {
				obj["income"] = *income;
			}
			if (repair) {
				obj["repair"] = *repair;
			}
			if (repairList) {
				json::array arr;
				for (auto const& val : *repairList) {
					arr.push_back(json::string{ val });
				}
				obj["repairList"] = std::move(arr);
			}
			if (occludesVision) {
				obj["occludesVision"] = *occludesVision;
			}
			if (hitPoints) {
				obj["hitPoints"] = *hitPoints;
			}
			if (destroyed) {
				obj["destroyed"] = *destroyed;
			}
			if (destroyedOrientation) {
				obj["destroyedOrientation"] = *destroyedOrientation;
			}
			if (damagedLike) {
				obj["damagedLike"] = *damagedLike;
			}
			if (activateList) {
				json::array arr;
				for (auto const& val : *activateList) {
					arr.push_back(json::string{ val });
				}
				obj["activateList"] = std::move(arr);
			}
			if (activateActiveUnitEffects) {
				json::array arr;
				for (auto const& val : *activateActiveUnitEffects) {
					arr.push_back(json::string{ val });
				}
				obj["activateActiveUnitEffects"] = std::move(arr);
			}
			if (activateActiveTerrainEffects) {
				json::array arr;
				for (auto const& val : *activateActiveTerrainEffects) {
					arr.push_back(json::string{ val });
				}
				obj["activateActiveTerrainEffects"] = std::move(arr);
			}
			if (activateActiveGlobalEffects) {
				json::array arr;
				for (auto const& val : *activateActiveGlobalEffects) {
					arr.push_back(json::string{ val });
				}
				obj["activateActiveGlobalEffects"] = std::move(arr);
			}
			if (activationMax) {
				obj["activationMax"] = *activationMax;
			}
			if (activationChange) {
				obj["activationChange"] = *activationChange;
			}
			if (loseIfCaptured) {
				obj["loseIfCaptured"] = *loseIfCaptured;
			}
			if (loseIfAllCaptured) {
				obj["loseIfAllCaptured"] = *loseIfAllCaptured;
			}
		}
	};

	struct UnitType {
		std::string name;
		int64_t cost;
		int64_t maxFuel;
		int64_t maxAmmo;
		int64_t visionRange;
		int64_t movementRange;
		std::string movementClass;
		std::vector<std::string> classifications;
		std::optional<int64_t> fuelPerDay;
		std::optional<int64_t> fuelPerDayStealth;
		std::optional<std::vector<std::string>> weapons;
		std::optional<int64_t> supplyRepair;
		std::optional<int64_t> transportCapacity;
		std::optional<std::vector<std::string>> transportList;
		std::optional<int64_t> hitPoints;
		std::optional<int64_t> captureSpeed;
		std::optional<bool> ignoresVisionOcclusion;
		std::optional<std::string> stealthType;
		std::optional<bool> stationaryFire;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("cost")) {
				if (auto tPtr = ptr->if_int64()) {
					cost = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'cost' as int64_t in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'cost' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("maxFuel")) {
				if (auto tPtr = ptr->if_int64()) {
					maxFuel = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'maxFuel' as int64_t in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'maxFuel' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("maxAmmo")) {
				if (auto tPtr = ptr->if_int64()) {
					maxAmmo = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'maxAmmo' as int64_t in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'maxAmmo' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("visionRange")) {
				if (auto tPtr = ptr->if_int64()) {
					visionRange = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'visionRange' as int64_t in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'visionRange' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("movementRange")) {
				if (auto tPtr = ptr->if_int64()) {
					movementRange = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'movementRange' as int64_t in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'movementRange' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("movementClass")) {
				if (auto tPtr = ptr->if_string()) {
					movementClass = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'movementClass' as std::string in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'movementClass' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("classifications")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'classifications' in UnitType was expected to be string, but was of wrong type.");
						}
					}
					classifications = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'classifications' as array in UnitType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'classifications' was missing in UnitType");
			}
			if (auto ptr = obj.if_contains("fuelPerDay")) {
				if (auto tPtr = ptr->if_int64()) {
					fuelPerDay = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fuelPerDay' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("fuelPerDayStealth")) {
				if (auto tPtr = ptr->if_int64()) {
					fuelPerDayStealth = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'fuelPerDayStealth' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("weapons")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'weapons' in UnitType was expected to be string, but was of wrong type.");
						}
					}
					weapons = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'weapons' as array in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("supplyRepair")) {
				if (auto tPtr = ptr->if_int64()) {
					supplyRepair = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'supplyRepair' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("transportCapacity")) {
				if (auto tPtr = ptr->if_int64()) {
					transportCapacity = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'transportCapacity' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("transportList")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'transportList' in UnitType was expected to be string, but was of wrong type.");
						}
					}
					transportList = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'transportList' as array in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("hitPoints")) {
				if (auto tPtr = ptr->if_int64()) {
					hitPoints = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'hitPoints' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("captureSpeed")) {
				if (auto tPtr = ptr->if_int64()) {
					captureSpeed = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'captureSpeed' as int64_t in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("ignoresVisionOcclusion")) {
				if (auto tPtr = ptr->if_bool()) {
					ignoresVisionOcclusion = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'ignoresVisionOcclusion' as bool in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("stealthType")) {
				if (auto tPtr = ptr->if_string()) {
					stealthType = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'stealthType' as std::string in UnitType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("stationaryFire")) {
				if (auto tPtr = ptr->if_bool()) {
					stationaryFire = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'stationaryFire' as bool in UnitType, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			obj["cost"] = cost;
			obj["maxFuel"] = maxFuel;
			obj["maxAmmo"] = maxAmmo;
			obj["visionRange"] = visionRange;
			obj["movementRange"] = movementRange;
			obj["movementClass"] = movementClass;
			{
				json::array arr;
				for (auto const& val : classifications) {
					arr.push_back(json::string{ val });
				}
				obj["classifications"] = std::move(arr);
			}
			if (fuelPerDay) {
				obj["fuelPerDay"] = *fuelPerDay;
			}
			if (fuelPerDayStealth) {
				obj["fuelPerDayStealth"] = *fuelPerDayStealth;
			}
			if (weapons) {
				json::array arr;
				for (auto const& val : *weapons) {
					arr.push_back(json::string{ val });
				}
				obj["weapons"] = std::move(arr);
			}
			if (supplyRepair) {
				obj["supplyRepair"] = *supplyRepair;
			}
			if (transportCapacity) {
				obj["transportCapacity"] = *transportCapacity;
			}
			if (transportList) {
				json::array arr;
				for (auto const& val : *transportList) {
					arr.push_back(json::string{ val });
				}
				obj["transportList"] = std::move(arr);
			}
			if (hitPoints) {
				obj["hitPoints"] = *hitPoints;
			}
			if (captureSpeed) {
				obj["captureSpeed"] = *captureSpeed;
			}
			if (ignoresVisionOcclusion) {
				obj["ignoresVisionOcclusion"] = *ignoresVisionOcclusion;
			}
			if (stealthType) {
				obj["stealthType"] = *stealthType;
			}
			if (stationaryFire) {
				obj["stationaryFire"] = *stationaryFire;
			}
		}
	};
	struct WeaponType {
		std::string name;
		int64_t ammoConsumed;
		std::optional<std::map<std::string, int64_t>> baseDamage;
		int64_t maxRange;
		std::optional<int64_t> minRange;
		std::optional<bool> selfTarget;
		std::optional<bool> affectedByLuck;
		std::optional<bool> nonLethal;
		std::optional<int64_t> areaOfEffect;
		std::optional<std::vector<std::string>> targetsStealth;
		std::optional<int64_t> flatDamage;

		void readFrom(json::object const& obj) {
			if (auto ptr = obj.if_contains("name")) {
				if (auto tPtr = ptr->if_string()) {
					name = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'name' as std::string in WeaponType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'name' was missing in WeaponType");
			}
			if (auto ptr = obj.if_contains("ammoConsumed")) {
				if (auto tPtr = ptr->if_int64()) {
					ammoConsumed = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'ammoConsumed' as int64_t in WeaponType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'ammoConsumed' was missing in WeaponType");
			}
			if (auto ptr = obj.if_contains("baseDamage")) {
				if (auto tPtr = ptr->if_object()) {
					std::map<std::string, int64_t> map;
					for (auto const& entry : *tPtr) {
						if (auto mPtr = entry.value().if_int64()) {
							map[entry.key_c_str()] = int64_t{ *mPtr };
						}
						else {
							throw std::runtime_error("Entry in map 'baseDamage' in WeaponType was expected to be int64, but was of wrong type.");
						}
					}
					baseDamage = std::move(map);
				}
				else {
					throw std::runtime_error("Expected 'baseDamage' as object in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("maxRange")) {
				if (auto tPtr = ptr->if_int64()) {
					maxRange = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'maxRange' as int64_t in WeaponType, but was of wrong type.");
				}
			}
			else {
				throw std::runtime_error("'maxRange' was missing in WeaponType");
			}
			if (auto ptr = obj.if_contains("minRange")) {
				if (auto tPtr = ptr->if_int64()) {
					minRange = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'minRange' as int64_t in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("selfTarget")) {
				if (auto tPtr = ptr->if_bool()) {
					selfTarget = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'selfTarget' as bool in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("affectedByLuck")) {
				if (auto tPtr = ptr->if_bool()) {
					affectedByLuck = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'affectedByLuck' as bool in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("nonLethal")) {
				if (auto tPtr = ptr->if_bool()) {
					nonLethal = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'nonLethal' as bool in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("areaOfEffect")) {
				if (auto tPtr = ptr->if_int64()) {
					areaOfEffect = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'areaOfEffect' as int64_t in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("targetsStealth")) {
				if (auto tPtr = ptr->if_array()) {
					std::vector<std::string> arr;
					for (auto const& val : *tPtr) {
						if (auto vPtr = val.if_string()) {
							arr.push_back(std::string{ *vPtr });
						}
						else {
							throw std::runtime_error("Item in array 'targetsStealth' in WeaponType was expected to be string, but was of wrong type.");
						}
					}
					targetsStealth = std::move(arr);
				}
				else {
					throw std::runtime_error("Expected 'targetsStealth' as array in WeaponType, but was of wrong type.");
				}
			}
			if (auto ptr = obj.if_contains("flatDamage")) {
				if (auto tPtr = ptr->if_int64()) {
					flatDamage = *tPtr;
				}
				else {
					throw std::runtime_error("Expected 'flatDamage' as int64_t in WeaponType, but was of wrong type.");
				}
			}
		}

		void writeTo(json::object& obj) const {
			obj["name"] = name;
			obj["ammoConsumed"] = ammoConsumed;
			if (baseDamage) {
				json::object m;
				for (auto const& entry : *baseDamage) {
					m[entry.first] = entry.second;
				}
				obj["baseDamage"] = std::move(m);
			}
			obj["maxRange"] = maxRange;
			if (minRange) {
				obj["minRange"] = *minRange;
			}
			if (selfTarget) {
				obj["selfTarget"] = *selfTarget;
			}
			if (affectedByLuck) {
				obj["affectedByLuck"] = *affectedByLuck;
			}
			if (nonLethal) {
				obj["nonLethal"] = *nonLethal;
			}
			if (areaOfEffect) {
				obj["areaOfEffect"] = *areaOfEffect;
			}
			if (targetsStealth) {
				json::array arr;
				for (auto const& val : *targetsStealth) {
					arr.push_back(json::string{ val });
				}
				obj["targetsStealth"] = std::move(arr);
			}
			if (flatDamage) {
				obj["flatDamage"] = *flatDamage;
			}
		}
	};

}
