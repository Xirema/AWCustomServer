#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct UnitType{
        std::string name;
        int64_t cost;
        int64_t maxFuel;
        int64_t maxAmmo;
        int64_t visionRange;
        int64_t movementRange;
        std::string movementClass;
        std::vector<std::string> classifications;
        std::optional<int64_t> fuelPerDay;
        std::optional<int64_t> fuelPerDayStealth;
        std::optional<std::vector<std::string>> weapons;
        std::optional<int64_t> supplyRepair;
        std::optional<int64_t> transportCapacity;
        std::optional<std::vector<std::string>> transportList;
        std::optional<int64_t> hitPoints;
        std::optional<int64_t> captureSpeed;
        std::optional<bool> ignoresVisionOcclusion;
        std::optional<std::string> stealthType;
        std::optional<bool> stationaryFire;

        void readFrom(json::object const& obj);
        void writeTo(json::object & obj) const;
    };
    struct WeaponType{
        std::string name;
        int64_t ammoConsumed;
        std::optional<std::map<std::string, int64_t>> baseDamage;
        int64_t maxRange;
        std::optional<int64_t> minRange;
        std::optional<bool> selfTarget;
        std::optional<bool> affectedByLuck;
        std::optional<bool> nonLethal;
        std::optional<int64_t> areaOfEffect;
        std::optional<std::vector<std::string>> targetsStealth;
        std::optional<int64_t> flatDamage;

        void readFrom(json::object const& obj);
        void writeTo(json::object & obj) const;
    };
}
