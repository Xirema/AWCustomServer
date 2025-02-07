#pragma once
#include<jss/jssinclude.h>

namespace dTypes{
    struct PassiveUnitEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> unitTypeRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<int64_t> firepowerMod;
        std::optional<int64_t> defenseMod;
        std::optional<int64_t> indirectDefenseMod;
        std::optional<int64_t> minRangeMod;
        std::optional<int64_t> maxRangeMod;
        std::optional<int64_t> fuelUseMod;
        std::optional<int64_t> ammoUseMod;
        std::optional<int64_t> goodLuckMod;
        std::optional<int64_t> badLuckMod;
        std::optional<int64_t> movementMod;
        std::optional<int64_t> visionMod;
        std::optional<int64_t> terrainStarsMod;
        std::optional<int64_t> terrainStarsFlatMod;
        std::optional<int64_t> terrainStarsDefense;
        std::optional<int64_t> terrainStarsFirepower;
        std::optional<int64_t> terrainStarsFlatDefense;
        std::optional<int64_t> terrainStarsFlatFirepower;
        std::optional<int64_t> counterfireMod;
        std::optional<bool> counterFirst;
        std::optional<int64_t> captureRateMod;
        std::optional<int64_t> unitCostMod;
        std::optional<std::vector<std::string>> hiddenHitPoints;
        std::optional<std::vector<std::string>> luckPointsVisible;
        std::optional<std::vector<std::string>> hpPartVisible;
        std::optional<int64_t> firepowerFromFunds;
        std::optional<int64_t> defenseFromFunds;
        std::optional<int64_t> fundsFromDamage;
        std::optional<std::map<std::string, int64_t>> firepowerFromOwnedTerrain;
        std::optional<std::map<std::string, int64_t>> defenseFromOwnedTerrain;
        std::optional<std::map<std::string, int64_t>> visionVariantMods;
        std::optional<std::map<std::string, int64_t>> firepowerVariantMods;
        std::optional<std::map<std::string, int64_t>> defenseVariantMods;
        std::optional<int64_t> coMeterChargeFromDealtDamage;
        std::optional<int64_t> coMeterChargeFromReceivedDamage;
    };
    //Remember: BOOST_DESCRIBE_STRUCT only works for up to 53 members!
    BOOST_DESCRIBE_STRUCT(PassiveUnitEffect, (), (
        name,
        targets,
        unitTypeRequired,
        classificationRequired,
        terrainRequired,
        firepowerMod,
        defenseMod,
        indirectDefenseMod,
        minRangeMod,
        maxRangeMod,
        fuelUseMod,
        ammoUseMod,
        goodLuckMod,
        badLuckMod,
        movementMod,
        visionMod,
        terrainStarsMod,
        terrainStarsFlatMod,
        terrainStarsDefense,
        terrainStarsFirepower,
        terrainStarsFlatDefense,
        terrainStarsFlatFirepower,
        counterfireMod,
        counterFirst,
        captureRateMod,
        unitCostMod,
        hiddenHitPoints,
        luckPointsVisible,
        hpPartVisible,
        firepowerFromFunds,
        defenseFromFunds,
        fundsFromDamage,
        firepowerFromOwnedTerrain,
        defenseFromOwnedTerrain,
        visionVariantMods,
        firepowerVariantMods,
        defenseVariantMods,
        coMeterChargeFromDealtDamage,
        coMeterChargeFromReceivedDamage
    ))
    struct ActiveUnitEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> unitTypeRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<int64_t> hitPointMod;
        std::optional<int64_t> roundHitPoints;
        std::optional<int64_t> setFuel;
        std::optional<int64_t> setAmmo;
        std::optional<int64_t> addFuel;
        std::optional<int64_t> addAmmo;
        std::optional<int64_t> multiplyFuel;
        std::optional<int64_t> multiplyAmmo;
        std::optional<bool> makeActive;
        std::optional<int64_t> stunDuration;
        std::optional<int64_t> coChargeFactor;
    };
    BOOST_DESCRIBE_STRUCT(ActiveUnitEffect, (), (
        name,
        targets,
        unitTypeRequired,
        classificationRequired,
        terrainRequired,
        hitPointMod,
        roundHitPoints,
        setFuel,
        setAmmo,
        addFuel,
        addAmmo,
        multiplyFuel,
        multiplyAmmo,
        makeActive,
        stunDuration,
        coChargeFactor
    ))
    struct PassiveTerrainEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> affects;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<std::vector<std::string>> classificationRequired;
        std::optional<int64_t> incomeMod;
        std::optional<int64_t> incomeFlatMod;
        std::optional<std::vector<std::string>> buildListMod;
        std::optional<int64_t> repairMod;
        std::optional<bool> occludesVisionMod;
        std::optional<int64_t> visionModBoost;
        std::optional<int64_t> buildCostMod;
    };
    BOOST_DESCRIBE_STRUCT(PassiveTerrainEffect, (), (
        name,
        targets,
        affects,
        terrainRequired,
        classificationRequired,
        incomeMod,
        incomeFlatMod,
        buildListMod,
        repairMod,
        occludesVisionMod,
        visionModBoost,
        buildCostMod
    ))
    struct ActiveTerrainEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::vector<std::string>> affects;
        std::optional<std::vector<std::string>> terrainRequired;
        std::optional<std::string> unitSummonedName;
        std::optional<int64_t> unitSummonedInitialDamage;
        std::optional<bool> unitSummonedActive;
    };
    BOOST_DESCRIBE_STRUCT(ActiveTerrainEffect, (), (
        name,
        targets,
        affects,
        terrainRequired,
        unitSummonedName,
        unitSummonedInitialDamage,
        unitSummonedActive
    ))
    struct PassiveGlobalEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<std::string> variantMod;
        std::optional<std::map<std::string, int64_t>> variantHintMod;
        std::optional<std::string> movementClassVariantReplace;
        std::optional<std::string> movementClassVariantOverride;
        std::optional<int64_t> minimumVisionMod;
    };
    BOOST_DESCRIBE_STRUCT(PassiveGlobalEffect, (), (
        name,
        targets,
        variantMod,
        variantHintMod,
        movementClassVariantReplace,
        movementClassVariantOverride,
        minimumVisionMod
    ))
    struct ActiveGlobalEffect{
        std::string name;
        std::optional<std::vector<std::string>> targets;
        std::optional<int64_t> fundMod;
        std::optional<int64_t> fundFlatMod;
        std::optional<int64_t> powerBarMod;
        std::optional<int64_t> powerBarPerFunds;
        std::optional<int64_t> missileCount;
        std::optional<std::vector<std::string>> missileTargetMethod;
        std::optional<int64_t> missileDamage;
        std::optional<int64_t> missileAreaOfEffect;
        std::optional<int64_t> missileStunDuration;
        std::optional<int64_t> coChargeFactor;
    };
    BOOST_DESCRIBE_STRUCT(ActiveGlobalEffect, (), (
        name,
        targets,
        fundMod,
        fundFlatMod,
        powerBarMod,
        powerBarPerFunds,
        missileCount,
        missileTargetMethod,
        missileDamage,
        missileAreaOfEffect,
        missileStunDuration,
        coChargeFactor
    ))
}
