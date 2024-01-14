#include<RestServer.h>
#include<RestFunctions.h>
#include<Properties.h>
#include<iostream>
#include<Cert.h>
#include<boost/json/src.hpp>

int main() {
	try {
		properties::Properties const& props = properties::Properties::instance();
		net::SSLCert sslCert{.key = cert::key, .cert = cert::certificate};
		net::RestServer server{ "AWCustom Server", static_cast<uint16_t>(props.getInt("serverPort")), sslCert };
		std::unordered_map<std::string, net::GETFunc> getFunctions;
		std::unordered_map<std::string, net::POSTFunc> postFunctions{
			{"/data/uploadMod", rest::data::upload_mod},
			{"/data/getUnits", rest::data::get_units},
			{"/data/getWeapons", rest::data::get_weapons},
			{"/data/getTerrains", rest::data::get_terrains},
			{"/data/getMovements", rest::data::get_movements},
			{"/data/getCommanders", rest::data::get_commanders},
			{"/data/getPlayers", rest::data::get_players},
			{"/data/getPUEs", rest::data::get_pues},
			{"/data/getAUEs", rest::data::get_aues},
			{"/data/getPTEs", rest::data::get_ptes},
			{"/data/getATEs", rest::data::get_ates},
			{"/data/getPGEs", rest::data::get_pges},
			{"/data/getAGEs", rest::data::get_ages},
			{"/data/getSettings", rest::data::get_settings},
			{"/data/getModData", rest::data::get_mod_metadata},
			{"/data/getTextResources", rest::data::get_text_resources},
			{"/data/getImageResources", rest::data::get_image_resources},

			{"/state/getGameState", rest::state::get_gamestate},
			{"/state/getUnitStates", rest::state::get_unitstates},
			{"/state/getTerrainStates", rest::state::get_terrainstates},
			{"/state/getPlayerStates", rest::state::get_playerstates},
			{"/state/getSettingState", rest::state::get_settingstate},
		};
		server.start(getFunctions, postFunctions);
		//server.printRequests(true);
		std::cout << "Listening for the following endpoints:" << std::endl;
		for (auto const& [name, func] : getFunctions) {
			std::cout << "  " << name << std::endl;
		}
		for (auto const& [name, func] : postFunctions) {
			std::cout << "  " << name << std::endl;
		}
		std::cout << "Press [Enter] to stop the server." << std::endl;
		std::string line;
		std::getline(std::cin, line);
	}
	catch (std::runtime_error const& e) {
		std::cerr << e.what() << std::endl;
	}
	catch (...) {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (std::exception const& e) {
			std::cerr << e.what() << std::endl;
		}
	}
}
