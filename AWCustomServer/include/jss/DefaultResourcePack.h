#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct DefaultResourcePack{
        std::string name;
        std::optional<std::string> version;
    };
    BOOST_DESCRIBE_STRUCT(DefaultResourcePack, (), (name, version))
}
