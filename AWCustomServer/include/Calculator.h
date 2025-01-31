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
  int64_t calculateUnitFirepower(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData,
    bool attacking = true
  );
  int64_t calculateUnitDefense(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData,
    bool attackerIndirect = false
  );

  sTypes::PlayerState const* getUnitOwner(
    sTypes::UnitState const& unit, 
    game::Game const& game
  );

  std::string getCurrentVariant(
    game::Game const& game,
    dTypes::ModData const& modData
  );

  std::vector<std::pair<dTypes::PassiveUnitEffect const*, sTypes::PlayerState const*>>
  getAllPassiveUnitEffects(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData,
    std::function<bool(dTypes::PassiveUnitEffect const*)> filter = [](auto && effect) {return true;}
  );

  std::vector<std::pair<dTypes::PassiveTerrainEffect const*, sTypes::PlayerState const*>>
  getAllPassiveTerrainEffects(
    sTypes::TerrainState const& terrain,
    game::Game const& game,
    dTypes::ModData const& modData,
    std::function<bool(dTypes::PassiveTerrainEffect const*)> filter = [](auto && effect) {return true;}
  );

  std::vector<std::pair<dTypes::PassiveGlobalEffect const*, sTypes::PlayerState const*>>
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

  int64_t countTerrainsOwnedByPlayer(
    int64_t playerId,
    std::vector<std::string> const& terrainNames,
    game::Game const& game,
    dTypes::ModData const& modData
  );

  int64_t calculateUnitTerrainStars(
    sTypes::UnitState const& unit,
    game::Game const& game,
    dTypes::ModData const& modData
  );

  constexpr int64_t flatHitPoints(int64_t realHitPoints) {
    return static_cast<int64_t>(std::ceil(realHitPoints / 10.)) * 10;
  }
}