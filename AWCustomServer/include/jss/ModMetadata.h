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

        void readFrom(json::object const& obj);
        void writeTo(json::object& obj) const;
    };
    struct ModData {
        std::optional<std::string> protocol;
        ModMetadata modMetadata;
        std::vector<UnitType> units;
        std::vector<WeaponType> weapons;
        std::vector<TerrainType> terrains;
        std::vector<MovementClass> movements;
        std::vector<MovementRule> movementRules;
        std::vector<CommanderType> commanders;
        std::vector<PlayerType> players;
        std::vector<PassiveUnitEffect> passiveUnitEffects;
        std::vector<ActiveUnitEffect> activeUnitEffects;
        std::vector<PassiveTerrainEffect> passiveTerrainEffects;
        std::vector<ActiveTerrainEffect> activeTerrainEffects;
        std::vector<PassiveGlobalEffect> passiveGlobalEffects;
        std::vector<ActiveGlobalEffect> activeGlobalEffects;
        std::vector<Settings> defaultSettings;

        void readFrom(json::object const& obj);
        void writeTo(json::object& obj) const;
    };
}
