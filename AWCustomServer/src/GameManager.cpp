#include<GameManager.h>
#include<future>
#include<Properties.h>
#include<DBFunctions.h>

namespace game {
  GameManager& GameManager::instance() {
    static GameManager manager;
    return manager;
  }

  GamePtr GameManager::getGame(int64_t id) {
    std::unique_lock lock{cacheMutex};
    GamePtr ret;
    auto it = cachedGames.find(id);
    if(it == cachedGames.end()) {
      ret = std::make_shared<Cached<Game>>();
      auto & game = ret->object;
      auto gameStateFuture = std::async(db::get_gamestate, id);
      auto unitsFuture = std::async(db::get_unitstates, id);
      auto terrainsFuture = std::async(db::get_terrainstates, id);
      auto playersFuture = std::async(db::get_playerstates, id);
      auto settingsFuture = std::async(db::get_settingstate, id);
      game.gameState = gameStateFuture.get();
      for(auto const& unit : unitsFuture.get()) {
        auto & newUnit = game.unitsById[unit.id] = unit;
        game.unitsByCoordinate[newUnit] = &newUnit;
      }
      for(auto const& terrain : terrainsFuture.get()) {
        auto & newTerrain = game.terrainsById[terrain.id] = terrain;
        game.terrainsByCoordinate[newTerrain] = &newTerrain;
      }
      for(auto const& player : playersFuture.get()) {
        game.playersById[player.id] = player;
      }
      game.settings = settingsFuture.get();
    } else {
      ret = it->second;
    }
    auto const& properties = properties::Properties::instance();
    auto expiry = std::chrono::minutes{properties.getInt("gameCacheExpiration", 24 * 60)};
    ret->expiration = std::chrono::system_clock::now() + expiry;
    return ret;
  }

  ModPtr GameManager::getMod(int64_t id) {
    std::unique_lock lock{cacheMutex};
    ModPtr ret;
    auto it = cachedMods.find(id);
    if(it == cachedMods.end()) {
      ret = std::make_shared<Cached<dTypes::ModData>>();
      ret->object = db::get_mod(id);
    } else {
      ret = it->second;
    }
    auto const& properties = properties::Properties::instance();
    auto expiry = std::chrono::minutes{properties.getInt("gameCacheExpiration", 24 * 60)};
    ret->expiration = std::chrono::system_clock::now() + expiry;
    return ret;
  }

  void GameManager::cleanup() {
    std::unique_lock lock{cacheMutex};
    auto now = std::chrono::system_clock::now();
    std::erase_if(cachedGames, [now](auto const& entry) {
      auto const& [key, game] = entry;
      return game->expiration < now;
    });
    std::erase_if(cachedMods, [now](auto const& entry) {
      auto const& [key, mod] = entry;
      return mod->expiration < now;
    });
  }
}