#pragma once
#include "GameState.h"
#include "PlayerState.h"
#include "UnitState.h"
#include "TerrainState.h"
#include "SettingsState.h"
#include<Coord.h>

namespace game {
  using coord::Coord;
  struct Game {
    sTypes::GameState gameState;
    std::unordered_map<int64_t, sTypes::PlayerState> playersById;
    std::unordered_map<int64_t, sTypes::UnitState> unitsById;
    std::unordered_map<int64_t, sTypes::TerrainState> terrainsById;
    sTypes::SettingsState settings;
    std::unordered_map<Coord, sTypes::TerrainState*> terrainsByCoordinate;
    std::unordered_map<Coord, sTypes::UnitState*> unitsByCoordinate;
  };
}