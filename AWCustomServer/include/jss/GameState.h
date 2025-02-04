#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct GameState{
        int64_t id;
        int64_t day;
        int64_t playerTurn;
        std::vector<int64_t> playerOrder;
        std::string variant;
        std::optional<std::string> variantStart;
        bool active;
        int64_t hash;
    };

    BOOST_DESCRIBE_STRUCT(GameState, (), (id, day, playerTurn, playerOrder, variant, variantStart, active, hash))
}
