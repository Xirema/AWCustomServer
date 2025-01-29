#pragma once
#include<RestFunctions.h>
#include<mutex>
#include<Coord.h>
#include<Id.h>
#include<chrono>

namespace game {
  using coord::Coord;
  template<typename CachedObject>
  struct Cached {
    CachedObject object;
    std::chrono::system_clock::time_point expiration;
    std::mutex gameMutex;
  };
  struct Game {
    sTypes::GameState gameState;
    std::unordered_map<int64_t, sTypes::PlayerState> playersById;
    std::unordered_map<int64_t, sTypes::UnitState> unitsById;
    std::unordered_map<int64_t, sTypes::TerrainState> terrainsById;
    sTypes::SettingsState settings;
    std::unordered_map<Coord, sTypes::TerrainState*> terrainsByCoordinate;
    std::unordered_map<Coord, sTypes::UnitState*> unitsByCoordinate;
  };
  using GamePtr = std::shared_ptr<Cached<Game>>;
  using ModPtr = std::shared_ptr<Cached<dTypes::ModData>>;
  class GameManager {
    GameManager() = default;
    std::mutex cacheMutex;
    std::unordered_map<int64_t, GamePtr> cachedGames;
    std::unordered_map<int64_t, ModPtr> cachedMods;
  public:
    static GameManager& instance();
    GamePtr getGame(std::string_view id);
    ModPtr getMod(std::string_view id);
    void cleanup();
  };
}
