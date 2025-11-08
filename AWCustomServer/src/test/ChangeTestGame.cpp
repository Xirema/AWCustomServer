#include<DBFunctions.h>

#include <print>
#include <random>
#include <ranges>
int main() {
  dTypes::MapDefinition newTestMap;
  newTestMap.metadata.name = "Test Map";
  newTestMap.metadata.version = "0.0.1";
  newTestMap.metadata.description = "Ipsum Lorum";
  newTestMap.metadata.modName = "AWBW";
  newTestMap.metadata.modVersion = "Beta 0.06";
  newTestMap.playerSlots = {"max", "olaf", "sturm", "kindle", "grit"};
  auto& terrains = newTestMap.initialTerrains;
  auto& units = newTestMap.initialUnits;

  std::map<std::string_view, sTypes::PlayerState> playerSlots;
  playerSlots["max"] = sTypes::PlayerState{.user = 11,
                                           .commanderName = "max",
                                           .funds = 0,
                                           .powerCharge = 0,
                                           .armyColor = "os",
                                           .alive = true,
                                           .totalPowerUses = 0,
                                           .unitFacing = 0,
                                           .playerType = "normal",
                                           .powerActive = "scop",
                                           .powerActiveDay = 1};
  playerSlots["olaf"] = sTypes::PlayerState{.user = 12,
                                            .commanderName = "olaf",
                                            .funds = 0,
                                            .powerCharge = 0,
                                            .armyColor = "bm",
                                            .alive = true,
                                            .totalPowerUses = 0,
                                            .unitFacing = 1,
                                            .playerType = "normal"};
  playerSlots["sturm"] = sTypes::PlayerState{.user = 13,
                                             .commanderName = "sturm",
                                             .funds = 0,
                                             .powerCharge = 0,
                                             .armyColor = "bh",
                                             .alive = true,
                                             .totalPowerUses = 0,
                                             .unitFacing = 0,
                                             .playerType = "normal"};
  playerSlots["kindle"] = sTypes::PlayerState{.user = 14,
                                              .commanderName = "kindle",
                                              .funds = 0,
                                              .powerCharge = 0,
                                              .armyColor = "pl",
                                              .alive = true,
                                              .totalPowerUses = 0,
                                              .unitFacing = 1,
                                              .playerType = "normal"};
  playerSlots["grit"] = sTypes::PlayerState{.user = 15,
                                            .commanderName = "grit",
                                            .funds = 0,
                                            .powerCharge = 0,
                                            .armyColor = "ge",
                                            .alive = true,
                                            .totalPowerUses = 0,
                                            .unitFacing = 0,
                                            .playerType = "normal"};
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
    terrains.emplace_back() = dTypes::InitialTerrain{.terrainName = std::string{terrainType}, .x = x, .y = y};
    if (UNIT_SPAWN_CHANCE < unitSpawnDist(engine)) {
      continue;
    }
    auto unitIndex = playerDist(engine);
    auto playerSlotIt = playerSlots.begin();
    std::advance(playerSlotIt, unitIndex);
    auto playerSlot = playerSlotIt->first;
    auto const& unitList = coUnitDistributions.at(playerSlot);
    auto unitName = unitList[unitDist(engine)];
    units.emplace_back() = dTypes::InitialUnit{.unitName = std::string{unitName},
                                               .x = x,
                                               .y = y,
                                               .playerSlot = std::string{playerSlot},
                                               .startingHitPoints = 100,
                                               .startingAmmo = 9,
                                               .startingFuel = 99,
                                               .startingLuck = unitSpawnDist(engine) - unitSpawnDist(engine)};
  }

  auto mapId = db::uploadMap(newTestMap);

  sTypes::SettingsState settings{.startingFunds = 0,
                                 .incomeMultiplier = 100,
                                 .fogOfWar = false,
                                 .variant = {{"normal", 100l}},
                                 .coPowers = true,
                                 .teams = false,
                                 .modId = 1,
                                 .coMeterSize = 9000,
                                 .coMeterMultiplier = 100};


  db::createGame(-1, mapId, playerSlots, settings);
}