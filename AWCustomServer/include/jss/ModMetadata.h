#pragma once
#include<jss/jssinclude.h>
#include<jss/DefaultResourcePack.h>
#include<jss/Commander.h>
#include<jss/Effect.h>
#include<jss/Movement.h>
#include<jss/Settings.h>
#include<jss/Terrain.h>
#include<jss/Unit.h>

namespace dTypes {
    struct ModMetadata {
        std::string name;
        std::string version;
        std::optional<std::string> modId;
        std::optional<bool> expired;
        std::optional<std::vector<DefaultResourcePack>> defaultResourcePacks;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("name")) {
                if (auto tPtr = ptr->if_string()) {
                    name = *tPtr;
                }
                else {
                    throw std::runtime_error("Expected 'name' as std::string in ModMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
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
                    throw std::runtime_error("Expected 'version' as std::string in ModMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'version' was missing in ModMetadata");
            }
            if (auto ptr = obj.if_contains("modId")) {
                if (auto tPtr = ptr->if_string()) {
                    modId = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'modId' as std::string in ModMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            if (auto ptr = obj.if_contains("expired")) {
                if (auto tPtr = ptr->if_bool()) {
                    expired = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'expired' as bool in ModMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            if (auto ptr = obj.if_contains("defaultResourcePacks")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<DefaultResourcePack> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            DefaultResourcePack v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'defaultResourcePacks' in ModMetadata was expected to be object, but was of wrong type.");
                        }
                    }
                    defaultResourcePacks = std::move(arr);
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'defaultResourcePacks' as array in ModMetadata, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
        }

        void writeTo(json::object& obj) const {
            obj["name"] = name;
            obj["version"] = version;
            if (modId) {
                obj["modId"] = *modId;
            }
            if (expired) {
                obj["expired"] = *expired;
            }
            if (defaultResourcePacks) {
                json::array arr;
                for (auto const& val : *defaultResourcePacks) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["defaultResourcePacks"] = std::move(arr);
            }
        }
    };
    struct ModData {
        std::optional<std::string> protocol;
        ModMetadata modMetadata;
        std::vector<UnitType> units;
        std::vector<WeaponType> weapons;
        std::vector<TerrainType> terrains;
        std::vector<MovementClass> movements;
        std::vector<CommanderType> commanders;
        std::vector<PlayerType> players;
        std::vector<PassiveUnitEffect> passiveUnitEffects;
        std::vector<ActiveUnitEffect> activeUnitEffects;
        std::vector<PassiveTerrainEffect> passiveTerrainEffects;
        std::vector<ActiveTerrainEffect> activeTerrainEffects;
        std::vector<PassiveGlobalEffect> passiveGlobalEffects;
        std::vector<ActiveGlobalEffect> activeGlobalEffects;
        std::vector<Settings> defaultSettings;

        void readFrom(json::object const& obj) {
            if (auto ptr = obj.if_contains("protocol")) {
                if (auto tPtr = ptr->if_string()) {
                    protocol = *tPtr;
                }
                else if (!ptr->is_null()) {
                    throw std::runtime_error("Expected 'protocol' as std::string in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            if (auto ptr = obj.if_contains("modMetadata")) {
                if (auto tPtr = ptr->if_object()) {
                    modMetadata.readFrom(*tPtr);
                }
                else {
                    throw std::runtime_error("Expected 'modMetadata' as ModMetadata in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'modMetadata' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("units")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<UnitType> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            UnitType v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'units' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    units = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'units' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'units' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("weapons")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<WeaponType> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            WeaponType v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'weapons' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    weapons = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'weapons' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'weapons' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("terrains")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<TerrainType> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            TerrainType v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'terrains' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    terrains = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'terrains' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'terrains' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("movements")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<MovementClass> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            MovementClass v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'movements' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    movements = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'movements' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'movements' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("commanders")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<CommanderType> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            CommanderType v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'commanders' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    commanders = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'commanders' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'commanders' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("players")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<PlayerType> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            PlayerType v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'players' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    players = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'players' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'players' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("passiveUnitEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<PassiveUnitEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            PassiveUnitEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'passiveUnitEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    passiveUnitEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'passiveUnitEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'passiveUnitEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("activeUnitEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<ActiveUnitEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            ActiveUnitEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'activeUnitEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    activeUnitEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'activeUnitEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'activeUnitEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("passiveTerrainEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<PassiveTerrainEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            PassiveTerrainEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'passiveTerrainEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    passiveTerrainEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'passiveTerrainEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'passiveTerrainEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("activeTerrainEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<ActiveTerrainEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            ActiveTerrainEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'activeTerrainEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    activeTerrainEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'activeTerrainEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'activeTerrainEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("passiveGlobalEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<PassiveGlobalEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            PassiveGlobalEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'passiveGlobalEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    passiveGlobalEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'passiveGlobalEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'passiveGlobalEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("activeGlobalEffects")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<ActiveGlobalEffect> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            ActiveGlobalEffect v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'activeGlobalEffects' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    activeGlobalEffects = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'activeGlobalEffects' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'activeGlobalEffects' was missing in ModData");
            }
            if (auto ptr = obj.if_contains("defaultSettings")) {
                if (auto tPtr = ptr->if_array()) {
                    std::vector<Settings> arr;
                    for (auto const& val : *tPtr) {
                        if (auto vPtr = val.if_object()) {
                            Settings v;
                            v.readFrom(*vPtr);
                            arr.push_back(std::move(v));
                        }
                        else {
                            throw std::runtime_error("Item in array 'defaultSettings' in ModData was expected to be object, but was of wrong type.");
                        }
                    }
                    defaultSettings = std::move(arr);
                }
                else {
                    throw std::runtime_error("Expected 'defaultSettings' as array in ModData, but was of type " + std::string(to_string(ptr->kind())) + ".");
                }
            }
            else {
                throw std::runtime_error("'defaultSettings' was missing in ModData");
            }
        }

        void writeTo(json::object& obj) const {
            if (protocol) {
                obj["protocol"] = *protocol;
            }
            {
                json::object m;
                modMetadata.writeTo(m);
                obj["modMetadata"] = std::move(m);
            }
            {
                json::array arr;
                for (auto const& val : units) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["units"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : weapons) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["weapons"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : terrains) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["terrains"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : movements) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["movements"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : commanders) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["commanders"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : players) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["players"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : passiveUnitEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["passiveUnitEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : activeUnitEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["activeUnitEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : passiveTerrainEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["passiveTerrainEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : activeTerrainEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["activeTerrainEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : passiveGlobalEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["passiveGlobalEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : activeGlobalEffects) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["activeGlobalEffects"] = std::move(arr);
            }
            {
                json::array arr;
                for (auto const& val : defaultSettings) {
                    json::object obj;
                    val.writeTo(obj);
                    arr.push_back(std::move(obj));
                }
                obj["defaultSettings"] = std::move(arr);
            }
        }
    };
}
