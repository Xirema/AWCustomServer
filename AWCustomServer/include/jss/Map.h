#pragma once
#include <jss/jssinclude.h>

namespace dTypes
{
	struct InitialTerrain {
		std::string terrainName;
		int64_t x;
		int64_t y;
		std::optional<int64_t> orientation;
		std::optional<std::string> playerSlot;
		std::optional<std::map<std::string, int64_t>> terrainMods;
	};
	BOOST_DESCRIBE_STRUCT(InitialTerrain, (), (terrainName, x, y, orientation, playerSlot, terrainMods))
	struct InitialUnit {
		std::string unitName;
		int64_t x;
		int64_t y;
		std::optional<std::string> playerSlot;
		std::optional<std::map<std::string, int64_t>> unitMods;
	};
	BOOST_DESCRIBE_STRUCT(InitialUnit, (), (unitName, x, y, playerSlot, unitMods))
	struct MapType
	{
		std::string name;
		std::optional<std::vector<std::string>> playerSlots;
		std::vector<InitialTerrain> initialTerrains;
		std::optional<std::vector<InitialUnit>> initialUnits;
	};
	BOOST_DESCRIBE_STRUCT(MapType, (), (name, playerSlots, initialTerrains, initialUnits))
}
