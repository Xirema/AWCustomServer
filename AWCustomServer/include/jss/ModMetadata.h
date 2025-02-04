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
    };
    BOOST_DESCRIBE_STRUCT(ModMetadata, (), (
        name,
        version,
        modId,
        expired,
        defaultResourcePacks
    ))
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
    };
    BOOST_DESCRIBE_STRUCT(ModData, (), (
        protocol,
        modMetadata,
        units,
        weapons,
        terrains,
        movements,
        movementRules,
        commanders,
        players,
        passiveUnitEffects,
        activeUnitEffects,
        passiveTerrainEffects,
        activeTerrainEffects,
        passiveGlobalEffects,
        activeGlobalEffects,
        defaultSettings
    ))
}
