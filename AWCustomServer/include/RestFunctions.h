#pragma once
#include<RestServer.h>

namespace rest {
	namespace data {
		std::string upload_mod(net::HTTPHeaders const& headers, std::string body);
		std::string get_units(net::HTTPHeaders const& headers);
		std::string get_weapons(net::HTTPHeaders const& headers);
		std::string get_terrains(net::HTTPHeaders const& headers);
		std::string get_commanders(net::HTTPHeaders const& headers);
		std::string get_movements(net::HTTPHeaders const& headers);
		std::string get_players(net::HTTPHeaders const& headers);
		std::string get_pues(net::HTTPHeaders const& headers);
		std::string get_aues(net::HTTPHeaders const& headers);
		std::string get_ptes(net::HTTPHeaders const& headers);
		std::string get_ates(net::HTTPHeaders const& headers);
		std::string get_pges(net::HTTPHeaders const& headers);
		std::string get_ages(net::HTTPHeaders const& headers);
		std::string get_settings(net::HTTPHeaders const& headers);
		std::string get_mod_metadata(net::HTTPHeaders const& headers);
	}

	namespace state {
		std::string get_gamestate(net::HTTPHeaders const& headers);
		std::string get_playerstates(net::HTTPHeaders const& headers);
		std::string get_unitstates(net::HTTPHeaders const& headers);
		std::string get_terrainstates(net::HTTPHeaders const& headers);
		std::string get_settingstate(net::HTTPHeaders const& headers);
	}

	namespace resource {
		std::string upload_pack(net::HTTPHeaders const& headers, std::string body);
		std::string get_resource_pack(net::HTTPHeaders const& headers);
		std::string get_pack_metadata(net::HTTPHeaders const& headers);
	}
}