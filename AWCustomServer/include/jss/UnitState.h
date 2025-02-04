#pragma once
#include <jss/jssinclude.h>

namespace sTypes
{
	struct UnitState
	{
		int64_t id;
		int64_t x;
		int64_t y;
		std::string name;
		int64_t ammo;
		int64_t fuel;
		bool active;
		std::optional<int64_t> stunned;
		std::optional<std::vector<int64_t>> transporting;
		std::optional<int64_t> owner;
		std::optional<int64_t> hitPoints;
		std::optional<bool> stealthed;
		std::optional<double> currentGoodLuck;
		std::optional<double> currentBadLuck;
	};
	BOOST_DESCRIBE_STRUCT(UnitState, (), (
		id,
		x,
		y,
		name,
		ammo,
		fuel,
		active,
		stunned,
		transporting,
		owner,
		hitPoints,
		stealthed,
		currentGoodLuck,
		currentBadLuck
	))
}
