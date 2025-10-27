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
  std::vector<Column> metaColumns = {COLUMN_META_VERSION, COLUMN_META_PROTECTED};
  if (auto ec = createTable(session, metaColumns, DB_GENERAL, TABLE_META); ec != DBErrorCode::NONE) {
    return ec;
  }
  auto metaColumnsStatement = createInsertColumns(metaColumns);
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
  std::vector<Column> textResourceColumns = TABLE_TEXT_RESOURCE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, textResourceColumns, DB_RESOURCE, TABLE_TEXT_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> imageResourceColumns = TABLE_IMAGE_RESOURCE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, imageResourceColumns, DB_RESOURCE, TABLE_IMAGE_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> packColumns = TABLE_PACK_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, packColumns, DB_RESOURCE, TABLE_PACK); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}

DBErrorCode buildStateSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_STATE), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_STATE), results);
  std::vector<Column> gameStateColumns = TABLE_STATE_GAME_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, gameStateColumns, DB_STATE, TABLE_STATE_GAME); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> gamePlayerOrderColumns = TABLE_STATE_GAME_PLAYERORDER_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, gamePlayerOrderColumns, DB_STATE, TABLE_STATE_GAME_PLAYERORDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerColumns = TABLE_STATE_PLAYER_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, playerColumns, DB_STATE, TABLE_STATE_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerBannedUnitsColumns = TABLE_STATE_PLAYER_BANNEDUNITS_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, playerBannedUnitsColumns, DB_STATE, TABLE_STATE_PLAYER_BANNEDUNITS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitColumns = TABLE_STATE_UNIT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitColumns, DB_STATE, TABLE_STATE_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitTransportColumns = TABLE_STATE_UNIT_TRANSPORT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitTransportColumns, DB_STATE, TABLE_STATE_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainColumns = TABLE_STATE_TERRAIN_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, terrainColumns, DB_STATE, TABLE_STATE_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsColumns = TABLE_STATE_SETTINGS_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, settingsColumns, DB_STATE, TABLE_STATE_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsVariantColumns = TABLE_STATE_SETTINGS_VARIANT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, settingsVariantColumns, DB_STATE, TABLE_STATE_SETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}
DBErrorCode buildDataSchema(Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_DATA), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_DATA), results);

  std::vector<Column> dataModColumns = TABLE_DATA_MOD_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, dataModColumns, DB_DATA, TABLE_DATA_MOD); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> dataDefaultPackColumns = TABLE_DATA_MOD_DEFAULTPACK_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, dataDefaultPackColumns, DB_DATA, TABLE_DATA_MOD_DEFAULTPACK); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> unitTypeColumns = TABLE_DATA_UNIT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitTypeColumns, DB_DATA, TABLE_DATA_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitWeaponColumns = TABLE_DATA_UNIT_WEAPON_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitWeaponColumns, DB_DATA, TABLE_DATA_UNIT_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitClassificationColumns = TABLE_DATA_UNIT_CLASSIFICATION_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitClassificationColumns, DB_DATA, TABLE_DATA_UNIT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitTransportColumns = TABLE_DATA_UNIT_TRANSPORT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, unitTransportColumns, DB_DATA, TABLE_DATA_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> weaponColumns = TABLE_DATA_WEAPON_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, weaponColumns, DB_DATA, TABLE_DATA_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> weaponBasedamageColumns = TABLE_DATA_WEAPON_BASEDAMAGE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, weaponBasedamageColumns, DB_DATA, TABLE_DATA_WEAPON_BASEDAMAGE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> weaponStealthTargetColumns = TABLE_DATA_WEAPON_STEALTHTARGET_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, weaponStealthTargetColumns, DB_DATA, TABLE_DATA_WEAPON_STEALTHTARGET); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> terrainColumns = TABLE_DATA_TERRAIN_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, terrainColumns, DB_DATA, TABLE_DATA_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainBuildRepairColumns = TABLE_DATA_TERRAIN_BUILDREPAIR_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, terrainBuildRepairColumns, DB_DATA, TABLE_DATA_TERRAIN_BUILDREPAIR); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainActivationListColumns = TABLE_DATA_TERRAIN_ACTIVATIONLIST_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, terrainActivationListColumns, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONLIST); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainActivationEffectColumns = TABLE_DATA_TERRAIN_ACTIVATIONEFFECT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, terrainActivationEffectColumns, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONEFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> commanderColumns = TABLE_DATA_COMMANDER_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, commanderColumns, DB_DATA, TABLE_DATA_COMMANDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> commanderEffectColumns = TABLE_DATA_COMMANDER_EFFECT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, commanderEffectColumns, DB_DATA, TABLE_DATA_COMMANDER_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> movementColumns = TABLE_DATA_MOVEMENT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, movementColumns, DB_DATA, TABLE_DATA_MOVEMENT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> movementCostColumns = TABLE_DATA_MOVEMENT_COST_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, movementCostColumns, DB_DATA, TABLE_DATA_MOVEMENT_COST); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> movementRuleRefColumns = TABLE_DATA_MOVEMENT_RULE_REF_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, movementRuleRefColumns, DB_DATA, TABLE_DATA_MOVEMENT_RULE_REF); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> movementRuleColumns = TABLE_DATA_MOVEMENT_RULE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, movementRuleColumns, DB_DATA, TABLE_DATA_MOVEMENT_RULE); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> playerColumns = TABLE_DATA_PLAYER_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, playerColumns, DB_DATA, TABLE_DATA_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerPermittedSlotColumns = TABLE_DATA_PLAYER_PERMITTEDPLAYERSLOT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, playerPermittedSlotColumns, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDPLAYERSLOT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerPermittedCommanderColumns = TABLE_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, playerPermittedCommanderColumns, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDCOMMANDERTYPE); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> effectTargetColumns = TABLE_DATA_EFFECT_TARGET_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, effectTargetColumns, DB_DATA, TABLE_DATA_EFFECT_TARGET); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectAffectColumns = TABLE_DATA_EFFECT_AFFECT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, effectAffectColumns, DB_DATA, TABLE_DATA_EFFECT_AFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectClassificationColumns = TABLE_DATA_EFFECT_CLASSIFICATION_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, effectClassificationColumns, DB_DATA, TABLE_DATA_EFFECT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectTerrainColumns = TABLE_DATA_EFFECT_TERRAIN_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, effectTerrainColumns, DB_DATA, TABLE_DATA_EFFECT_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectUnitTypeColumns = TABLE_DATA_EFFECT_UNITTYPEREQUIRED_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, effectUnitTypeColumns, DB_DATA, TABLE_DATA_EFFECT_UNITTYPEREQUIRED); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pueColumns = TABLE_DATA_PUE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueColumns, DB_DATA, TABLE_DATA_PASSIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueFirepowerTerrainColumns = TABLE_DATA_PUE_FIREPOWERFROMTERRAIN_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueFirepowerTerrainColumns, DB_DATA, TABLE_DATA_PUE_FIREPOWERFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueDefenseTerrainColumns = TABLE_DATA_PUE_DEFENSEFROMTERRAIN_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueDefenseTerrainColumns, DB_DATA, TABLE_DATA_PUE_DEFENSEFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueVisionVariantColumns = TABLE_DATA_PUE_VISIONVARIANT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueVisionVariantColumns, DB_DATA, TABLE_DATA_PUE_VISIONVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueFirepowerVariantColumns = TABLE_DATA_PUE_FIREPOWERVARIANT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueFirepowerVariantColumns, DB_DATA, TABLE_DATA_PUE_FIREPOWERVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueDefenseVariantColumns = TABLE_DATA_PUE_DEFENSEVARIANT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueDefenseVariantColumns, DB_DATA, TABLE_DATA_PUE_DEFENSEVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueIntelColumns = TABLE_DATA_PUE_INTEL_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pueIntelColumns, DB_DATA, TABLE_DATA_PUE_INTEL); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> aueColumns = TABLE_DATA_AUE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, aueColumns, DB_DATA, TABLE_DATA_ACTIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pteColumns = TABLE_DATA_PTE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pteColumns, DB_DATA, TABLE_DATA_PASSIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pteBuildListColumns = TABLE_DATA_PTE_BUILDLISTMOD_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pteBuildListColumns, DB_DATA, TABLE_DATA_PTE_BUILDLISTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> ateColumns = TABLE_DATA_ATE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, ateColumns, DB_DATA, TABLE_DATA_ACTIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pgeColumns = TABLE_DATA_PGE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pgeColumns, DB_DATA, TABLE_DATA_PASSIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pgeVariantHintColumns = TABLE_DATA_PGE_VARIANTHINTMOD_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, pgeVariantHintColumns, DB_DATA, TABLE_DATA_PGE_VARIANTHINTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> ageColumns = TABLE_DATA_AGE_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, ageColumns, DB_DATA, TABLE_DATA_ACTIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> ageMissileTargetColumns = TABLE_DATA_AGE_MISSILETARGETMETHOD_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, ageMissileTargetColumns, DB_DATA, TABLE_DATA_AGE_MISSILETARGETMETHOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> settingsColumns = TABLE_DATA_DEFAULT_GAME_SETTINGS_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, settingsColumns, DB_DATA, TABLE_DATA_DEFAULT_GAME_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsVariantColumns = TABLE_DATA_SETTINGS_VARIANT_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, settingsVariantColumns, DB_DATA, TABLE_DATA_DEFAULTSETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> configColumns = TABLE_DATA_CONFIG_COLUMNS | std::ranges::to<std::vector<Column>>();
  if (auto ec = createTable(session, configColumns, DB_DATA, TABLE_DATA_CONFIG); ec != DBErrorCode::NONE) {
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