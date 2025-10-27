#pragma once
#include <jss/Game.h>
#include <jss/ModMetadata.h>

namespace calc {
struct CalculatorState {
  game::Game const& game;
  dTypes::ModData const& modData;
  std::optional<int64_t> requestingUser;
  CalculatorState(game::Game const& game, dTypes::ModData const& modData, std::optional<int64_t> requestingUser = {})
      : game(game), modData(modData), requestingUser(requestingUser) {}
};
int64_t calculateUnitMovementRange(sTypes::UnitState const& unit, CalculatorState state);
int64_t calculateUnitVisionRange(sTypes::UnitState const& unit, CalculatorState state);
int64_t calculateUnitFirepower(sTypes::UnitState const& unit, CalculatorState state, bool attacking = true);
int64_t calculateUnitDefense(sTypes::UnitState const& unit, CalculatorState state, bool attackerIndirect = false);
int64_t calculateUnitLuck(sTypes::UnitState const& unit, CalculatorState state, bool goodLuck = true);

sTypes::PlayerState const* getUnitOwner(sTypes::UnitState const& unit, game::Game const& game);

std::string getCurrentVariant(CalculatorState state);

std::vector<std::pair<dTypes::PassiveUnitEffect const*, sTypes::PlayerState const*>> getAllPassiveUnitEffects(
    sTypes::UnitState const& unit, CalculatorState state,
    std::function<bool(dTypes::PassiveUnitEffect const*)> filter = [](auto&& effect) { return true; });

std::vector<std::pair<dTypes::PassiveTerrainEffect const*, sTypes::PlayerState const*>> getAllPassiveTerrainEffects(
    sTypes::TerrainState const& terrain, CalculatorState state,
    std::function<bool(dTypes::PassiveTerrainEffect const*)> filter = [](auto&& effect) { return true; });

std::vector<std::pair<dTypes::PassiveGlobalEffect const*, sTypes::PlayerState const*>> getAllPassiveGlobalEffects(
    sTypes::PlayerState const* targetPlayer, CalculatorState state,
    std::function<bool(dTypes::PassiveGlobalEffect const*)> filter = [](auto&& effect) { return true; });

bool unitMatchesEffect(dTypes::UnitType const& unitType, std::string_view terrainName, dTypes::PassiveUnitEffect const& effect);

bool terrainMatchesEffect(dTypes::TerrainType const& terrainType, dTypes::PassiveTerrainEffect const& effect);

int64_t countTerrainsOwnedByPlayer(int64_t playerId, std::vector<std::string> const& terrainNames, CalculatorState state);

int64_t calculateUnitTerrainStars(sTypes::UnitState const& unit, CalculatorState state);

int64_t calculateUnitRange(sTypes::UnitState const& unit, int64_t weaponIndex, CalculatorState state, bool maxRange = true);

int64_t calculateUnitCapturePoints(sTypes::UnitState const& unit, CalculatorState state);

int64_t calculateUnitCost(sTypes::UnitState const& unit, CalculatorState state);

std::optional<int64_t> calculateMovementCost(sTypes::UnitState const& unit, sTypes::TerrainState const& terrain, CalculatorState state);

enum class UnitIntelFlags : uint64_t { NONE = 0, HIDE_HITPOINTS = 1, EXACT_HITPOINTS = 2, LUCK = 4 };

UnitIntelFlags getUnitIntel(sTypes::UnitState const& unit, sTypes::PlayerState const* observingPlayer, CalculatorState state);

constexpr UnitIntelFlags& operator|=(UnitIntelFlags& a, UnitIntelFlags b) {
  a = static_cast<UnitIntelFlags>(static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
  return a;
}

constexpr bool operator&(UnitIntelFlags a, UnitIntelFlags b) { return static_cast<uint64_t>(a) & static_cast<uint64_t>(b); }

enum class Alliance { SELF, ALLY, ENEMY, NEUTRAL };

Alliance getAlliance(sTypes::PlayerState const* a, sTypes::PlayerState const* b);

constexpr int64_t flatHitPoints(int64_t realHitPoints) { return static_cast<int64_t>(std::ceil(realHitPoints / 10.)) * 10; }
}  // namespace calc