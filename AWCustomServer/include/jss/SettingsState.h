#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct SettingsState{
        std::string id;
        int64_t startingFunds;
        int64_t incomeMultiplier;
        bool fogOfWar;
        std::map<std::string, int64_t> variant;
        bool coPowers;
        bool teams;
        std::string modId;
        int64_t coMeterSize;
        int64_t coMeterMultiplier;
        std::optional<int64_t> unitLimit;
        std::optional<int64_t> captureLimit;
        std::optional<int64_t> dayLimit;
    };
    BOOST_DESCRIBE_STRUCT(SettingsState, (), (
        id,
        startingFunds,
        incomeMultiplier,
        fogOfWar,
        variant,
        coPowers,
        teams,
        modId,
        coMeterSize,
        coMeterMultiplier,
        unitLimit,
        captureLimit,
        dayLimit
    ))
}
