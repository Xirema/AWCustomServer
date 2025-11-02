#include <GameManager.h>
#include <SQLUtil.h>
#include <dbs/DBTablesCurrent.h>

#include <print>
#include <random>
#include <ranges>
namespace mysql = boost::mysql;
int main() {
  constexpr std::string_view deleteStatement = "delete from `{}`.`{}` where `{}` = -1";
  constexpr auto dbstate = dbcurrent::DB_STATE;
  // game::GameManager & manager = game::GameManager::instance();
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};
  mysql::results results;
  std::map<std::string_view, std::string_view> tablesToWipe{
      {dbcurrent::TABLE_STATE_GAME, "ID"},          {dbcurrent::TABLE_STATE_GAME_PLAYERORDER, "GAME_ID"},
      {dbcurrent::TABLE_STATE_PLAYER, "GAME_ID"},   {dbcurrent::TABLE_STATE_PLAYER_BANNEDUNITS, "GAME_ID"},
      {dbcurrent::TABLE_STATE_SETTINGS, "GAME_ID"}, {dbcurrent::TABLE_STATE_SETTINGS_VARIANT, "GAME_ID"},
      {dbcurrent::TABLE_STATE_UNIT, "GAME_ID"},     {dbcurrent::TABLE_STATE_UNIT_TRANSPORT, "GAME_ID"},
      {dbcurrent::TABLE_STATE_TERRAIN, "GAME_ID"}};

  sTypes::GameState testGame{
      .id = -1, .day = 1, .playerTurn = 1, .playerOrder = {1, 2, 3, 4, 5}, .variant = "normal", .variantStart = "normal", .active = true, .hash = 0};
  std::vector<sTypes::PlayerState> players{
      sTypes::PlayerState{.id = 1,
                          .user = 11,
                          .commanderName = "max",
                          .funds = 0,
                          .powerCharge = 0,
                          .armyColor = "os",
                          .alive = true,
                          .totalPowerUses = 0,
                          .unitFacing = 0,
                          .playerType = "normal",
                          .powerActive = "scop",
                          .powerActiveDay = 1},
      sTypes::PlayerState{.id = 2,
                          .user = 12,
                          .commanderName = "olaf",
                          .funds = 0,
                          .powerCharge = 0,
                          .armyColor = "bm",
                          .alive = true,
                          .totalPowerUses = 0,
                          .unitFacing = 1,
                          .playerType = "normal"},
      sTypes::PlayerState{.id = 3,
                          .user = 13,
                          .commanderName = "sturm",
                          .funds = 0,
                          .powerCharge = 0,
                          .armyColor = "bh",
                          .alive = true,
                          .totalPowerUses = 0,
                          .unitFacing = 0,
                          .playerType = "normal"},
      sTypes::PlayerState{.id = 4,
                          .user = 14,
                          .commanderName = "kindle",
                          .funds = 0,
                          .powerCharge = 0,
                          .armyColor = "rf",
                          .alive = true,
                          .totalPowerUses = 0,
                          .unitFacing = 1,
                          .playerType = "normal"},
      sTypes::PlayerState{.id = 5,
                          .user = 15,
                          .commanderName = "grit",
                          .funds = 0,
                          .powerCharge = 0,
                          .armyColor = "ci",
                          .alive = true,
                          .totalPowerUses = 0,
                          .unitFacing = 0,
                          .playerType = "normal"},
  };
  std::vector<sTypes::TerrainState> terrains;
  std::vector<sTypes::UnitState> units;
  std::vector<size_t> terrainWeights{10, 3, 1, 3};
  std::vector<std::string_view> terrainNames{"plains", "forest", "mountain", "road"};
  std::vector<size_t> unitWeights{10, 4, 3, 3, 1};
  std::map<std::string_view, std::vector<std::string_view>> coUnitDistributions{
      {"max", {"tank", "infantry", "rocket", "battle_copter", "fighter"}},
      {"olaf", {"infantry", "tank", "rocket", "battle_copter", "fighter"}},
      {"sturm", {"rocket", "tank", "infantry", "recon", "missile"}},
      {"kindle", {"infantry", "tank", "rocket", "battle_copter", "fighter"}},
      {"grit", {"rocket", "artillery", "missile", "infantry", "tank"}}};
  std::discrete_distribution<size_t> terrainDist{terrainWeights.begin(), terrainWeights.end()};
  std::discrete_distribution<size_t> unitDist{unitWeights.begin(), unitWeights.end()};
  std::uniform_int_distribution<size_t> playerDist{0, 4};
  std::uniform_real_distribution<float> unitSpawnDist{0, 1};
  constexpr float UNIT_SPAWN_CHANCE = 0.1f;
  std::minstd_rand engine{std::random_device{}()};
  for (auto const& [index, coord] :
       std::ranges::cartesian_product_view(std::ranges::iota_view(0, 50), std::ranges::iota_view(0, 50)) | std::views::enumerate) {
    auto const& [y, x] = coord;
    auto terrainType = terrainNames[terrainDist(engine)];
    terrains.emplace_back() = sTypes::TerrainState{.id = index, .x = x, .y = y, .name = std::string{terrainType}};
    if (UNIT_SPAWN_CHANCE < unitSpawnDist(engine)) {
      continue;
    }
    auto const& unitPlayer = players[playerDist(engine)];
    auto const& unitList = coUnitDistributions.at(unitPlayer.commanderName);
    auto unitName = unitList[unitDist(engine)];
    units.emplace_back() = sTypes::UnitState{.id = index,
                                             .x = x,
                                             .y = y,
                                             .name = std::string{unitName},
                                             .ammo = 9,
                                             .fuel = 99,
                                             .active = true,
                                             .owner = unitPlayer.id,
                                             .hitPoints = 100,
                                             .currentGoodLuck = unitSpawnDist(engine),
                                             .currentBadLuck = unitSpawnDist(engine)};
  }

  sTypes::SettingsState settings{.id = 0,
                                 .startingFunds = 0,
                                 .incomeMultiplier = 100,
                                 .fogOfWar = false,
                                 .variant = {{"normal", 100l}},
                                 .coPowers = true,
                                 .teams = false,
                                 .modId = 1,
                                 .coMeterSize = 9000,
                                 .coMeterMultiplier = 100};

  try {
    for (auto const& [tableName, idColumn] : tablesToWipe) {
      session.connection.execute(std::format(deleteStatement, dbstate, tableName, idColumn), results);
    }
    // GAME STATE
    auto gameInsertColumns = sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_GAME_COLUMNS | std::ranges::to<std::vector<db::Column>>());
    auto gameParameters = sqlutil::toParameterPack(testGame.id, testGame.day, testGame.playerTurn, testGame.variant, testGame.variantStart,
                                                   testGame.active, testGame.hash, "en");
    auto [gameValuesString, gameParametersFlattened] = sqlutil::createInsertValues({gameParameters});
    auto gameInsertStatement =
        session.connection.prepare_statement(std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_GAME, gameInsertColumns, gameValuesString));
    session.connection.execute(gameInsertStatement.bind(gameParametersFlattened.begin(), gameParametersFlattened.end()), results);

    auto gamePlayerOrderInsertColumns = sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_GAME_PLAYERORDER_COLUMNS | std::views::drop(1) |
                                                                     std::ranges::to<std::vector<db::Column>>());
    auto gamePlayerOrderParameters = testGame.playerOrder | std::views::enumerate | std::views::transform([](auto&& tuple) {
                                       auto const& [order, playerId] = tuple;
                                       return sqlutil::toParameterPack(-1, playerId, order);
                                     }) |
                                     std::ranges::to<std::vector<sqlutil::ParameterPack>>();
    auto [gamePlayerOrderValuesString, gamePlayerOrderParametersFlattened] = sqlutil::createInsertValues(gamePlayerOrderParameters);
    auto gamePlayerOrderInsertStatement = session.connection.prepare_statement(
        std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_GAME_PLAYERORDER, gamePlayerOrderInsertColumns, gamePlayerOrderValuesString));
    session.connection.execute(
        gamePlayerOrderInsertStatement.bind(gamePlayerOrderParametersFlattened.begin(), gamePlayerOrderParametersFlattened.end()), results);

    // UNITS
    auto unitInsertColumns =
        sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_UNIT_COLUMNS | std::views::drop(1) | std::ranges::to<std::vector<db::Column>>());
    auto unitParameters = units | std::views::transform([](sTypes::UnitState const& unit) {
                            return sqlutil::toParameterPack(-1, unit.x, unit.y, unit.name, unit.ammo, unit.fuel, unit.active, unit.stunned,
                                                            unit.owner, unit.hitPoints, unit.stealthed, unit.currentGoodLuck, unit.currentBadLuck);
                          }) |
                          std::ranges::to<std::vector<sqlutil::ParameterPack>>();
    auto [unitValuesString, unitParametersFlattened] = sqlutil::createInsertValues(unitParameters);
    auto unitInsertStatement =
        session.connection.prepare_statement(std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_UNIT, unitInsertColumns, unitValuesString));
    session.connection.execute(unitInsertStatement.bind(unitParametersFlattened.begin(), unitParametersFlattened.end()), results);

    // TERRAIN
    auto terrainInsertColumns =
        sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_TERRAIN_COLUMNS | std::views::drop(1) | std::ranges::to<std::vector<db::Column>>());
    auto terrainParameters = terrains | std::views::transform([](sTypes::TerrainState const& terrain) {
                               return sqlutil::toParameterPack(-1, terrain.x, terrain.y, terrain.name, terrain.orientation, terrain.capturePoints,
                                                               terrain.activationCount, terrain.owner, terrain.hitPoints);
                             }) |
                             std::ranges::to<std::vector<sqlutil::ParameterPack>>();
    auto [terrainValuesString, terrainParametersFlattened] = sqlutil::createInsertValues(terrainParameters);
    auto terrainInsertStatement = session.connection.prepare_statement(
        std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_TERRAIN, terrainInsertColumns, terrainValuesString));
    session.connection.execute(terrainInsertStatement.bind(terrainParametersFlattened.begin(), terrainParametersFlattened.end()), results);

    // PLAYERS
    auto playerInsertColumns = sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_PLAYER_COLUMNS | std::ranges::to<std::vector<db::Column>>());
    auto playerParameters =
        players | std::views::transform([](sTypes::PlayerState const& player) {
          return sqlutil::toParameterPack(player.id, -1, player.user, player.commanderName, player.funds, player.powerCharge, player.armyColor,
                                          player.alive, player.totalPowerUses, player.unitFacing, player.playerType, player.team, player.powerActive,
                                          player.powerActiveDay, player.incomeMultiplier, player.coMeterMultiplier);
        }) |
        std::ranges::to<std::vector<sqlutil::ParameterPack>>();
    auto [playerValuesString, playerParametersFlattened] = sqlutil::createInsertValues(playerParameters);
    auto playerInsertStatement = session.connection.prepare_statement(
        std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_PLAYER, playerInsertColumns, playerValuesString));
    session.connection.execute(playerInsertStatement.bind(playerParametersFlattened.begin(), playerParametersFlattened.end()), results);

    // SETTINGS
    auto settingsInsertColumns =
        sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_SETTINGS_COLUMNS | std::views::drop(1) | std::ranges::to<std::vector<db::Column>>());
    auto settingsParameters = sqlutil::toParameterPack(-1, settings.startingFunds, settings.incomeMultiplier, settings.fogOfWar, settings.coPowers,
                                                       settings.teams, settings.modId, settings.coMeterSize, settings.coMeterMultiplier,
                                                       settings.unitLimit, settings.captureLimit, settings.dayLimit);
    auto [settingsValuesString, settingsParametersFlattened] = sqlutil::createInsertValues({settingsParameters});
    auto settingsInsertStatement = session.connection.prepare_statement(
        std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_SETTINGS, settingsInsertColumns, settingsValuesString));
    session.connection.execute(settingsInsertStatement.bind(settingsParametersFlattened.begin(), settingsParametersFlattened.end()), results);

    // SETTINGS VARIANT
    auto settingsVariantInsertColumns = sqlutil::createInsertColumns(dbcurrent::TABLE_STATE_SETTINGS_VARIANT_COLUMNS | std::views::drop(1) |
                                                                     std::ranges::to<std::vector<db::Column>>());
    auto settingsVariantParameters = settings.variant | std::views::transform([](auto const& settingsVariant) {
                                       return sqlutil::toParameterPack(-1, settingsVariant.first, settingsVariant.second);
                                     }) |
                                     std::ranges::to<std::vector<sqlutil::ParameterPack>>();
    auto [settingsVariantValuesString, settingsVariantParametersFlattened] = sqlutil::createInsertValues(settingsVariantParameters);
    auto settingsVariantInsertStatement = session.connection.prepare_statement(
        std::format(db::SQL_INSERT, dbstate, dbcurrent::TABLE_STATE_SETTINGS_VARIANT, settingsVariantInsertColumns, settingsVariantValuesString));
    session.connection.execute(
        settingsVariantInsertStatement.bind(settingsVariantParametersFlattened.begin(), settingsVariantParametersFlattened.end()), results);
    transaction.commit();
  } catch (mysql::error_with_diagnostics const& err) {
    sqlutil::printError(err);
  }
}