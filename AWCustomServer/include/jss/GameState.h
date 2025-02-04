#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct GameState{
        std::string id;
        int64_t day;
        int64_t playerTurn;
        std::vector<std::string> playerOrder;
        std::string variant;
        bool active;
    };

    BOOST_DESCRIBE_STRUCT(GameState, (), (id, day, playerTurn, playerOrder, variant, active))
}
