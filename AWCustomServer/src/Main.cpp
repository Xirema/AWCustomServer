#include<RestServer.h>
#include<RestFunctions.h>
#include<Properties.h>
#include<iostream>
#include<Cert.h>
#include<signal.h>
#include<RestFunctionUtility.h>

static std::atomic_bool terminated = false;

void terminationHandler(int signal) {
  terminated = true;
}

int main() {
  try {
    properties::Properties const& props = properties::Properties::instance();
    //Uncomment to reenable SSL
    //net::SSLCert sslCert{.key = cert::key, .cert = cert::certificate};
    //net::RestServer server{ "AWCustom Server", static_cast<uint16_t>(props.getInt("serverPort")), sslCert };
    net::RestServer server{ "AWCustom Server", static_cast<uint16_t>(props.getInt("serverPort"))};
    net::HTTPFunctionMap functions;
    functions[net::define(net::HTTPVerb::GET, "/data/getUnits")] = net::convert(rest::data::get_units);
    functions[net::define(net::HTTPVerb::GET, "/data/getWeapons")] = net::convert(rest::data::get_weapons);
    functions[net::define(net::HTTPVerb::GET, "/data/getTerrains")] = net::convert(rest::data::get_terrains);
    functions[net::define(net::HTTPVerb::GET, "/data/getMovements")] = net::convert(rest::data::get_movements);
    functions[net::define(net::HTTPVerb::GET, "/data/getMovementRules")] = net::convert(rest::data::get_movement_rules);
    functions[net::define(net::HTTPVerb::GET, "/data/getCommanders")] = net::convert(rest::data::get_commanders);
    functions[net::define(net::HTTPVerb::GET, "/data/getPlayers")] = net::convert(rest::data::get_players);
    functions[net::define(net::HTTPVerb::GET, "/data/getPUEs")] = net::convert(rest::data::get_pues);
    functions[net::define(net::HTTPVerb::GET, "/data/getAUEs")] = net::convert(rest::data::get_aues);
    functions[net::define(net::HTTPVerb::GET, "/data/getPTEs")] = net::convert(rest::data::get_ptes);
    functions[net::define(net::HTTPVerb::GET, "/data/getATEs")] = net::convert(rest::data::get_ates);
    functions[net::define(net::HTTPVerb::GET, "/data/getPGEs")] = net::convert(rest::data::get_pges);
    functions[net::define(net::HTTPVerb::GET, "/data/getAGEs")] = net::convert(rest::data::get_ages);
    functions[net::define(net::HTTPVerb::GET, "/data/getSettings")] = net::convert(rest::data::get_settings);
    functions[net::define(net::HTTPVerb::GET, "/data/getModData")] = net::convert(rest::data::get_mod_metadata);
    functions[net::define(net::HTTPVerb::GET, "/data/getMods")] = net::convert(rest::data::get_mods);
    functions[net::define(net::HTTPVerb::GET, "/data/getConfig")] = net::convert(rest::data::get_mod_config);
    functions[net::define(net::HTTPVerb::GET, "/data/getMod")] = net::convert(rest::data::get_mod);

    functions[net::define(net::HTTPVerb::GET, "/state/getGameState")] = net::convert(rest::state::get_gamestate);
    functions[net::define(net::HTTPVerb::GET, "/state/getUnitStates")] = net::convert(rest::state::get_unitstates);
    functions[net::define(net::HTTPVerb::GET, "/state/getTerrainStates")] = net::convert(rest::state::get_terrainstates);
    functions[net::define(net::HTTPVerb::GET, "/state/getPlayerStates")] = net::convert(rest::state::get_playerstates);
    functions[net::define(net::HTTPVerb::GET, "/state/getSettingState")] = net::convert(rest::state::get_settingstate);

    functions[net::define(net::HTTPVerb::GET, "/resource/getPack")] = net::convert(rest::resource::get_resource_pack);
    functions[net::define(net::HTTPVerb::GET, "/resource/getMetadata")] = net::convert(rest::resource::get_pack_metadata);
    functions[net::define(net::HTTPVerb::GET, "/resource/listPacks")] = net::convert(rest::resource::list_packs);
    
    functions[net::define(net::HTTPVerb::POST, "/data/uploadMod")] = rest::data::upload_mod;
    functions[net::define(net::HTTPVerb::POST, "/resource/uploadPack")] = rest::resource::upload_pack;

    server.start(functions);
    //server.printRequests(true);
    std::cout << "Listening for the following endpoints:" << std::endl;
    for (auto const& [definition, func] : functions) {
      auto const& [verb, name] = definition;
      std::cout << "  " << boost::describe::enum_to_string(verb, "unknown") << ":  " << name << std::endl;
    }

    signal(SIGTERM, terminationHandler);

    while (!terminated) {}
		
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
