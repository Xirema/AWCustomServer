#include<GameManager.h>
#include<random>
#include<SQLUtil.h>
namespace mysql = boost::mysql;
int main() {
  std::vector<sTypes::TerrainState> terrains;
  std::minstd_rand engine{std::random_device{}()};
  std::uniform_int_distribution<int64_t> forestReplace{0, 99};
  std::uniform_int_distribution<int64_t> plainsOrientations{0, 6};
  for(int64_t y = 0; y < 50; y++) {
    for(int64_t x = 0; x < 50; x++) {
      auto& newTerrain = terrains.emplace_back();
      newTerrain.x = x;
      newTerrain.y = y;
      newTerrain.name = "plains";
      if(((y * 2) % 5) == (x % 5)) {
        newTerrain.name = "mountains";
      }
      if(forestReplace(engine) == 0) {
        newTerrain.name = "forest";
      }
      if(x == 40 && y == 40) {
        newTerrain.name = "tower";
        newTerrain.owner = 9999;
      }
      if(x == 45) {
        newTerrain.name = "city";
        newTerrain.owner = 10001;
      }
      if(newTerrain.name == "plains") {
        newTerrain.orientation = plainsOrientations(engine);
      }
    }
  }

  sqlutil::Session session;
  auto countStatement = session.connection.prepare_statement(R"SQL(
    select count(*)
    from STATE.TERRAIN
    where GAME_ID = ?
  )SQL");
  mysql::results countResults;
  session.connection.execute(countStatement.bind(-1), countResults);
  for(auto const& row : countResults.rows()) {
    auto count = row.at(0).as_int64();
    if(count > 0) {
      std::print("Terrains have already been initialized; delete from table before running again!\n");
      return -1;
    }
  }
  auto insertStatement = session.connection.prepare_statement(R"SQL(
    insert into STATE.TERRAIN
      (GAME_ID, X, Y, NAME, ORIENTATION, CAPTUREPOINTS, OWNER, ACTIVATIONCOUNT, HITPOINTS)
    values
      (?, ?, ?, ?, ?, ?, ?, ?, ?)
  )SQL");
  for(auto const& terrain : terrains) {
    mysql::results results;
    session.connection.execute(
      insertStatement.bind(
        -1, terrain.x, terrain.y, terrain.name, terrain.orientation, terrain.capturePoints, terrain.owner, terrain.activationCount, terrain.hitPoints
      ), results
    );
  }
}