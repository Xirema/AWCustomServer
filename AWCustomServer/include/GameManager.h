#pragma once
#include<RestFunctions.h>
#include<mutex>
#include<Coord.h>
#include<jss/Game.h>
#include<Id.h>
#include<chrono>

namespace game {
  template<typename CachedObject>
  struct Cached {
    CachedObject object;
    std::chrono::system_clock::time_point expiration;
    std::mutex gameMutex;
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
