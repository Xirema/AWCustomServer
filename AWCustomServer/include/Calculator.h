#pragma once
#include<jss/Game.h>
#include<jss/ModMetadata.h>

namespace calc {
  int64_t calculateUnitMovementRange(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData
  );

  std::vector<dTypes::PassiveUnitEffect const*>
  getAllPassiveUnitEffects(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData,
    std::function<bool(dTypes::PassiveUnitEffect const*)> filter = [](auto && effect) {return true;}
  );

  bool unitMatchesEffect(
    dTypes::UnitType const& unitType, 
    std::string_view terrainName, 
    dTypes::PassiveUnitEffect const& effect
  );
}