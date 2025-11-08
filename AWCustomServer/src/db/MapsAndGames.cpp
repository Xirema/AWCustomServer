#include <DBFunctions.h>
#include <SQLUtil.h>
#include <dbs/DBTablesCurrent.h>

#include <random>
#include <ranges>

namespace mysql = boost::mysql;
namespace db {
void createGame(std::optional<int64_t> gameId, int64_t mapId, std::map<std::string_view, sTypes::PlayerState> users,
                sTypes::SettingsState const& settings) {
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};
  constexpr std::string_view DELETE_STATEMENT = "delete from `{}`.`{}` where `{}` = ?";
  try {
    if (gameId) {
      std::map<std::string_view, std::string_view> tablesToWipe{
          {dbcurrent::TABLE_STATE_GAME, "ID"},          {dbcurrent::TABLE_STATE_GAME_PLAYERORDER, "GAME_ID"},
          {dbcurrent::TABLE_STATE_PLAYER, "GAME_ID"},   {dbcurrent::TABLE_STATE_PLAYER_BANNEDUNITS, "GAME_ID"},
          {dbcurrent::TABLE_STATE_SETTINGS, "GAME_ID"}, {dbcurrent::TABLE_STATE_SETTINGS_VARIANT, "GAME_ID"},
          {dbcurrent::TABLE_STATE_UNIT, "GAME_ID"},     {dbcurrent::TABLE_STATE_UNIT_TRANSPORT, "GAME_ID"},
          {dbcurrent::TABLE_STATE_TERRAIN, "GAME_ID"}};
      for (auto&& [tableName, gameIdColumn] : tablesToWipe) {
        mysql::results results;
        auto deleteStatement = session.connection.prepare_statement(std::format(DELETE_STATEMENT, dbcurrent::DB_STATE, tableName, gameIdColumn));
        session.connection.execute(deleteStatement.bind(*gameId), results);
      }
    }

    auto map = getMap(mapId);

    sTypes::GameState game{.id = 0, .day = 1, .playerTurn = 0, .active = true, .hash = 0};
    if (settings.variant.size() == 0) {
      game.variant = "normal";
    } else if (settings.variant.size() == 1) {
      game.variant = settings.variant.begin()->first;
    } else {
      std::minstd_rand engine{std::random_device{}()};
      auto weights = settings.variant | std::views::values | std::ranges::to<std::vector<int64_t>>();
      std::discrete_distribution<int64_t> variantDistribution{weights.begin(), weights.end()};
      auto variantChoice = variantDistribution(engine);
      auto variantIt = settings.variant.begin();
      std::advance(variantIt, variantChoice);
      game.variant = variantIt->first;
    }
    game.variantStart = game.variant;
    if (gameId) {
      game.id = *gameId;
    }
    auto gameMapper = [gameId](sTypes::GameState const& game) {
      auto ret = sqlutil::toParameterPack(game.day, game.playerTurn, game.variant, game.variantStart, game.active, game.hash, "en");
      if (gameId) {
        ret.emplace(ret.begin(), *gameId);
      }
      return ret;
    };
    auto newGameId = static_cast<int64_t>(sqlutil::doInsert(session, std::ranges::single_view(game), gameMapper,
                                                            dbcurrent::TABLE_STATE_GAME_COLUMNS | std::views::drop(gameId ? 0 : 1),
                                                            dbcurrent::DB_STATE, dbcurrent::TABLE_STATE_GAME)
                                              .last_insert_id());

    auto playerMapper = [newGameId](sTypes::PlayerState const& player) {
      return sqlutil::toParameterPack(newGameId, player.user, player.commanderName, player.funds, player.powerCharge, player.armyColor, player.alive,
                                      player.totalPowerUses, player.unitFacing, player.playerType, player.team, player.powerActive,
                                      player.powerActiveDay, player.incomeMultiplier, player.coMeterMultiplier);
    };
    std::map<std::string_view, uint64_t> playerSlotToIds;
    for (auto&& [playerSlot, playerState] : users) {
      auto playerId =
          sqlutil::doInsert(session, std::views::single(playerState), playerMapper, dbcurrent::TABLE_STATE_PLAYER_COLUMNS | std::views::drop(1),
                            dbcurrent::DB_STATE, dbcurrent::TABLE_STATE_PLAYER)
              .last_insert_id();
      playerSlotToIds.emplace(playerSlot, playerId);
    }

    auto playerSlotIdMapper = [&playerSlotToIds](std::string_view playerSlot) {
      auto it = playerSlotToIds.find(playerSlot);
      if (it == playerSlotToIds.end()) {
        throw std::runtime_error(std::format("Player Slot mismatch: tried to assign '{}'", playerSlot));
      }
      return it->second;
    };
    game.playerOrder = map.playerSlots | std::views::transform(playerSlotIdMapper) | std::ranges::to<std::vector<int64_t>>();

    auto playerOrderMapper = [newGameId](auto&& tuple) {
      auto const& [order, playerId] = tuple;
      return sqlutil::toParameterPack(newGameId, playerId, order);
    };
    sqlutil::doInsert(session, game.playerOrder | std::views::enumerate, playerOrderMapper,
                      dbcurrent::TABLE_STATE_GAME_PLAYERORDER_COLUMNS | std::views::drop(1), dbcurrent::DB_STATE,
                      dbcurrent::TABLE_STATE_GAME_PLAYERORDER);
    auto initialUnitMapper = [&playerSlotToIds](dTypes::InitialUnit const& initialUnit) {
      sTypes::UnitState ret;
      ret.name = initialUnit.unitName;
      ret.x = initialUnit.x;
      ret.y = initialUnit.y;
      // TODO: We need to do a lookup on the mod and figure out the correct values!
      ret.ammo = initialUnit.startingAmmo.value_or(9);
      ret.fuel = initialUnit.startingFuel.value_or(99);
      ret.active = true;
      ret.stunned = initialUnit.startingStunned;
      if (initialUnit.playerSlot) {
        ret.owner = playerSlotToIds.at(*initialUnit.playerSlot);
      }
      ret.hitPoints = initialUnit.startingHitPoints.value_or(100);
      ret.stealthed = initialUnit.startingStealthed;
      if (initialUnit.startingLuck) {
        auto luck = *initialUnit.startingLuck;
        ret.currentGoodLuck = luck > 0 ? luck : 0;
        ret.currentBadLuck = luck < 0 ? -luck : 0;
      }
      // else {
      // TODO: generate luck values!
      //}
      // TODO: Transporting!
      return ret;
    };
    auto units = map.initialUnits | std::views::transform(initialUnitMapper) | std::ranges::to<std::vector>();
    auto unitMapper = [newGameId](sTypes::UnitState const& unit) {
      return sqlutil::toParameterPack(newGameId, unit.x, unit.y, unit.name, unit.ammo, unit.fuel, unit.active, unit.stunned, unit.owner,
                                      unit.hitPoints, unit.stealthed, unit.currentGoodLuck, unit.currentBadLuck, nullptr, nullptr);
    };
    sqlutil::doInsert(session, units, unitMapper, dbcurrent::TABLE_STATE_UNIT_COLUMNS | std::views::drop(1), dbcurrent::DB_STATE,
                      dbcurrent::TABLE_STATE_UNIT);
    auto initialTerrainMapper = [&playerSlotToIds](dTypes::InitialTerrain const& initialTerrain) {
      sTypes::TerrainState ret;
      ret.name = initialTerrain.terrainName;
      ret.x = initialTerrain.x;
      ret.y = initialTerrain.y;
      ret.orientation = initialTerrain.orientation;
      ret.capturePoints = initialTerrain.startingCapturePoints;
      if (initialTerrain.playerSlot) {
        ret.owner = playerSlotToIds.at(initialTerrain.playerSlot.value());
      }
      // TODO: Initialize Activation Count!
      // TODO: Check mod for initial Hit Points fallback!
      ret.hitPoints = initialTerrain.startingHitPoints;
      return ret;
    };
    auto terrains = map.initialTerrains | std::views::transform(initialTerrainMapper) | std::ranges::to<std::vector>();
    auto terrainMapper = [newGameId](sTypes::TerrainState const& terrain) {
      return sqlutil::toParameterPack(newGameId, terrain.x, terrain.y, terrain.name, terrain.orientation, terrain.capturePoints,
                                      terrain.activationCount, terrain.owner, terrain.hitPoints);
    };
    sqlutil::doInsert(session, terrains, terrainMapper, dbcurrent::TABLE_STATE_TERRAIN_COLUMNS | std::views::drop(1), dbcurrent::DB_STATE,
                      dbcurrent::TABLE_STATE_TERRAIN);
    auto settingsMapper = [newGameId](sTypes::SettingsState const& settings) {
      return sqlutil::toParameterPack(newGameId, settings.startingFunds, settings.incomeMultiplier, settings.fogOfWar, settings.coPowers, settings.teams,
                                      settings.modId, settings.coMeterSize, settings.coMeterMultiplier, settings.unitLimit, settings.captureLimit,
                                      settings.dayLimit);
    };
    sqlutil::doInsert(session, std::views::single(settings), settingsMapper, dbcurrent::TABLE_STATE_SETTINGS_COLUMNS | std::views::drop(1),
                      dbcurrent::DB_STATE, dbcurrent::TABLE_STATE_SETTINGS);
    auto settingsVariantMapper = [newGameId](auto const& settingsVariant) {
      auto [variant, weight] = settingsVariant;
      return sqlutil::toParameterPack(newGameId, variant, weight);
    };
    sqlutil::doInsert(session, settings.variant, settingsVariantMapper, dbcurrent::TABLE_STATE_SETTINGS_VARIANT_COLUMNS | std::views::drop(1),
                      dbcurrent::DB_STATE, dbcurrent::TABLE_STATE_SETTINGS_VARIANT);
    transaction.commit();
  } catch (mysql::error_with_diagnostics const& e) {
    sqlutil::printError(e);
  }
}

dTypes::MapDefinition getMap(int64_t mapId) {
  sqlutil::Session session;
  auto rowMapper = [](mysql::row_view row) {
    dTypes::MapDefinition ret;
    using namespace sqlutil;
    set(ret.metadata.id, row.at(0));
    set(ret.metadata.name, row.at(1));
    set(ret.metadata.version, row.at(2));
    set(ret.metadata.expired, row.at(3));
    set(ret.metadata.description, row.at(4));
    set(ret.metadata.modName, row.at(5));
    set(ret.metadata.modVersion, row.at(6));
    return ret;
  };
  sqlutil::ColumnFilter filter{dbcurrent::COLUMN_DATA_MAP_ID, mapId};
  auto definitions = sqlutil::doSelect(session, rowMapper, dbcurrent::TABLE_DATA_MAP_DEFINITION_COLUMNS, dbcurrent::DB_DATA,
                                       dbcurrent::TABLE_DATA_MAP_DEFINITION, {filter});
  if (definitions.size() != 1) {
    throw std::runtime_error(std::format("Unable to find Map with id {}", mapId));
  }
  auto playerSlotMapper = [](mysql::row_view row) {
    std::string ret;
    using namespace sqlutil;
    set(ret, row.at(2));
    return ret;
  };
  auto& definition = definitions.front();
  filter.first = dbcurrent::COLUMN_DATA_MAP_PLAYERSLOT_MAP_ID;
  definition.playerSlots =
      sqlutil::doSelect(session, playerSlotMapper, dbcurrent::TABLE_DATA_MAP_PLAYERSLOT_COLUMNS, dbcurrent::DB_DATA,
                        dbcurrent::TABLE_DATA_MAP_PLAYERSLOT, {filter}, sqlutil::ColumnOrder(dbcurrent::COLUMN_DATA_MAP_PLAYERSLOT_ORDER, true));

  auto unitMapper = [](mysql::row_view row) {
    dTypes::InitialUnit unit;
    using namespace sqlutil;
    set(unit.unitName, row.at(2));
    set(unit.x, row.at(3));
    set(unit.y, row.at(4));
    set(unit.playerSlot, row.at(5));
    set(unit.startingHitPoints, row.at(6));
    set(unit.startingAmmo, row.at(7));
    set(unit.startingFuel, row.at(8));
    set(unit.startingStunned, row.at(9));
    set(unit.startingStealthed, row.at(10));
    set(unit.startingLuck, row.at(11));
    set(unit.transportedBy, row.at(12));
    return unit;
  };
  definition.initialUnits = sqlutil::doSelect(session, unitMapper, dbcurrent::TABLE_DATA_MAP_UNIT_COLUMNS, dbcurrent::DB_DATA,
                                              dbcurrent::TABLE_DATA_MAP_INITIAL_UNIT, {filter});
  auto terrainMapper = [](mysql::row_view row) {
    dTypes::InitialTerrain ret;
    using namespace sqlutil;
    set(ret.terrainName, row.at(2));
    set(ret.x, row.at(3));
    set(ret.y, row.at(4));
    set(ret.orientation, row.at(5));
    set(ret.playerSlot, row.at(6));
    set(ret.startingHitPoints, row.at(7));
    set(ret.startingCapturePoints, row.at(8));
    return ret;
  };
  definition.initialTerrains = sqlutil::doSelect(session, terrainMapper, dbcurrent::TABLE_DATA_MAP_TERRAIN_COLUMNS, dbcurrent::DB_DATA,
                                                 dbcurrent::TABLE_DATA_MAP_INITIAL_TERRAIN, {filter});
  return definition;
}
std::vector<dTypes::MapMetadata> findMaps(std::string_view name, std::optional<std::string_view> version, std::optional<int64_t> modId) { return {}; }

uint64_t uploadMap(dTypes::MapDefinition const& mapData) {
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};

  try {
    constexpr std::string_view OLD_MAP_EXPIRE = "update `{}`.`{}` set `{}` = current_timestamp where `{}` = ? and `{}` = ?";
    auto updateExpired = session.connection.prepare_statement(
        std::format(OLD_MAP_EXPIRE, dbcurrent::DB_DATA, dbcurrent::TABLE_DATA_MAP_DEFINITION, dbcurrent::COLUMN_DATA_MAP_EXPIRED.columnName,
                    dbcurrent::COLUMN_DATA_MAP_NAME.columnName, dbcurrent::COLUMN_DATA_MAP_VERSION.columnName));
    mysql::results updateResults;
    session.connection.execute(updateExpired.bind(mapData.metadata.name, mapData.metadata.version), updateResults);
    auto mapDefinitionMapper = [](dTypes::MapDefinition const& mapData) {
      return sqlutil::toParameterPack(mapData.metadata.name, mapData.metadata.version, nullptr, mapData.metadata.description, mapData.metadata.modName,
                                      mapData.metadata.modVersion);
    };
    auto results = sqlutil::doInsert(session, std::views::single(mapData), mapDefinitionMapper,
                                     dbcurrent::TABLE_DATA_MAP_DEFINITION_COLUMNS | std::views::drop(1), dbcurrent::DB_DATA,
                                     dbcurrent::TABLE_DATA_MAP_DEFINITION);
    auto mapId = results.last_insert_id();

    std::map<std::ptrdiff_t, uint64_t> unitIndexToId;
    auto unitMapper = [mapId](dTypes::InitialUnit const& unit) {
      return sqlutil::toParameterPack(mapId, unit.unitName, unit.x, unit.y, unit.playerSlot, unit.startingHitPoints, unit.startingAmmo,
                                      unit.startingFuel, unit.startingStunned, unit.startingStealthed, unit.startingLuck, -1);
    };
    constexpr auto UNIT_COLUMNS = dbcurrent::TABLE_DATA_MAP_UNIT_COLUMNS | std::views::drop(1);
    for (auto&& [index, unit] : mapData.initialUnits | std::views::enumerate) {
      auto unitResults =
          sqlutil::doInsert(session, std::views::single(unit), unitMapper, UNIT_COLUMNS, dbcurrent::DB_DATA, dbcurrent::TABLE_DATA_MAP_INITIAL_UNIT);
      unitIndexToId.emplace(index, unitResults.last_insert_id());
    }

    constexpr std::string_view UPDATE_UNIT_TRANSPORTED_BY = "update `{}`.`{}` set `{}` = ? where `{}` = ?";
    for (auto&& [index, unit] : mapData.initialUnits | std::views::enumerate) {
      if (!unit.transportedBy) {
        continue;
      }
      auto unitId = unitIndexToId.at(index);
      auto transportedByIndex = unit.transportedBy.value();
      auto transportedByIdIterator = unitIndexToId.find(transportedByIndex);
      if (transportedByIdIterator == unitIndexToId.end()) {
        throw std::runtime_error(std::format("Map Unit List references transport unit that does not exist: {}", transportedByIndex));
      }
      auto transportedById = transportedByIdIterator->second;
      auto updateStatement = session.connection.prepare_statement(
          std::format(UPDATE_UNIT_TRANSPORTED_BY, dbcurrent::DB_DATA, dbcurrent::TABLE_DATA_MAP_INITIAL_UNIT,
                      dbcurrent::COLUMN_DATA_MAP_UNIT_TRANSPORTED_BY.columnName, dbcurrent::COLUMN_DATA_MAP_UNIT_ID.columnName));
      mysql::results results;
      session.connection.execute(updateStatement.bind(unitId, transportedById), results);

      // Note: we are NOT validating for things like whether the unit is allowed to transport units/of that type/of that quantity
      // This allows mapmakers to do weird things like creating towers of units recursively transported, or else stuffing a hundred units
      // inside a unit that otherwise cannot transport anything. The game is supposed to gracefully handle this, despite the weirdness.
      // We're also not validating for transport loops, although consider this a TODO because we might eventually need to.
    }

    auto terrainMapper = [mapId](dTypes::InitialTerrain const& terrain) {
      return sqlutil::toParameterPack(mapId, terrain.terrainName, terrain.x, terrain.y, terrain.orientation, terrain.playerSlot,
                                      terrain.startingHitPoints, terrain.startingCapturePoints);
    };
    sqlutil::doInsert(session, mapData.initialTerrains, terrainMapper, dbcurrent::TABLE_DATA_MAP_TERRAIN_COLUMNS | std::views::drop(1),
                      dbcurrent::DB_DATA, dbcurrent::TABLE_DATA_MAP_INITIAL_TERRAIN);

    auto playerSlotMapper = [mapId](auto&& tuple) {
      auto&& [order, playerSlot] = tuple;
      return sqlutil::toParameterPack(mapId, playerSlot, order);
    };
    sqlutil::doInsert(session, mapData.playerSlots | std::views::enumerate, playerSlotMapper,
                      dbcurrent::TABLE_DATA_MAP_PLAYERSLOT_COLUMNS | std::views::drop(1), dbcurrent::DB_DATA, dbcurrent::TABLE_DATA_MAP_PLAYERSLOT);
    transaction.commit();
    return mapId;
  } catch (mysql::error_with_diagnostics const& e) {
    sqlutil::printError(e);
    throw e;
  }
}
}  // namespace db