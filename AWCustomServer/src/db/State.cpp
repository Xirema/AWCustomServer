#include<DBFunctions.h>
#include<Error.h>
#include<random>
#include<SQLUtil.h>

namespace mysql = boost::mysql;

namespace db {
  sTypes::GameState get_gamestate(int64_t gameId) {
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select *
      from STATE.GAME
      where ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(statement.bind(gameId), results);
    sTypes::GameState gameState;
    for(auto const& row : results.rows()) {
      gameState.id = row.at(0).as_int64();
      gameState.day = row.at(1).as_int64();
      gameState.playerTurn = row.at(2).as_int64();
      gameState.variant = std::string{row.at(3).as_string()};
      if(row.at(4).is_string()) {
        gameState.variantStart = std::string{row.at(4).as_string()};
      }
      gameState.active = row.at(5).as_int64() ? true : false;
      gameState.hash = row.at(6).as_int64();
      auto playerOrderStatement = session.connection.prepare_statement(R"SQL(
        select *
        from STATE.GAME_PLAYERORDER_REFERENCE
        where GAME_ID = ?
        order by `order` asc
      )SQL");
      mysql::results playerOrderResults;
      session.connection.execute(playerOrderStatement.bind(gameId), playerOrderResults);
      for(auto const& orderRow : playerOrderResults.rows()) {
        gameState.playerOrder.emplace_back(orderRow.at(2).as_int64());
      }
      return gameState;
    }
    throw net::RestError("No Game Found", net::RestErrorType::INVALID_DATA);
  }
	std::vector<sTypes::PlayerState> get_playerstates(int64_t gameId) {
    std::vector<sTypes::PlayerState> ret;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select *
      from STATE.PLAYER
      where GAME_ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(statement.bind(gameId), results);
    if(results.rows().size() == 0) {
      throw net::RestError("Game has no players (or does not exist)", net::RestErrorType::NOT_FOUND);
    }
    for(auto const& row : results.rows()) {
      auto& newPlayer = ret.emplace_back();
      newPlayer.id = row.at(0).as_int64();
      newPlayer.user = row.at(2).as_int64();
      newPlayer.commanderName = std::string{row.at(3).as_string()};
      newPlayer.funds = row.at(4).as_int64();
      newPlayer.powerCharge = row.at(5).as_int64();
      newPlayer.armyColor = std::string{row.at(6).as_string()};
      newPlayer.alive = row.at(7).as_int64() ? true : false;
      newPlayer.totalPowerUses = row.at(8).as_int64();
      newPlayer.unitFacing = row.at(9).as_int64();
      newPlayer.playerType = std::string{row.at(10).as_string()};
      if(row.at(11).is_string()) {
        newPlayer.team = std::string{row.at(11).as_string()};
      }
      if(row.at(12).is_string()) {
        newPlayer.powerActive = std::string{row.at(12).as_string()};
      }
      if(row.at(13).is_int64()) {
        newPlayer.powerActiveDay = row.at(13).as_int64();
      }
      if(row.at(14).is_int64()) {
        newPlayer.incomeMultiplier = row.at(14).as_int64();
      }
      if(row.at(15).is_int64()) {
        newPlayer.coMeterMultiplier = row.at(15).as_int64();
      }
      auto bannedUnitsStatement = session.connection.prepare_statement(R"SQL(
        select *
        from STATE.PLAYER_BANNEDUNITS_REFERENCE
        where GAME_ID = ? and PLAYER_ID = ?
      )SQL");
      mysql::results bannedUnitsResults;
      session.connection.execute(bannedUnitsStatement.bind(gameId, newPlayer.id), bannedUnitsResults);
      for(auto const& bannedUnit : bannedUnitsResults.rows()) {
        if(!newPlayer.bannedUnits) {
          newPlayer.bannedUnits.emplace();
        }
        newPlayer.bannedUnits->emplace_back(bannedUnit.at(3).as_string());
      }
    }
    return ret;
  }
	std::vector<sTypes::UnitState> get_unitstates(int64_t gameId) {
    std::vector<sTypes::UnitState> ret;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select *
      from STATE.UNIT
      where GAME_ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(statement.bind(gameId), results);
    for(auto const& row : results.rows()) {
      auto& newUnit = ret.emplace_back();
      newUnit.id = row.at(0).as_int64();
      newUnit.x = row.at(2).as_int64();
      newUnit.y = row.at(3).as_int64();
      newUnit.name = std::string{row.at(4).as_string()};
      newUnit.ammo = row.at(5).as_int64();
      newUnit.fuel = row.at(6).as_int64();
      newUnit.active = row.at(7).as_int64() ? true : false;
      if(row.at(8).is_int64()) {
        newUnit.stunned = row.at(8).as_int64();
      }
      if(row.at(9).is_int64()) {
        newUnit.owner = row.at(9).as_int64();
      }
      if(true /*TODO: Check if intel permits this info!*/) {
        newUnit.hitPoints = row.at(10).as_int64();
      }
      if(row.at(11).is_int64()) {
        newUnit.stealthed = row.at(11).as_int64() ? true : false;
      }
      if(true /*TODO: Check if intel permits this info!*/) {
        newUnit.currentGoodLuck = row.at(12).as_double();
      }
      if(true /*TODO: Check if intel permits this info!*/) {
        newUnit.currentBadLuck = row.at(13).as_double();
      }
      auto transportStatement = session.connection.prepare_statement(R"SQL(
        select *
        from STATE.UNIT_TRANSPORT_REFERENCE
        where GAME_ID = ? and TRANSPORT_ID = ?
        order by `ORDER` asc
      )SQL");
      mysql::results transportResults;
      session.connection.execute(transportStatement.bind(gameId, newUnit.id), transportResults);
      for(auto const& transportRow : transportResults.rows()) {
        if(!newUnit.transporting) {
          newUnit.transporting.emplace();
        }
        newUnit.transporting->emplace_back(transportRow.at(3).as_int64());
      }
    }
    return ret;
  }
	std::vector<sTypes::TerrainState> get_terrainstates(int64_t gameId) {
    std::vector<sTypes::TerrainState> ret;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select *
      from STATE.TERRAIN
      where GAME_ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(statement.bind(gameId), results);
    for(auto const& row : results.rows()) {
      auto& newTerrain = ret.emplace_back();
      newTerrain.id = row.at(0).as_int64();
      newTerrain.x = row.at(2).as_int64();
      newTerrain.y = row.at(3).as_int64();
      newTerrain.name = std::string{row.at(4).as_string()};
      if(row.at(5).is_int64()) {
        newTerrain.orientation = row.at(5).as_int64();
      }
      if(row.at(6).is_int64()) {
        newTerrain.capturePoints = row.at(6).as_int64();
      }
      if(row.at(7).is_int64()) {
        newTerrain.owner = row.at(7).as_int64();
      }
      if(row.at(8).is_int64()) {
        newTerrain.activationCount = row.at(8).as_int64();
      }
      if(row.at(9).is_int64()) {
        newTerrain.hitPoints = row.at(9).as_int64();
      }
    }
    return ret;
  }
	sTypes::SettingsState get_settingstate(int64_t gameId) {
    sTypes::SettingsState ret;
    sqlutil::Session session;
    auto statement = session.connection.prepare_statement(R"SQL(
      select *
      from STATE.SETTINGS
      where GAME_ID = ?
    )SQL");
    mysql::results results;
    session.connection.execute(statement.bind(gameId), results);
    for(auto const& row : results.rows()) {
      ret.id = row.at(0).as_int64();
      ret.startingFunds = row.at(2).as_int64();
      ret.incomeMultiplier = row.at(3).as_int64();
      ret.fogOfWar = row.at(4).as_int64() ? true : false;
      ret.coPowers = row.at(5).as_int64() ? true : false;
      ret.teams = row.at(6).as_int64() ? true : false;
      ret.modId = row.at(7).as_int64();
      ret.coMeterSize = row.at(8).as_int64();
      ret.coMeterMultiplier = row.at(9).as_int64();
      if(row.at(10).is_int64()) {
        ret.unitLimit = row.at(10).as_int64();
      }
      if(row.at(11).is_int64()) {
        ret.captureLimit = row.at(11).as_int64();
      }
      if(row.at(12).is_int64()) {
        ret.dayLimit = row.at(12).as_int64();
      }
      auto variantSettingsStatement = session.connection.prepare_statement(R"SQL(
        select *
        from STATE.SETTINGS_VARIANT_REFERENCE
        where GAME_ID = ?
      )SQL");
      mysql::results variantResults;
      session.connection.execute(variantSettingsStatement.bind(gameId), variantResults);
      for(auto const& variantRow : variantResults.rows()) {
        ret.variant[std::string{variantRow.at(2).as_string()}] = variantRow.at(3).as_int64();
      }
    }

    return ret;
  }
}