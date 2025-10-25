#include<RestServer.h>
#include<RestFunctions.h>
#include<Properties.h>
#include<iostream>
#include<Cert.h>
#include<signal.h>
#include<RestFunctionUtility.h>

#include<DBFunctions.h>
#include<dbs/DBTablesCurrent.h>
#include<Cli.h>

#include<print>
#include<iostream>

static std::atomic_bool terminated = false;

void terminationHandler(int signal) {
  terminated = true;
}

int main(int argc, char const ** argv) {
  cli::CommandLineInterface commandLine{argc, argv};
  if (commandLine.isDBRebuildRequested()) {
    dbcurrent::buildDatabase();
  }
  try {
    properties::Properties const& props = properties::Properties::instance();
    auto dbVersion = db::check_db_version();
    if (dbVersion.has_value()) {
      auto version = *dbVersion;
      if (version != dbcurrent::CURRENT_VERSION) {
        std::println(std::cerr, "DB is out of date and requires reconstructing: Version {}", version);
        return -1;
      }
    } else {
      std::println(std::cerr, "DB Error, DB requires reconstructing.");
      return -1;
    }
    //Uncomment to reenable SSL
    //net::SSLCert sslCert{.key = cert::key, .cert = cert::certificate};
    //net::RestServer server{ "AWCustom Server", static_cast<uint16_t>(props.getInt("serverPort")), sslCert };
    net::RestServer server{ "AWCustom Server", static_cast<uint16_t>(props.getInt("serverPort"))};
    net::HTTPFunctionMap functions;
    functions[net::defineGet("/data/getUnits")] = net::convert(rest::data::get_units);
    functions[net::defineGet("/data/getWeapons")] = net::convert(rest::data::get_weapons);
    functions[net::defineGet("/data/getTerrains")] = net::convert(rest::data::get_terrains);
    functions[net::defineGet("/data/getMovements")] = net::convert(rest::data::get_movements);
    functions[net::defineGet("/data/getMovementRules")] = net::convert(rest::data::get_movement_rules);
    functions[net::defineGet("/data/getCommanders")] = net::convert(rest::data::get_commanders);
    functions[net::defineGet("/data/getPlayers")] = net::convert(rest::data::get_players);
    functions[net::defineGet("/data/getPUEs")] = net::convert(rest::data::get_pues);
    functions[net::defineGet("/data/getAUEs")] = net::convert(rest::data::get_aues);
    functions[net::defineGet("/data/getPTEs")] = net::convert(rest::data::get_ptes);
    functions[net::defineGet("/data/getATEs")] = net::convert(rest::data::get_ates);
    functions[net::defineGet("/data/getPGEs")] = net::convert(rest::data::get_pges);
    functions[net::defineGet("/data/getAGEs")] = net::convert(rest::data::get_ages);
    functions[net::defineGet("/data/getSettings")] = net::convert(rest::data::get_settings);
    functions[net::defineGet("/data/getModData")] = net::convert(rest::data::get_mod_metadata);
    functions[net::defineGet("/data/getMods")] = net::convert(rest::data::get_mods);
    functions[net::defineGet("/data/getConfig")] = net::convert(rest::data::get_mod_config);
    functions[net::defineGet("/data/getMod")] = net::convert(rest::data::get_mod);

    functions[net::defineGet("/state/getGameState")] = net::convert(rest::state::get_gamestate);
    functions[net::defineGet("/state/getUnitStates")] = net::convert(rest::state::get_unitstates);
    functions[net::defineGet("/state/getTerrainStates")] = net::convert(rest::state::get_terrainstates);
    functions[net::defineGet("/state/getPlayerStates")] = net::convert(rest::state::get_playerstates);
    functions[net::defineGet("/state/getSettingState")] = net::convert(rest::state::get_settingstate);

    functions[net::defineGet("/resource/getPack")] = net::convert(rest::resource::get_resource_pack);
    functions[net::defineGet("/resource/getMetadata")] = net::convert(rest::resource::get_pack_metadata);
    functions[net::defineGet("/resource/listPacks")] = net::convert(rest::resource::list_packs);
    
    functions[net::definePost("/data/uploadMod")] = rest::data::upload_mod;
    functions[net::definePost("/resource/uploadPack")] = rest::resource::upload_pack;

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
