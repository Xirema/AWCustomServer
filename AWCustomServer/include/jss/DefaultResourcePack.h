#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct DefaultResourcePack{
        std::string name;
        std::optional<std::string> version;

        void readFrom(json::object const& obj);
        void writeTo(json::object & obj) const;
    };
}
