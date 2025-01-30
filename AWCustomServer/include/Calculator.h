#pragma once
#include<jss/Game.h>
#include<jss/ModMetadata.h>

namespace calc {
  int64_t calculateUnitMovementRange(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData
  );
  int64_t calculateUnitVisionRange(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData
  );

  sTypes::PlayerState const* getUnitOwner(
    sTypes::UnitState const& unit, 
    game::Game const& game
  );

  std::string getCurrentVariant(
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

  std::vector<dTypes::PassiveTerrainEffect const*>
  getAllPassiveTerrainEffects(
    sTypes::TerrainState const& terrain,
    game::Game const& game,
    dTypes::ModData const& modData,
    std::function<bool(dTypes::PassiveTerrainEffect const*)> filter = [](auto && effect) {return true;}
  );

  std::vector<dTypes::PassiveGlobalEffect const*>
  getAllPassiveGlobalEffects(
    game::Game const& game,
    dTypes::ModData const& modData,
    std::function<bool(dTypes::PassiveGlobalEffect const*)> filter = [](auto && effect) {return true;}
  );

  bool unitMatchesEffect(
    dTypes::UnitType const& unitType, 
    std::string_view terrainName, 
    dTypes::PassiveUnitEffect const& effect
  );

  bool terrainMatchesEffect(
    dTypes::TerrainType const& terrainType,
    dTypes::PassiveTerrainEffect const& effect
  );
}