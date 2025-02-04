#pragma once
#include<jss/jssinclude.h>

namespace sTypes{
    struct PlayerState{
        std::string id;
        std::string owner;
        std::string commanderName;
        int64_t funds;
        int64_t powerCharge;
        std::string armyColor;
        bool alive;
        int64_t totalPowerUses;
        int64_t unitFacing;
        std::string playerType;
        std::optional<std::string> team;
        std::optional<std::string> powerActive;
        std::optional<int64_t> powerActiveDay;
        std::optional<std::vector<std::string>> bannedUnits;
        std::optional<int64_t> incomeMultiplier;
        std::optional<int64_t> coMeterMultiplier;
    };
    BOOST_DESCRIBE_STRUCT(PlayerState, (), (
        id,
        owner,
        commanderName,
        funds,
        powerCharge,
        armyColor,
        alive,
        totalPowerUses,
        unitFacing,
        playerType,
        team,
        powerActive,
        powerActiveDay,
        bannedUnits,
        incomeMultiplier,
        coMeterMultiplier
    ))
}
