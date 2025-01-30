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
      auto movementRange = calc::calculateUnitMovementRange(unit, gamePtr->object, modPtr->object);
      auto visionRange = calc::calculateUnitVisionRange(unit, gamePtr->object, modPtr->object);
      auto unitOwner = calc::getUnitOwner(unit, gamePtr->object);
      std::print(
        "{} at {},{} owned by {}:\n",
        unit.name,
        unit.x, unit.y,
        unitOwner ? unitOwner->commanderName : "<no one>"
      );
      std::print(
        "  Movement Speed: {}\n", movementRange
      );
      std::print(
        "  Vision Range: {}\n", visionRange
      );
    }
    gamePtr->object.gameState.variant="rain";
    for(auto const& [id, unit] : gamePtr->object.unitsById) {
      auto movementRange = calc::calculateUnitMovementRange(unit, gamePtr->object, modPtr->object);
      auto visionRange = calc::calculateUnitVisionRange(unit, gamePtr->object, modPtr->object);
      auto unitOwner = calc::getUnitOwner(unit, gamePtr->object);
      std::print(
        "{} at {},{} owned by {}:\n",
        unit.name,
        unit.x, unit.y,
        unitOwner ? unitOwner->commanderName : "<no one>"
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