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
        throw std::runtime_error("No Terrain under unit at " + std::vformat("{},{}", std::make_format_args(unit.x, unit.y)));
      }
      auto movementRange = calc::calculateUnitMovementRange(unit, gamePtr->object, modPtr->object);
      auto visionRange = calc::calculateUnitVisionRange(unit, gamePtr->object, modPtr->object);
      auto unitOwner = calc::getUnitOwner(unit, gamePtr->object);
      if(!unitOwner) {
        throw std::runtime_error("Unable to find owner for unit at " + std::vformat("{},{}", std::make_format_args(unit.x, unit.y)));
      }
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
    }
  } catch (std::runtime_error const& e) {
    std::print(std::cerr, "{}\n", e.what());
  } catch (net::RestError const& e) {
    std::print(std::cerr, "{}: {}\n", e.type, e.message);
  }
}