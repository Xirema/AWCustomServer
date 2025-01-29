#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct TerrainState{
        std::string id;
        int64_t x;
        int64_t y;
        std::string name;
        std::optional<int64_t> orientation;
        std::optional<int64_t> capturePoints;
        std::optional<std::string> owner;
        std::optional<int64_t> activationCount;
        std::optional<int64_t> hitPoints;

        void readFrom(json::object const& obj);
        void writeTo(json::object & obj) const;
    };
}
