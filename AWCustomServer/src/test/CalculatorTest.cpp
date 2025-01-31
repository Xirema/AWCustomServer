#include<Calculator.h>
#include<GameManager.h>
#include<iostream>
#include<print>

int main() {
  try {
    game::GameManager& manager = game::GameManager::instance();
    auto gamePtr = manager.getGame(-1);
    auto modId = i64(gamePtr->object.settings.modId);
    auto modPtr = manager.getMod(modId);

    for(auto const& [id, unit] : gamePtr->object.unitsById) {
      auto terrain = gamePtr->object.terrainsByCoordinate.at(unit);
      if(!terrain) {
        throw std::runtime_error("No Terrain under unit at " + std::format("{},{}", unit.x, unit.y));
      }
      auto unitOwner = calc::getUnitOwner(unit, gamePtr->object);
      if(!unitOwner) {
        throw std::runtime_error("Unable to find owner for unit at " + std::format("{},{}", unit.x, unit.y));
      }
      auto movementRange = calc::calculateUnitMovementRange(unit, gamePtr->object, modPtr->object);
      auto visionRange = calc::calculateUnitVisionRange(unit, gamePtr->object, modPtr->object);
      auto firepower = calc::calculateUnitFirepower(unit, gamePtr->object, modPtr->object);
      auto defense = calc::calculateUnitDefense(unit, gamePtr->object, modPtr->object);
      auto terrainStars = calc::calculateUnitTerrainStars(unit, gamePtr->object, modPtr->object);
      auto goodLuck = calc::calculateUnitLuck(unit, gamePtr->object, modPtr->object);
      auto badLuck = calc::calculateUnitLuck(unit, gamePtr->object, modPtr->object, false);
      auto minRange = calc::calculateUnitRange(unit, 0, gamePtr->object, modPtr->object, false);
      auto maxRange = calc::calculateUnitRange(unit, 0, gamePtr->object, modPtr->object);
      std::print(
        "{} at {},{} standing on {} owned by {}:\n",
        unit.name,
        unit.x, unit.y,
        terrain->name,
        unitOwner->commanderName
      );
      std::print(
        "  Movement Speed: {}\n", movementRange
      );
      std::print(
        "  Vision Range: {}\n", visionRange
      );
      std::print(
        "  Firepower: {} (from {} stars) with {}{}-{} Luck fires in range {}-{}\n", firepower, terrainStars, badLuck > 0 ? "-" : "", badLuck, goodLuck, minRange, maxRange
      );
      std::print(
        "  Defense: {} (from {} stars)\n", defense, terrainStars
      );
    }
  } catch (std::runtime_error const& e) {
    std::print(std::cerr, "{}\n", e.what());
  } catch (net::RestError const& e) {
    std::print(std::cerr, "{}: {}\n", e.type, e.message);
  }
}