#pragma once
#include <jss/jssinclude.h>

namespace dTypes {
struct InitialTerrain {
  std::string terrainName;
  int64_t x;
  int64_t y;
  std::optional<int64_t> orientation;
  std::optional<std::string> playerSlot;
  std::optional<int64_t> startingHitPoints;
  std::optional<int64_t> startingCapturePoints;
};
BOOST_DESCRIBE_STRUCT(InitialTerrain, (), (terrainName, x, y, orientation, playerSlot, startingHitPoints, startingCapturePoints))

struct InitialUnit {
  std::string unitName;
  int64_t x;
  int64_t y;
  std::optional<std::string> playerSlot;
  std::optional<int64_t> startingHitPoints;
  std::optional<int64_t> startingAmmo;
  std::optional<int64_t> startingFuel;
  std::optional<int64_t> startingStunned;
  std::optional<bool> startingStealthed;
  std::optional<double> startingLuck;
  std::optional<int64_t> transportedBy;
};
BOOST_DESCRIBE_STRUCT(InitialUnit, (),
                      (unitName, x, y, playerSlot, startingHitPoints, startingAmmo, startingFuel, startingStunned, startingStealthed, startingLuck,
                       transportedBy))

struct MapMetadata {
  int64_t id;
  std::string name;
  std::string version;
  std::optional<std::string> expired;
  std::optional<std::string> description;
  std::string modName;
  std::optional<std::string> modVersion;
};
BOOST_DESCRIBE_STRUCT(MapMetadata, (), (id, name, version, expired, description, modName, modVersion))

struct MapDefinition {
  MapMetadata metadata;
  std::vector<std::string> playerSlots;
  std::vector<InitialTerrain> initialTerrains;
  std::vector<InitialUnit> initialUnits;
};
BOOST_DESCRIBE_STRUCT(MapDefinition, (), (metadata, playerSlots, initialTerrains, initialUnits))
}  // namespace dTypes
