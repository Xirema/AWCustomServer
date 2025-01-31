#include<RestFunctions.h>
#include<random>

namespace db {
  sTypes::GameState get_gamestate(int64_t gameId) {
    if (gameId != -1) {
      throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
    }
    sTypes::GameState gameState;
    gameState.active = true;
    gameState.day = 1;
    gameState.id = "-1";
    gameState.playerOrder = { "9999","10000", "10001", "10002", "10003" };
    gameState.playerTurn = 0;
    gameState.variant = "normal";
    return gameState;
  }
	std::vector<sTypes::PlayerState> get_playerstates(int64_t gameId) {
    if (gameId != -1) {
      throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
    }
    std::vector<sTypes::PlayerState> playerStates;
    auto& player1 = playerStates.emplace_back();
    player1.alive = true;
    player1.armyColor = "os";
    player1.commanderName = "max";
    player1.id = "9999";
    player1.owner = "1";
    player1.funds = 10000;
    player1.powerCharge = 0;
    player1.totalPowerUses = 1;
    player1.powerActive = "scop";
    player1.playerType = "normal";
    auto& player2 = playerStates.emplace_back();
    player2.alive = true;
    player2.armyColor = "bh";
    player2.commanderName = "sturm";
    player2.id = "10000";
    player2.owner = "2";
    player2.funds = 15000;
    player2.powerCharge = 63000;
    player2.totalPowerUses = 0;
    player2.playerType = "normal";
    player2.unitFacing = 1;
    auto& player3 = playerStates.emplace_back();
    player3.alive = true;
    player3.armyColor = "rf";
    player3.commanderName = "kindle";
    player3.id = "10001";
    player3.owner = "3";
    player3.funds = 5000;
    player3.powerCharge = 0;
    player3.totalPowerUses = 1;
    player3.powerActive = "scop";
    player3.playerType = "normal";
    auto& player4 = playerStates.emplace_back();
    player4.alive = true;
    player4.armyColor = "ge";
    player4.commanderName = "lash";
    player4.id = "10002";
    player4.owner = "4";
    player4.funds = 5000;
    player4.powerCharge = 0;
    player4.totalPowerUses = 1;
    player4.powerActive = "scop";
    player4.playerType = "normal";
    auto& player5 = playerStates.emplace_back();
    player5.alive = true;
    player5.armyColor = "bm";
    player5.commanderName = "grit";
    player5.id = "10003";
    player5.owner = "5";
    player5.funds = 5000;
    player5.powerCharge = 0;
    player5.totalPowerUses = 1;
    player5.playerType = "normal";
    return playerStates;
  }
	std::vector<sTypes::UnitState> get_unitstates(int64_t gameId) {
    if (gameId != -1)
    {
      throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
    }
    std::vector<sTypes::UnitState> unitStates;
    for (int i = 0; i < 8; i++)
    {
      auto &unit = unitStates.emplace_back();
      unit.id = std::to_string(i + 1);
      unit.y = i;
      unit.x = 1;
      unit.active = true;
      unit.ammo = 0;
      unit.name = "infantry";
      unit.fuel = 99;
      unit.hitPoints = 100;
      unit.owner = "9999";
      if (unit.y == 3)
      {
        unit.name = "fighter";
        unit.ammo = 1;
      }
      if (unit.y == 4)
      {
        unit.name = "tank";
      }
      if(i == 6) {
        unit.name = "artillery";
        unit.ammo = 1;
      }
    }
    for (int i = 0; i < 8; i++)
    {
      auto &unit = unitStates.emplace_back();
      unit.id = std::to_string(i + 9);
      unit.y = i;
      unit.x = 6;
      unit.active = true;
      unit.ammo = 0;
      unit.name = "infantry";
      unit.fuel = 99;
      unit.hitPoints = 100;
      unit.owner = "10000";
    }
    for (int i = 0; i < 8; i++)
    {
      auto &unit = unitStates.emplace_back();
      unit.id = std::to_string(i + 17);
      unit.y = i;
      unit.x = 8;
      unit.active = true;
      unit.ammo = 0;
      unit.name = "infantry";
      unit.fuel = 99;
      unit.hitPoints = 100;
      unit.owner = "10001";
    }
    for (int i = 0; i < 8; i++)
    {
      auto &unit = unitStates.emplace_back();
      unit.id = std::to_string(i + 25);
      unit.y = 10;
      unit.x = i;
      unit.active = true;
      unit.ammo = 0;
      unit.name = "infantry";
      if(i == 4) {
        unit.name = "battle_copter";
      }
      if(i == 6) {
        unit.name = "artillery";
      }
      unit.fuel = 99;
      unit.hitPoints = 100;
      unit.owner = "10002";
    }
    for (int i = 0; i < 8; i++)
    {
      auto &unit = unitStates.emplace_back();
      unit.id = std::to_string(i + 33);
      unit.y = 15;
      unit.x = i;
      unit.active = true;
      unit.ammo = 5;
      unit.name = "infantry";
      if(i == 4) {
        unit.name = "rocket";
      }
      if(i == 6) {
        unit.name = "artillery";
      }
      unit.fuel = 99;
      unit.hitPoints = 100;
      unit.owner = "10003";
    }
    return unitStates;
  }
	std::vector<sTypes::TerrainState> get_terrainstates(int64_t gameId) {
    if (gameId != -1)
    {
      throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
    }
    std::vector<sTypes::TerrainState> terrainStates;
    int nextId = 0;
    std::minstd_rand engine{static_cast<uint32_t>(nextId + 55)};
    std::uniform_int_distribution<uint32_t> dist{0, 6};
    std::uniform_int_distribution<uint32_t> forestReplace{0, 99};
    for (int i = 0; i < 50; i++)
    {
      for (int j = 0; j < 50; j++)
      {
        auto &terrain = terrainStates.emplace_back();
        terrain.id = std::to_string(nextId++);
        terrain.name = "plains";
        terrain.x = i;
        terrain.y = j;
        if (((terrain.y * 2) % 5) == (terrain.x % 5))
        {
          terrain.name = "mountain";
        }

        if(forestReplace(engine) == 0) {
          terrain.name = "forest";
        }

        if(i == 40 && j == 40) {
          terrain.name = "tower";
          terrain.owner = "9999";
        }

        if(i == 45) {
          terrain.name = "city";
          terrain.owner = "10001";
        }

        if (terrain.name == "plains")
        { 
          terrain.orientation = dist(engine);
        }

      }
    }

    return terrainStates;
  }
	sTypes::SettingsState get_settingstate(int64_t gameId) {
    if (gameId != -1)
    {
      throw net::RestError("No Game Found", net::RestError::Type::INVALID_DATA);
    }
    sTypes::SettingsState settingState;
    settingState.id = std::to_string(1);
    settingState.fogOfWar = false;
    settingState.teams = false;
    settingState.variant = {{"normal", 100}};
    settingState.incomeMultiplier = 100;
    settingState.startingFunds = 0;
    settingState.coMeterMultiplier = 100;
    settingState.coMeterSize = 9000;
    settingState.coPowers = true;
    settingState.modId = "7";
    return settingState;
  }
}