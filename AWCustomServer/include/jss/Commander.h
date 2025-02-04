#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct CommanderType{
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
    };

    BOOST_DESCRIBE_STRUCT(CommanderType, (), (
        name, passiveUnitEffectsD2d, passiveTerrainEffectsD2d, passiveGlobalEffectsD2d,
        copCost, passiveUnitEffectsCop, activeUnitEffectsCop, passiveTerrainEffectsCop, activeTerrainEffectsCop, passiveGlobalEffectsCop, activeGlobalEffectsCop,
        scopCost, passiveUnitEffectsScop, activeUnitEffectsScop, passiveTerrainEffectsScop, activeTerrainEffectsScop, passiveGlobalEffectsScop, activeGlobalEffectsScop,
        coMeterMultiplier,
        playable
    ))
    struct PlayerType{
        std::string name;
        std::optional<std::string> commanderTypeMod;
        std::optional<std::vector<std::string>> permittedPlayerSlots;
        std::optional<std::vector<std::string>> permittedCommanderTypes;
        std::optional<std::string> teamName;
    };
    BOOST_DESCRIBE_STRUCT(PlayerType, (), (name, commanderTypeMod, permittedPlayerSlots, permittedCommanderTypes, teamName))
}
