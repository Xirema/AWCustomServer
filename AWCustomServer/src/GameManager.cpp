#include<GameManager.h>
#include<future>
#include<Properties.h>

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
      auto gameStateFuture = std::async([id]{return db::get_gamestate(id);});
      auto unitsFuture = std::async([id]{return db::get_unitstates(id);});
      auto terrainsFuture = std::async([id]{return db::get_terrainstates(id);});
      auto playersFuture = std::async([id]{return db::get_playerstates(id);});
      auto settingsFuture = std::async([id]{return db::get_settingstate(id);});
      game.gameState = gameStateFuture.get();
      for(auto const& unit : unitsFuture.get()) {
        auto uid = i64(unit.id);
        auto & newUnit = game.unitsById[uid] = unit;
        game.unitsByCoordinate[Coord{newUnit.x, newUnit.y}] = &newUnit;
      }
      for(auto const& terrain : terrainsFuture.get()) {
        auto tid = i64(terrain.id);
        auto & newTerrain = game.terrainsById[tid] = terrain;
        game.terrainsByCoordinate[Coord{newTerrain.x, newTerrain.y}] = &newTerrain;
      }
      for(auto const& player : playersFuture.get()) {
        auto pid = i64(player.id);
        game.playersById[pid] = player;
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
      auto & mod = ret->object;
      auto metadataFuture = std::async([id]{return db::get_mod_metadata(id, {}, {});});
      auto unitsFuture = std::async([id]{return db::get_units(id, {});});
      auto weaponsFuture = std::async([id]{return db::get_weapons(id, {});});
      auto terrainsFuture = std::async([id]{return db::get_terrains(id, {});});
      auto commandersFuture = std::async([id]{return db::get_commanders(id, {});});
      auto movementsFuture = std::async([id]{return db::get_movements(id, {});});
      auto movementRulesFuture = std::async([id]{return db::get_movement_rules(id, {});});
      auto playersFuture = std::async([id]{return db::get_players(id, {});});
      auto puesFuture = std::async([id]{return db::get_pues(id, {});});
      auto auesFuture = std::async([id]{return db::get_aues(id, {});});
      auto ptesFuture = std::async([id]{return db::get_ptes(id, {});});
      auto atesFuture = std::async([id]{return db::get_ates(id, {});});
      auto pgesFuture = std::async([id]{return db::get_pges(id, {});});
      auto agesFuture = std::async([id]{return db::get_ages(id, {});});
      auto settingsFuture = std::async([id]{return db::get_settings(id, {});});
      mod.modMetadata = std::move(metadataFuture.get());
      mod.units = std::move(unitsFuture.get());
      mod.weapons = std::move(weaponsFuture.get());
      mod.terrains = std::move(terrainsFuture.get());
      mod.commanders = std::move(commandersFuture.get());
      mod.movements = std::move(movementsFuture.get());
      mod.movementRules = std::move(movementRulesFuture.get());
      mod.players = std::move(playersFuture.get());
      mod.passiveUnitEffects = std::move(puesFuture.get());
      mod.activeUnitEffects = std::move(auesFuture.get());
      mod.passiveTerrainEffects = std::move(ptesFuture.get());
      mod.activeTerrainEffects = std::move(atesFuture.get());
      mod.passiveGlobalEffects = std::move(pgesFuture.get());
      mod.activeGlobalEffects = std::move(agesFuture.get());
      mod.defaultSettings = std::move(settingsFuture.get());
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