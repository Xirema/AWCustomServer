#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct TerrainType{
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
    };
    BOOST_DESCRIBE_STRUCT(TerrainType, (), (
        name,
        stars,
        maxCapturePoints,
        sameAs,
        buildList,
        income,
        repair,
        repairList,
        occludesVision,
        hitPoints,
        destroyed,
        destroyedOrientation,
        damagedLike,
        activateList,
        activateActiveUnitEffects,
        activateActiveTerrainEffects,
        activateActiveGlobalEffects,
        activationMax,
        activationChange,
        loseIfCaptured,
        loseIfAllCaptured
    ))
}
