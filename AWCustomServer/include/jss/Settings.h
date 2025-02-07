#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct Settings{
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
    };
    BOOST_DESCRIBE_STRUCT(Settings, (), (
        name,
        startingFunds,
        incomeMultiplier,
        fogOfWar,
        variant,
        coPowers,
        teams,
        unitLimit,
        captureLimit,
        dayLimit,
        coMeterSize,
        coMeterMultiplier
    ))
    struct Config {
        std::optional<int64_t> minTerrainStars;
        std::optional<bool> unlimitedUnload;
        std::optional<bool> terrainDefenseScalesWithHitpoints;
        std::optional<bool> terrainFirepowerScalesWithHitpoints;
    };
    BOOST_DESCRIBE_STRUCT(Config, (), (
        minTerrainStars,
        unlimitedUnload,
        terrainDefenseScalesWithHitpoints,
        terrainFirepowerScalesWithHitpoints
    ))
}
