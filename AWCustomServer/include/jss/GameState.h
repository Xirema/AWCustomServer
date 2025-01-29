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

        void readFrom(json::object const& obj);
        void writeTo(json::object & obj) const;
    };
}
