#pragma once
#include<jss/jssinclude.h>
#include<sstream>

namespace dTypes{
	struct MovementRule{
		std::string name;
		std::string type;
		int64_t maxRepeat;
		std::string confirmType;
		bool stopIfUsed;
	};
	BOOST_DESCRIBE_STRUCT(MovementRule, (), (
		name,
		type,
		maxRepeat,
		confirmType,
		stopIfUsed
	))
	struct MovementClass
	{
		std::string name;
		std::map<std::string, int64_t> movementCosts;
		std::optional<std::map<std::string, std::map<std::string, int64_t>>> variantMods;
		std::vector<std::string> movementRules;
	};
	BOOST_DESCRIBE_STRUCT(MovementClass, (), (
		name,
		movementCosts,
		variantMods,
		movementRules
	))
}
