#include <SQLUtil.h>
#include <dbs/DBTablesV0.0.1.h>

#include <format>
#include <iostream>
#include <map>
#include <print>
#include <ranges>
#include <string_view>

namespace mysql = boost::mysql;

namespace db::v0_0_1 {
namespace {
#ifdef NDEBUG
constexpr bool DEBUGGING = false;
#else
constexpr bool DEBUGGING = true;
#endif

using namespace db;
using namespace sqlutil;
DBErrorCode buildGeneralSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_GENERAL), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_GENERAL), results);
  if (auto ec = createTable(session, TABLE_META_COLUMNS, DB_GENERAL, TABLE_META); ec != DBErrorCode::NONE) {
    return ec;
  }
  auto metaColumnsStatement = createInsertColumns(TABLE_META_COLUMNS);
  auto [metaValueString, metaValues] = createInsertValues({{mysql::field{CURRENT_VERSION}, mysql::field{0}}});
  auto preparedStatement =
      session.connection.prepare_statement(std::format(SQL_INSERT, DB_GENERAL, TABLE_META, metaColumnsStatement, metaValueString));
  session.connection.execute(preparedStatement.bind(metaValues.begin(), metaValues.end()), results);
  return DBErrorCode::NONE;
}

DBErrorCode buildResourceSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_RESOURCE), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_RESOURCE), results);
  if (auto ec = createTable(session, TABLE_TEXT_RESOURCE_COLUMNS, DB_RESOURCE, TABLE_TEXT_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_IMAGE_RESOURCE_COLUMNS, DB_RESOURCE, TABLE_IMAGE_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_PACK_COLUMNS, DB_RESOURCE, TABLE_PACK); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}

DBErrorCode buildStateSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_STATE), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_STATE), results);
  if (auto ec = createTable(session, TABLE_STATE_GAME_COLUMNS, DB_STATE, TABLE_STATE_GAME); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_GAME_PLAYERORDER_COLUMNS, DB_STATE, TABLE_STATE_GAME_PLAYERORDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_PLAYER_COLUMNS, DB_STATE, TABLE_STATE_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_PLAYER_BANNEDUNITS_COLUMNS, DB_STATE, TABLE_STATE_PLAYER_BANNEDUNITS); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_UNIT_COLUMNS, DB_STATE, TABLE_STATE_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_UNIT_TRANSPORT_COLUMNS, DB_STATE, TABLE_STATE_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_TERRAIN_COLUMNS, DB_STATE, TABLE_STATE_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_SETTINGS_COLUMNS, DB_STATE, TABLE_STATE_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_STATE_SETTINGS_VARIANT_COLUMNS, DB_STATE, TABLE_STATE_SETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}
DBErrorCode buildDataSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_DATA), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_DATA), results);

  if (auto ec = createTable(session, TABLE_DATA_MOD_COLUMNS, DB_DATA, TABLE_DATA_MOD); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_MOD_DEFAULTPACK_COLUMNS, DB_DATA, TABLE_DATA_MOD_DEFAULTPACK); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_UNIT_COLUMNS, DB_DATA, TABLE_DATA_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_UNIT_WEAPON_COLUMNS, DB_DATA, TABLE_DATA_UNIT_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_UNIT_CLASSIFICATION_COLUMNS, DB_DATA, TABLE_DATA_UNIT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_UNIT_TRANSPORT_COLUMNS, DB_DATA, TABLE_DATA_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_WEAPON_COLUMNS, DB_DATA, TABLE_DATA_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_WEAPON_BASEDAMAGE_COLUMNS, DB_DATA, TABLE_DATA_WEAPON_BASEDAMAGE); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_WEAPON_STEALTHTARGET_COLUMNS, DB_DATA, TABLE_DATA_WEAPON_STEALTHTARGET); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_TERRAIN_COLUMNS, DB_DATA, TABLE_DATA_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_TERRAIN_BUILDREPAIR_COLUMNS, DB_DATA, TABLE_DATA_TERRAIN_BUILDREPAIR); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_TERRAIN_ACTIVATIONLIST_COLUMNS, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONLIST); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_TERRAIN_ACTIVATIONEFFECT_COLUMNS, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONEFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_COMMANDER_COLUMNS, DB_DATA, TABLE_DATA_COMMANDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_COMMANDER_EFFECT_COLUMNS, DB_DATA, TABLE_DATA_COMMANDER_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_MOVEMENT_COLUMNS, DB_DATA, TABLE_DATA_MOVEMENT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_MOVEMENT_COST_COLUMNS, DB_DATA, TABLE_DATA_MOVEMENT_COST); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_MOVEMENT_RULE_REF_COLUMNS, DB_DATA, TABLE_DATA_MOVEMENT_RULE_REF); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_MOVEMENT_RULE_COLUMNS, DB_DATA, TABLE_DATA_MOVEMENT_RULE); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_PLAYER_COLUMNS, DB_DATA, TABLE_DATA_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PLAYER_PERMITTEDPLAYERSLOT_COLUMNS, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDPLAYERSLOT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_COLUMNS, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDCOMMANDERTYPE); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_EFFECT_TARGET_COLUMNS, DB_DATA, TABLE_DATA_EFFECT_TARGET); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_EFFECT_AFFECT_COLUMNS, DB_DATA, TABLE_DATA_EFFECT_AFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_EFFECT_CLASSIFICATION_COLUMNS, DB_DATA, TABLE_DATA_EFFECT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_EFFECT_TERRAIN_COLUMNS, DB_DATA, TABLE_DATA_EFFECT_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session,TABLE_DATA_EFFECT_UNITTYPEREQUIRED_COLUMNS, DB_DATA, TABLE_DATA_EFFECT_UNITTYPEREQUIRED); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_PUE_COLUMNS, DB_DATA, TABLE_DATA_PASSIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_FIREPOWERFROMTERRAIN_COLUMNS, DB_DATA, TABLE_DATA_PUE_FIREPOWERFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_DEFENSEFROMTERRAIN_COLUMNS, DB_DATA, TABLE_DATA_PUE_DEFENSEFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_VISIONVARIANT_COLUMNS, DB_DATA, TABLE_DATA_PUE_VISIONVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_FIREPOWERVARIANT_COLUMNS, DB_DATA, TABLE_DATA_PUE_FIREPOWERVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_DEFENSEVARIANT_COLUMNS, DB_DATA, TABLE_DATA_PUE_DEFENSEVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PUE_INTEL_COLUMNS, DB_DATA, TABLE_DATA_PUE_INTEL); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_AUE_COLUMNS, DB_DATA, TABLE_DATA_ACTIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_PTE_COLUMNS, DB_DATA, TABLE_DATA_PASSIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PTE_BUILDLISTMOD_COLUMNS, DB_DATA, TABLE_DATA_PTE_BUILDLISTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_ATE_COLUMNS, DB_DATA, TABLE_DATA_ACTIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_PGE_COLUMNS, DB_DATA, TABLE_DATA_PASSIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_PGE_VARIANTHINTMOD_COLUMNS, DB_DATA, TABLE_DATA_PGE_VARIANTHINTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_AGE_COLUMNS, DB_DATA, TABLE_DATA_ACTIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_AGE_MISSILETARGETMETHOD_COLUMNS, DB_DATA, TABLE_DATA_AGE_MISSILETARGETMETHOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_DEFAULT_GAME_SETTINGS_COLUMNS, DB_DATA, TABLE_DATA_DEFAULT_GAME_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_SETTINGS_VARIANT_COLUMNS, DB_DATA, TABLE_DATA_DEFAULTSETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_CONFIG_COLUMNS, DB_DATA, TABLE_DATA_CONFIG); ec != DBErrorCode::NONE) {
    return ec;
  }

  if (auto ec = createTable(session, TABLE_DATA_MAP_DEFINITION_COLUMNS, DB_DATA, TABLE_DATA_MAP_DEFINITION); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_MAP_UNIT_COLUMNS, DB_DATA, TABLE_DATA_MAP_INITIAL_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  if (auto ec = createTable(session, TABLE_DATA_MAP_TERRAIN_COLUMNS, DB_DATA, TABLE_DATA_MAP_INITIAL_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}
}  // namespace

DBErrorCode buildDatabase() {
  Session session;
  Transaction transaction{session};
  DBErrorCode code = DBErrorCode::NONE;
  try {
    if (auto ec = buildGeneralSchema(session); ec != DBErrorCode::NONE) {
      return ec;
    }
    if (auto ec = buildResourceSchema(session); ec != DBErrorCode::NONE) {
      return ec;
    }
    if (auto ec = buildStateSchema(session); ec != DBErrorCode::NONE) {
      return ec;
    }
    if (auto ec = buildDataSchema(session); ec != DBErrorCode::NONE) {
      return ec;
    }
    transaction.commit();
  } catch (mysql::error_with_diagnostics const& err) {
    std::println(std::cerr, "DB Error: {} - {}", err.code().value(), err.what());
    std::println(std::cerr, "Client Message: {}", err.get_diagnostics().client_message());
    std::println(std::cerr, "Server Message: {}", err.get_diagnostics().server_message());
    code = DBErrorCode::UNKNOWN;
  }
  return code;
}
}  // namespace db::v0_0_1