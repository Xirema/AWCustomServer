#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct TerrainState{
        int64_t id;
        int64_t x;
        int64_t y;
        std::string name;
        std::optional<int64_t> orientation;
        std::optional<int64_t> capturePoints;
        std::optional<int64_t> owner;
        std::optional<int64_t> activationCount;
        std::optional<int64_t> hitPoints;
    };
    BOOST_DESCRIBE_STRUCT(TerrainState, (), (
        id,
        x,
        y,
        name,
        orientation,
        capturePoints,
        owner,
        activationCount,
        hitPoints
    ))
}
