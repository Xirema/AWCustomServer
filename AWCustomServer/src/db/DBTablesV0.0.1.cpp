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

using namespace std::string_literals;
std::string createInsertColumns(std::vector<std::string_view> const& columns) {
  return columns | std::views::transform([](std::string_view const& column) { return std::format("`{}`", column); }) | std::views::join_with(","s) |
         std::ranges::to<std::string>();
}

using ParameterPack = std::vector<mysql::field>;

std::string parameterizeField(mysql::field const& field) { return "?"; }

std::string parameterizePack(ParameterPack const& pack) {
  std::string parameterizedFields = pack | std::views::transform(parameterizeField) | std::views::join_with(',') | std::ranges::to<std::string>();
  return std::format("({})", parameterizedFields);
}

std::pair<std::string, ParameterPack> createInsertValues(std::vector<ParameterPack> const& values) {
  for (auto const& [a, b] : values | std::views::adjacent<2>) {
    if (a.size() != b.size()) {
      throw std::runtime_error("Parameter Packs aren't the same size!");
    }
  }
  ParameterPack arguments = values | std::views::join | std::ranges::to<ParameterPack>();
  std::string argumentString = values | std::views::transform(parameterizePack) | std::views::join_with(","s) | std::ranges::to<std::string>();
  return std::make_pair(argumentString, arguments);
}

std::string columnCreateString(Column const& column) {
  return std::format(SQL_CREATE_COLUMN, column.columnName, column.columnType, column.nullable ? "" : "not",
                     column.autoIncrement ? "auto_increment" : "");
}

std::string createColumns(std::vector<Column>& columns) {
  Column* primaryKey = nullptr;
  for (auto& column : columns) {
    if (primaryKey) {
      column.autoIncrement = false;
    }
    if (column.autoIncrement) {
      primaryKey = &column;
    }
  }
  std::string retString = columns | std::views::transform(columnCreateString) | std::views::join_with(", "s) | std::ranges::to<std::string>();
  if (primaryKey) {
    retString += ", "s + std::format(SQL_DEFINE_PRIMARY_KEY, primaryKey->columnName);
  }
  return retString;
}

DBErrorCode createTable(sqlutil::Session& session, std::vector<Column>& columns, std::string_view schema, std::string_view table) {
  mysql::results results;
  std::string columnDefines = createColumns(columns);
  session.connection.execute(std::format(SQL_CREATE_TABLE, schema, table, columnDefines), results);
  return DBErrorCode::NONE;
}

DBErrorCode buildGeneralSchema(sqlutil::Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_GENERAL), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_GENERAL), results);
  std::vector<Column> metaColumns = {COLUMN_META_VERSION, COLUMN_META_PROTECTED};
  if (auto ec = createTable(session, metaColumns, DB_GENERAL, TABLE_META); ec != DBErrorCode::NONE) {
    return ec;
  }
  auto metaColumnsStatement = createInsertColumns({COLUMN_META_VERSION.columnName, COLUMN_META_PROTECTED.columnName});
  auto [metaValueString, metaValues] = createInsertValues({{mysql::field{CURRENT_VERSION}, mysql::field{0}}});
  auto preparedStatement =
      session.connection.prepare_statement(std::format(SQL_INSERT, DB_GENERAL, TABLE_META, metaColumnsStatement, metaValueString));
  session.connection.execute(preparedStatement.bind(metaValues.begin(), metaValues.end()), results);
  return DBErrorCode::NONE;
}

DBErrorCode buildResourceSchema(sqlutil::Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_RESOURCE), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_RESOURCE), results);
  std::vector<Column> textResourceColumns = {COLUMN_TEXTRESOURCE_ID,          COLUMN_TEXTRESOURCE_PACK_ID,   COLUMN_TEXTRESOURCE_KEY,
                                             COLUMN_TEXTRESOURCE_TYPE,        COLUMN_TEXTRESOURCE_SHORTNAME, COLUMN_TEXTRESOURCE_LONGNAME,
                                             COLUMN_TEXTRESOURCE_DESCRIPTION, COLUMN_TEXTRESOURCE_LANGUAGE};
  if (auto ec = createTable(session, textResourceColumns, DB_RESOURCE, TABLE_TEXT_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> imageResourceColumns = {
      COLUMN_IMAGERESOURCE_ID,         COLUMN_IMAGERESOURCE_PACK_ID,        COLUMN_IMAGERESOURCE_KEY,
      COLUMN_IMAGERESOURCE_TYPE,       COLUMN_IMAGERESOURCE_ARMYCOLOR,      COLUMN_IMAGERESOURCE_ORIENTATION,
      COLUMN_IMAGERESOURCE_VARIANT,    COLUMN_IMAGERESOURCE_ORDER,          COLUMN_IMAGERESOURCE_SMALLIMAGE,
      COLUMN_IMAGERESOURCE_LARGEIMAGE, COLUMN_IMAGERESOURCE_SMALLIMAGEBLOB, COLUMN_IMAGERESOURCE_LARGEIMAGEBLOB};
  if (auto ec = createTable(session, imageResourceColumns, DB_RESOURCE, TABLE_IMAGE_RESOURCE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> packColumns = {COLUMN_RESOURCEPACK_PACK_ID, COLUMN_RESOURCEPACK_NAME,    COLUMN_RESOURCEPACK_VERSION,
                                     COLUMN_RESOURCEPACK_CREATED, COLUMN_RESOURCEPACK_EXPIRED, COLUMN_RESOURCEPACK_PROTOCOL};
  if (auto ec = createTable(session, packColumns, DB_RESOURCE, TABLE_PACK); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}

DBErrorCode buildStateSchema(sqlutil::Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_STATE), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_STATE), results);
  std::vector<Column> gameStateColumns = {COLUMN_STATE_GAME_ID,      COLUMN_STATE_GAME_DAY,          COLUMN_STATE_GAME_PLAYER_TURN,
                                          COLUMN_STATE_GAME_VARIANT, COLUMN_STATE_GAME_VARIANTSTART, COLUMN_STATE_GAME_ACTIVE,
                                          COLUMN_STATE_GAME_HASH,    COLUMN_STATE_GAME_LANGUAGE};
  if (auto ec = createTable(session, gameStateColumns, DB_STATE, TABLE_STATE_GAME); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> gamePlayerOrderColumns = {COLUMN_STATE_GAME_PLAYERORDER_ID, COLUMN_STATE_GAME_PLAYERORDER_GAME_ID,
                                                COLUMN_STATE_GAME_PLAYERORDER_PLAYER, COLUMN_STATE_GAME_PLAYERORDER_ORDER};
  if (auto ec = createTable(session, gamePlayerOrderColumns, DB_STATE, TABLE_STATE_GAME_PLAYERORDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerColumns = {COLUMN_STATE_PLAYER_ID,
                                       COLUMN_STATE_PLAYER_GAME_ID,
                                       COLUMN_STATE_PLAYER_USER,
                                       COLUMN_STATE_PLAYER_COMMANDER_NAME,
                                       COLUMN_STATE_PLAYER_FUNDS,
                                       COLUMN_STATE_PLAYER_POWER_CHARGE,
                                       COLUMN_STATE_PLAYER_ARMY_COLOR,
                                       COLUMN_STATE_PLAYER_ALIVE,
                                       COLUMN_STATE_PLAYER_TOTAL_POWER_USES,
                                       COLUMN_STATE_PLAYER_UNIT_FACING,
                                       COLUMN_STATE_PLAYER_PLAYER_TYPE,
                                       COLUMN_STATE_PLAYER_TEAM,
                                       COLUMN_STATE_PLAYER_POWER_ACTIVE,
                                       COLUMN_STATE_PLAYER_POWER_ACTIVE_DAY,
                                       COLUMN_STATE_PLAYER_INCOME_MULTIPLIER,
                                       COLUMN_STATE_PLAYER_CO_METER_MULTIPLIER};
  if (auto ec = createTable(session, playerColumns, DB_STATE, TABLE_STATE_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerBannedUnitsColumns = {COLUMN_STATE_PLAYER_BANNEDUNITS_ID, COLUMN_STATE_PLAYER_BANNEDUNITS_GAME_ID,
                                                  COLUMN_STATE_PLAYER_BANNEDUNITS_PLAYER_ID, COLUMN_STATE_PLAYER_BANNEDUNITS_UNIT_NAME};
  if (auto ec = createTable(session, playerBannedUnitsColumns, DB_STATE, TABLE_STATE_PLAYER_BANNEDUNITS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitColumns = {COLUMN_STATE_UNIT_ID,
                                     COLUMN_STATE_UNIT_GAME_ID,
                                     COLUMN_STATE_UNIT_X,
                                     COLUMN_STATE_UNIT_Y,
                                     COLUMN_STATE_UNIT_NAME,
                                     COLUMN_STATE_UNIT_AMMO,
                                     COLUMN_STATE_UNIT_FUEL,
                                     COLUMN_STATE_UNIT_ACTIVE,
                                     COLUMN_STATE_UNIT_STUNNED,
                                     COLUMN_STATE_UNIT_OWNER,
                                     COLUMN_STATE_UNIT_HIT_POINTS,
                                     COLUMN_STATE_UNIT_STEALTHED,
                                     COLUMN_STATE_UNIT_CURRENT_GOOD_LUCK,
                                     COLUMN_STATE_UNIT_CURRENT_BAD_LUCK};
  if (auto ec = createTable(session, unitColumns, DB_STATE, TABLE_STATE_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitTransportColumns = {COLUMN_STATE_UNIT_TRANSPORT_ID, COLUMN_STATE_UNIT_TRANSPORT_GAME_ID,
                                              COLUMN_STATE_UNIT_TRANSPORT_TRANSPORT_ID, COLUMN_STATE_UNIT_TRANSPORT_UNIT_ID,
                                              COLUMN_STATE_UNIT_TRANSPORT_ORDER};
  if (auto ec = createTable(session, unitTransportColumns, DB_STATE, TABLE_STATE_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainColumns = {COLUMN_STATE_TERRAIN_ID,
                                        COLUMN_STATE_TERRAIN_GAME_ID,
                                        COLUMN_STATE_TERRAIN_X,
                                        COLUMN_STATE_TERRAIN_Y,
                                        COLUMN_STATE_TERRAIN_NAME,
                                        COLUMN_STATE_TERRAIN_ORIENTATION,
                                        COLUMN_STATE_TERRAIN_CAPTURE_POINTS,
                                        COLUMN_STATE_TERRAIN_ACTIVATION_COUNT,
                                        COLUMN_STATE_TERRAIN_OWNER,
                                        COLUMN_STATE_TERRAIN_HIT_POINTS,
                                        COLUMN_STATE_TERRAIN_STEALTHED,
                                        COLUMN_STATE_TERRAIN_CURRENT_GOOD_LUCK,
                                        COLUMN_STATE_TERRAIN_CURRENT_BAD_LUCK};
  if (auto ec = createTable(session, terrainColumns, DB_STATE, TABLE_STATE_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsColumns = {COLUMN_STATE_SETTINGS_ID,
                                         COLUMN_STATE_SETTINGS_GAME_ID,
                                         COLUMN_STATE_SETTINGS_STARTING_FUNDS,
                                         COLUMN_STATE_SETTINGS_INCOME_MULTIPLIER,
                                         COLUMN_STATE_SETTINGS_FOG_OF_WAR,
                                         COLUMN_STATE_SETTINGS_CO_POWERS,
                                         COLUMN_STATE_SETTINGS_TEAMS,
                                         COLUMN_STATE_SETTINGS_MOD_ID,
                                         COLUMN_STATE_SETTINGS_CO_METER_SIZE,
                                         COLUMN_STATE_SETTINGS_CO_METER_MULTIPLIER,
                                         COLUMN_STATE_SETTINGS_UNIT_LIMIT,
                                         COLUMN_STATE_SETTINGS_CAPTURE_LIMIT,
                                         COLUMN_STATE_SETTINGS_DAY_LIMIT};
  if (auto ec = createTable(session, settingsColumns, DB_STATE, TABLE_STATE_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsVariantColumns = {COLUMN_STATE_SETTINGS_VARIANT_ID, COLUMN_STATE_SETTINGS_VARIANT_GAME_ID,
                                                COLUMN_STATE_SETTINGS_VARIANT_VARIANT, COLUMN_STATE_SETTINGS_VARIANT_WEIGHT};
  if (auto ec = createTable(session, settingsVariantColumns, DB_STATE, TABLE_STATE_SETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}
DBErrorCode buildDataSchema(sqlutil::Session& session) {
  mysql::results results;
  session.connection.execute(std::format(SQL_DROP_SCHEMA, DB_DATA), results);
  session.connection.execute(std::format(SQL_CREATE_SCHEMA, DB_DATA), results);

  std::vector<Column> dataModColumns = {
      COLUMN_DATA_MOD_ID,
      COLUMN_DATA_MOD_NAME,
      COLUMN_DATA_MOD_VERSION,
      COLUMN_DATA_MOD_EXPIRED,
  };
  if (auto ec = createTable(session, dataModColumns, DB_DATA, TABLE_DATA_MOD); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> dataDefaultPackColumns = {
      COLUMN_DATA_MOD_DEFAULTPACK_ID,    COLUMN_DATA_MOD_DEFAULTPACK_MOD_ID,  COLUMN_DATA_MOD_DEFAULTPACK_NAME,
      COLUMN_DATA_MOD_DEFAULTPACK_ORDER, COLUMN_DATA_MOD_DEFAULTPACK_VERSION,
  };
  if (auto ec = createTable(session, dataDefaultPackColumns, DB_DATA, TABLE_DATA_MOD_DEFAULTPACK); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> unitTypeColumns = {
      COLUMN_DATA_UNIT_ID,
      COLUMN_DATA_UNIT_MOD_ID,
      COLUMN_DATA_UNIT_NAME,
      COLUMN_DATA_UNIT_COST,
      COLUMN_DATA_UNIT_MAXFUEL,
      COLUMN_DATA_UNIT_MAXAMMO,
      COLUMN_DATA_UNIT_VISIONRANGE,
      COLUMN_DATA_UNIT_MOVEMENTCLASS,
      COLUMN_DATA_UNIT_MOVEMENTRANGE,
      COLUMN_DATA_UNIT_FUELPERDAY,
      COLUMN_DATA_UNIT_FUELPERDAYSTEALTH,
      COLUMN_DATA_UNIT_SUPPLYREPAIR,
      COLUMN_DATA_UNIT_TRANSPORTCAPACITY,
      COLUMN_DATA_UNIT_HITPOINTS,
      COLUMN_DATA_UNIT_CAPTURESPEED,
      COLUMN_DATA_UNIT_IGNORESVISIONOCCLUSION,
      COLUMN_DATA_UNIT_STEALTHTYPE,
      COLUMN_DATA_UNIT_STATIONARYFIRE,
  };
  if (auto ec = createTable(session, unitTypeColumns, DB_DATA, TABLE_DATA_UNIT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitWeaponColumns = {
      COLUMN_DATA_UNIT_WEAPON_ID,   COLUMN_DATA_UNIT_WEAPON_MOD_ID, COLUMN_DATA_UNIT_WEAPON_UNITNAME,
      COLUMN_DATA_UNIT_WEAPON_NAME, COLUMN_DATA_UNIT_WEAPON_ORDER,
  };
  if (auto ec = createTable(session, unitWeaponColumns, DB_DATA, TABLE_DATA_UNIT_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitClassificationColumns = {
      COLUMN_DATA_UNIT_CLASSIFICATION_ID,
      COLUMN_DATA_UNIT_CLASSIFICATION_MOD_ID,
      COLUMN_DATA_UNIT_CLASSIFICATION_UNITNAME,
      COLUMN_DATA_UNIT_CLASSIFICATION_NAME,
  };
  if (auto ec = createTable(session, unitClassificationColumns, DB_DATA, TABLE_DATA_UNIT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> unitTransportColumns = {
      COLUMN_DATA_UNIT_TRANSPORT_ID,
      COLUMN_DATA_UNIT_TRANSPORT_MOD_ID,
      COLUMN_DATA_UNIT_TRANSPORT_UNITNAME,
      COLUMN_DATA_UNIT_TRANSPORT_NAME,
  };
  if (auto ec = createTable(session, unitTransportColumns, DB_DATA, TABLE_DATA_UNIT_TRANSPORT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> weaponColumns = {
      COLUMN_DATA_WEAPON_ID,        COLUMN_DATA_WEAPON_MOD_ID,       COLUMN_DATA_WEAPON_NAME,       COLUMN_DATA_WEAPON_AMMOCONSUMED,
      COLUMN_DATA_WEAPON_MAXRANGE,  COLUMN_DATA_WEAPON_MINRANGE,     COLUMN_DATA_WEAPON_SELFTARGET, COLUMN_DATA_WEAPON_AFFECTEDBYLUCK,
      COLUMN_DATA_WEAPON_NONLETHAL, COLUMN_DATA_WEAPON_AREAOFEFFECT, COLUMN_DATA_WEAPON_FLATDAMAGE,
  };
  if (auto ec = createTable(session, weaponColumns, DB_DATA, TABLE_DATA_WEAPON); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> weaponBasedamageColumns = {
      COLUMN_DATA_WEAPON_BASEDAMAGE_ID,       COLUMN_DATA_WEAPON_BASEDAMAGE_MOD_ID, COLUMN_DATA_WEAPON_BASEDAMAGE_NAME,
      COLUMN_DATA_WEAPON_BASEDAMAGE_UNITNAME, COLUMN_DATA_WEAPON_BASEDAMAGE_DAMAGE,
  };
  if (auto ec = createTable(session, weaponBasedamageColumns, DB_DATA, TABLE_DATA_WEAPON_BASEDAMAGE); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> weaponStealthTargetColumns = {
      COLUMN_DATA_WEAPON_STEALTHTARGET_ID,
      COLUMN_DATA_WEAPON_STEALTHTARGET_MOD_ID,
      COLUMN_DATA_WEAPON_STEALTHTARGET_WEAPON,
      COLUMN_DATA_WEAPON_STEALTHTARGET_STEALTHTYPE,
  };
  if (auto ec = createTable(session, weaponStealthTargetColumns, DB_DATA, TABLE_DATA_WEAPON_STEALTHTARGET); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> terrainColumns = {
      COLUMN_DATA_TERRAIN_ID,
      COLUMN_DATA_TERRAIN_MOD_ID,
      COLUMN_DATA_TERRAIN_NAME,
      COLUMN_DATA_TERRAIN_STARS,
      COLUMN_DATA_TERRAIN_MAXCAPTUREPOINTS,
      COLUMN_DATA_TERRAIN_SAMEAS,
      COLUMN_DATA_TERRAIN_INCOME,
      COLUMN_DATA_TERRAIN_REPAIR,
      COLUMN_DATA_TERRAIN_OCCLUDESVISION,
      COLUMN_DATA_TERRAIN_HITPOINTS,
      COLUMN_DATA_TERRAIN_DESTROYED,
      COLUMN_DATA_TERRAIN_DAMAGEDLIKE,
      COLUMN_DATA_TERRAIN_ACTIVATIONMAX,
      COLUMN_DATA_TERRAIN_ACTIVATIONCHANGE,
      COLUMN_DATA_TERRAIN_LOSEIFCAPTURED,
      COLUMN_DATA_TERRAIN_LOSEIFALLCAPTURED,
      COLUMN_DATA_TERRAIN_DESTROYEDORIENTATION,
  };
  if (auto ec = createTable(session, terrainColumns, DB_DATA, TABLE_DATA_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainBuildRepairColumns = {
      COLUMN_DATA_TERRAIN_BUILDREPAIR_ID,        COLUMN_DATA_TERRAIN_BUILDREPAIR_MOD_ID,     COLUMN_DATA_TERRAIN_BUILDREPAIR_TERRAIN_NAME,
      COLUMN_DATA_TERRAIN_BUILDREPAIR_UNIT_NAME, COLUMN_DATA_TERRAIN_BUILDREPAIR_UNIT_ORDER, COLUMN_DATA_TERRAIN_BUILDREPAIR_TYPE,
  };
  if (auto ec = createTable(session, terrainBuildRepairColumns, DB_DATA, TABLE_DATA_TERRAIN_BUILDREPAIR); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainActivationListColumns = {
      COLUMN_DATA_TERRAIN_ACTIVATIONLIST_ID,
      COLUMN_DATA_TERRAIN_ACTIVATIONLIST_MOD_ID,
      COLUMN_DATA_TERRAIN_ACTIVATIONLIST_TERRAIN_NAME,
      COLUMN_DATA_TERRAIN_ACTIVATIONLIST_UNIT_NAME,
  };
  if (auto ec = createTable(session, terrainActivationListColumns, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONLIST); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> terrainActivationEffectColumns = {
      COLUMN_DATA_TERRAIN_ACTIVATIONEFFECT_ID,           COLUMN_DATA_TERRAIN_ACTIVATIONEFFECT_MOD_ID,
      COLUMN_DATA_TERRAIN_ACTIVATIONEFFECT_TERRAIN_NAME, COLUMN_DATA_TERRAIN_ACTIVATIONEFFECT_EFFECT_TYPE,
      COLUMN_DATA_TERRAIN_ACTIVATIONEFFECT_EFFECT_NAME,
  };
  if (auto ec = createTable(session, terrainActivationEffectColumns, DB_DATA, TABLE_DATA_TERRAIN_ACTIVATIONEFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> commanderColumns = {
      COLUMN_DATA_COMMANDER_ID,       COLUMN_DATA_COMMANDER_MOD_ID,   COLUMN_DATA_COMMANDER_NAME,
      COLUMN_DATA_COMMANDER_COPCOST,  COLUMN_DATA_COMMANDER_SCOPCOST, COLUMN_DATA_COMMANDER_COMETERMULTIPLIER,
      COLUMN_DATA_COMMANDER_PLAYABLE,
  };
  if (auto ec = createTable(session, commanderColumns, DB_DATA, TABLE_DATA_COMMANDER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> commanderEffectColumns = {
      COLUMN_DATA_COMMANDER_EFFECT_ID,   COLUMN_DATA_COMMANDER_EFFECT_MOD_ID, COLUMN_DATA_COMMANDER_EFFECT_COMMANDER_NAME,
      COLUMN_DATA_COMMANDER_EFFECT_NAME, COLUMN_DATA_COMMANDER_EFFECT_TYPE,   COLUMN_DATA_COMMANDER_EFFECT_CATEGORY,
  };
  if (auto ec = createTable(session, commanderEffectColumns, DB_DATA, TABLE_DATA_COMMANDER_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> movementColumns = {
      COLUMN_DATA_MOVEMENT_ID,
      COLUMN_DATA_MOVEMENT_MOD_ID,
      COLUMN_DATA_MOVEMENT_NAME,
  };
  if (auto ec = createTable(session, movementColumns, DB_DATA, TABLE_DATA_MOVEMENT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> movementCostColumns = {
      COLUMN_DATA_MOVEMENT_COST_ID,      COLUMN_DATA_MOVEMENT_COST_MOD_ID, COLUMN_DATA_MOVEMENT_COST_NAME,
      COLUMN_DATA_MOVEMENT_COST_TERRAIN, COLUMN_DATA_MOVEMENT_COST_COST,   COLUMN_DATA_MOVEMENT_COST_VARIANT,
  };
  if (auto ec = createTable(session, movementCostColumns, DB_DATA, TABLE_DATA_MOVEMENT_COST); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> movementRuleRefColumns = {
      COLUMN_DATA_MOVEMENT_RULE_REF_ID,   COLUMN_DATA_MOVEMENT_RULE_REF_MOD_ID, COLUMN_DATA_MOVEMENT_RULE_REF_NAME,
      COLUMN_DATA_MOVEMENT_RULE_REF_RULE, COLUMN_DATA_MOVEMENT_RULE_REF_ORDER,
  };
  if (auto ec = createTable(session, movementRuleRefColumns, DB_DATA, TABLE_DATA_MOVEMENT_RULE_REF); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> movementRuleColumns = {
      COLUMN_DATA_MOVEMENT_RULE_ID,         COLUMN_DATA_MOVEMENT_RULE_MOD_ID,    COLUMN_DATA_MOVEMENT_RULE_NAME,
      COLUMN_DATA_MOVEMENT_RULE_TYPE,       COLUMN_DATA_MOVEMENT_RULE_MAXREPEAT, COLUMN_DATA_MOVEMENT_RULE_CONFIRMTYPE,
      COLUMN_DATA_MOVEMENT_RULE_STOPIFUSED,
  };
  if (auto ec = createTable(session, movementRuleColumns, DB_DATA, TABLE_DATA_MOVEMENT_RULE); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> playerColumns = {
      COLUMN_DATA_PLAYER_ID, COLUMN_DATA_PLAYER_MOD_ID, COLUMN_DATA_PLAYER_NAME, COLUMN_DATA_PLAYER_COMMANDERMOD, COLUMN_DATA_PLAYER_TEAM,
  };
  if (auto ec = createTable(session, playerColumns, DB_DATA, TABLE_DATA_PLAYER); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerPermittedSlotColumns = {
      COLUMN_DATA_PLAYER_PERMITTEDPLAYERSLOT_ID,
      COLUMN_DATA_PLAYER_PERMITTEDPLAYERSLOT_MOD_ID,
      COLUMN_DATA_PLAYER_PERMITTEDPLAYERSLOT_NAME,
      COLUMN_DATA_PLAYER_PERMITTEDPLAYERSLOT_SLOT,
  };
  if (auto ec = createTable(session, playerPermittedSlotColumns, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDPLAYERSLOT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> playerPermittedCommanderColumns = {
      COLUMN_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_ID,
      COLUMN_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_MOD_ID,
      COLUMN_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_NAME,
      COLUMN_DATA_PLAYER_PERMITTEDCOMMANDERTYPE_COMMANDER,
  };
  if (auto ec = createTable(session, playerPermittedCommanderColumns, DB_DATA, TABLE_DATA_PLAYER_PERMITTEDCOMMANDERTYPE); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> effectTargetColumns = {
      COLUMN_DATA_EFFECT_TARGET_ID,   COLUMN_DATA_EFFECT_TARGET_MOD_ID, COLUMN_DATA_EFFECT_TARGET_EFFECT_NAME,
      COLUMN_DATA_EFFECT_TARGET_TYPE, COLUMN_DATA_EFFECT_TARGET_TARGET,
  };
  if (auto ec = createTable(session, effectTargetColumns, DB_DATA, TABLE_DATA_EFFECT_TARGET); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectAffectColumns = {
      COLUMN_DATA_EFFECT_AFFECT_ID,   COLUMN_DATA_EFFECT_AFFECT_MOD_ID,  COLUMN_DATA_EFFECT_AFFECT_EFFECT_NAME,
      COLUMN_DATA_EFFECT_AFFECT_TYPE, COLUMN_DATA_EFFECT_AFFECT_AFFECTS,
  };
  if (auto ec = createTable(session, effectAffectColumns, DB_DATA, TABLE_DATA_EFFECT_AFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectClassificationColumns = {
      COLUMN_DATA_EFFECT_CLASSIFICATION_ID,   COLUMN_DATA_EFFECT_CLASSIFICATION_MOD_ID,         COLUMN_DATA_EFFECT_CLASSIFICATION_EFFECT_NAME,
      COLUMN_DATA_EFFECT_CLASSIFICATION_TYPE, COLUMN_DATA_EFFECT_CLASSIFICATION_CLASSIFICATION,
  };
  if (auto ec = createTable(session, effectClassificationColumns, DB_DATA, TABLE_DATA_EFFECT_CLASSIFICATION); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectTerrainColumns = {
      COLUMN_DATA_EFFECT_TERRAIN_ID,   COLUMN_DATA_EFFECT_TERRAIN_MOD_ID,  COLUMN_DATA_EFFECT_TERRAIN_EFFECT_NAME,
      COLUMN_DATA_EFFECT_TERRAIN_TYPE, COLUMN_DATA_EFFECT_TERRAIN_TERRAIN,
  };
  if (auto ec = createTable(session, effectTerrainColumns, DB_DATA, TABLE_DATA_EFFECT_TERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> effectUnitTypeColumns = {
      COLUMN_DATA_EFFECT_UNITTYPEREQUIRED_ID,   COLUMN_DATA_EFFECT_UNITTYPEREQUIRED_MOD_ID, COLUMN_DATA_EFFECT_UNITTYPEREQUIRED_EFFECT_NAME,
      COLUMN_DATA_EFFECT_UNITTYPEREQUIRED_TYPE, COLUMN_DATA_EFFECT_UNITTYPEREQUIRED_UNIT,
  };
  if (auto ec = createTable(session, effectUnitTypeColumns, DB_DATA, TABLE_DATA_EFFECT_UNITTYPEREQUIRED); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pueColumns = {
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_ID,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_MOD_ID,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_NAME,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_FIREPOWER,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_DEFENSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_INDIRECTDEFENSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_MINRANGE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_MAXRANGE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_FUELUSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_AMMOUSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_GOODLUCK,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_BADLUCK,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_MOVEMENT,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_VISION,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINSTARS,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINSTARSFLAT,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINDEFENSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINFIREPOWER,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_COUNTERFIRE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_COUNTERFIRST,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_CAPTURERATE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_UNITCOST,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_FIREPOWERFROMFUNDS,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_DEFENSEFROMFUNDS,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_FUNDSFROMDAMAGE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_COMETERCHARGEFROMDEALTDAMAGE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_COMETERCHARGEFROMRECEIVEDDAMAGE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINSTARSFLATDEFENSE,
      COLUMN_DATA_PASSIVE_UNIT_EFFECT_TERRAINSTARSFLATFIREPOWER,
  };
  if (auto ec = createTable(session, pueColumns, DB_DATA, TABLE_DATA_PASSIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueFirepowerTerrainColumns = {
      COLUMN_DATA_PUE_FIREPOWERFROMTERRAIN_ID,      COLUMN_DATA_PUE_FIREPOWERFROMTERRAIN_MOD_ID,    COLUMN_DATA_PUE_FIREPOWERFROMTERRAIN_EFFECT_NAME,
      COLUMN_DATA_PUE_FIREPOWERFROMTERRAIN_TERRAIN, COLUMN_DATA_PUE_FIREPOWERFROMTERRAIN_FIREPOWER,
  };
  if (auto ec = createTable(session, pueFirepowerTerrainColumns, DB_DATA, TABLE_DATA_PUE_FIREPOWERFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueDefenseTerrainColumns = {
      COLUMN_DATA_PUE_DEFENSEFROMTERRAIN_ID,      COLUMN_DATA_PUE_DEFENSEFROMTERRAIN_MOD_ID,  COLUMN_DATA_PUE_DEFENSEFROMTERRAIN_EFFECT_NAME,
      COLUMN_DATA_PUE_DEFENSEFROMTERRAIN_TERRAIN, COLUMN_DATA_PUE_DEFENSEFROMTERRAIN_DEFENSE,
  };
  if (auto ec = createTable(session, pueDefenseTerrainColumns, DB_DATA, TABLE_DATA_PUE_DEFENSEFROMTERRAIN); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueVisionVariantColumns = {
      COLUMN_DATA_PUE_VISIONVARIANT_ID,      COLUMN_DATA_PUE_VISIONVARIANT_MOD_ID, COLUMN_DATA_PUE_VISIONVARIANT_EFFECT_NAME,
      COLUMN_DATA_PUE_VISIONVARIANT_VARIANT, COLUMN_DATA_PUE_VISIONVARIANT_VISION,
  };
  if (auto ec = createTable(session, pueVisionVariantColumns, DB_DATA, TABLE_DATA_PUE_VISIONVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueFirepowerVariantColumns = {
      COLUMN_DATA_PUE_FIREPOWERVARIANT_ID,      COLUMN_DATA_PUE_FIREPOWERVARIANT_MOD_ID,    COLUMN_DATA_PUE_FIREPOWERVARIANT_EFFECT_NAME,
      COLUMN_DATA_PUE_FIREPOWERVARIANT_VARIANT, COLUMN_DATA_PUE_FIREPOWERVARIANT_FIREPOWER,
  };
  if (auto ec = createTable(session, pueFirepowerVariantColumns, DB_DATA, TABLE_DATA_PUE_FIREPOWERVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueDefenseVariantColumns = {
      COLUMN_DATA_PUE_DEFENSEVARIANT_ID,      COLUMN_DATA_PUE_DEFENSEVARIANT_MOD_ID,  COLUMN_DATA_PUE_DEFENSEVARIANT_EFFECT_NAME,
      COLUMN_DATA_PUE_DEFENSEVARIANT_VARIANT, COLUMN_DATA_PUE_DEFENSEVARIANT_DEFENSE,
  };
  if (auto ec = createTable(session, pueDefenseVariantColumns, DB_DATA, TABLE_DATA_PUE_DEFENSEVARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pueIntelColumns = {
      COLUMN_DATA_PUE_INTEL_ID,   COLUMN_DATA_PUE_INTEL_MOD_ID, COLUMN_DATA_PUE_INTEL_EFFECT_NAME,
      COLUMN_DATA_PUE_INTEL_TYPE, COLUMN_DATA_PUE_INTEL_TARGET,
  };
  if (auto ec = createTable(session, pueIntelColumns, DB_DATA, TABLE_DATA_PUE_INTEL); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> aueColumns = {
      COLUMN_DATA_ACTIVE_UNIT_EFFECT_ID,           COLUMN_DATA_ACTIVE_UNIT_EFFECT_MOD_ID,         COLUMN_DATA_ACTIVE_UNIT_EFFECT_NAME,
      COLUMN_DATA_ACTIVE_UNIT_EFFECT_HITPOINT,     COLUMN_DATA_ACTIVE_UNIT_EFFECT_ROUNDHITPOINT,  COLUMN_DATA_ACTIVE_UNIT_EFFECT_SETFUEL,
      COLUMN_DATA_ACTIVE_UNIT_EFFECT_SETAMMO,      COLUMN_DATA_ACTIVE_UNIT_EFFECT_ADDFUEL,        COLUMN_DATA_ACTIVE_UNIT_EFFECT_ADDAMMO,
      COLUMN_DATA_ACTIVE_UNIT_EFFECT_MULTIPLYFUEL, COLUMN_DATA_ACTIVE_UNIT_EFFECT_MULTIPLYAMMO,   COLUMN_DATA_ACTIVE_UNIT_EFFECT_MAKEACTIVE,
      COLUMN_DATA_ACTIVE_UNIT_EFFECT_STUNDURATION, COLUMN_DATA_ACTIVE_UNIT_EFFECT_COCHARGEFACTOR,
  };
  if (auto ec = createTable(session, aueColumns, DB_DATA, TABLE_DATA_ACTIVE_UNIT_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pteColumns = {
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_ID,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_MOD_ID,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_NAME,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_INCOME,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_INCOMEFLAT,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_REPAIR,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_OCCLUDESVISION,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_VISION,
      COLUMN_DATA_PASSIVE_TERRAIN_EFFECT_BUILDCOST,
  };
  if (auto ec = createTable(session, pteColumns, DB_DATA, TABLE_DATA_PASSIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pteBuildListColumns = {
      COLUMN_DATA_PTE_BUILDLISTMOD_ID,
      COLUMN_DATA_PTE_BUILDLISTMOD_MOD_ID,
      COLUMN_DATA_PTE_BUILDLISTMOD_EFFECT_NAME,
      COLUMN_DATA_PTE_BUILDLISTMOD_UNIT,
  };
  if (auto ec = createTable(session, pteBuildListColumns, DB_DATA, TABLE_DATA_PTE_BUILDLISTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> ateColumns = {
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_ID,
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_MOD_ID,
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_NAME,
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_UNITSUMMONEDNAME,
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_UNITSUMMONEDDAMAGE,
      COLUMN_DATA_ACTIVE_TERRAIN_EFFECT_UNITSUMMONEDACTIVE,
  };
  if (auto ec = createTable(session, ateColumns, DB_DATA, TABLE_DATA_ACTIVE_TERRAIN_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> pgeColumns = {
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_ID,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_MOD_ID,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_NAME,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_VARIANT,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_MOVEMENTVARIANTREPLACE,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_MOVEMENTVARIANTOVERRIDE,
      COLUMN_DATA_PASSIVE_GLOBAL_EFFECT_MINIMUMVISIONMOD,
  };
  if (auto ec = createTable(session, pgeColumns, DB_DATA, TABLE_DATA_PASSIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> pgeVariantHintColumns = {
      COLUMN_DATA_PGE_VARIANTHINTMOD_ID,          COLUMN_DATA_PGE_VARIANTHINTMOD_MOD_ID,
      COLUMN_DATA_PGE_VARIANTHINTMOD_EFFECT_NAME, COLUMN_DATA_PGE_VARIANTHINTMOD_EFFECT_VARIANT,
      COLUMN_DATA_PGE_VARIANTHINTMOD_HINT,
  };
  if (auto ec = createTable(session, pgeVariantHintColumns, DB_DATA, TABLE_DATA_PGE_VARIANTHINTMOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> ageColumns = {
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_ID,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_MOD_ID,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_NAME,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_FUND,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_FUNDFLAT,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_POWERBAR,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_POWERBARPERFUNDS,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_MISSILECOUNT,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_MISSILEDAMAGE,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_MISSILEAREAOFEFFECT,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_MISSILESTUNDURATION,
      COLUMN_DATA_ACTIVE_GLOBAL_EFFECT_COCHARGEFACTOR,
  };
  if (auto ec = createTable(session, ageColumns, DB_DATA, TABLE_DATA_ACTIVE_GLOBAL_EFFECT); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> ageMissileTargetColumns = {
      COLUMN_DATA_AGE_MISSILETARGETMETHOD_ID,
      COLUMN_DATA_AGE_MISSILETARGETMETHOD_MOD_ID,
      COLUMN_DATA_AGE_MISSILETARGETMETHOD_EFFECT_NAME,
      COLUMN_DATA_AGE_MISSILETARGETMETHOD_EFFECT_TARGET_METHOD,
  };
  if (auto ec = createTable(session, ageMissileTargetColumns, DB_DATA, TABLE_DATA_AGE_MISSILETARGETMETHOD); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> settingsColumns = {
      COLUMN_DATA_SETTINGS_ID,
      COLUMN_DATA_SETTINGS_MOD_ID,
      COLUMN_DATA_SETTINGS_NAME,
      COLUMN_DATA_SETTINGS_FOG,
      COLUMN_DATA_SETTINGS_POWERS,
      COLUMN_DATA_SETTINGS_TEAMS,
      COLUMN_DATA_SETTINGS_STARTINGFUNDS,
      COLUMN_DATA_SETTINGS_INCOMEMULTIPLIER,
      COLUMN_DATA_SETTINGS_UNITLIMIT,
      COLUMN_DATA_SETTINGS_CAPTURELIMIT,
      COLUMN_DATA_SETTINGS_DAYLIMIT,
      COLUMN_DATA_SETTINGS_COMETERSIZE,
      COLUMN_DATA_SETTINGS_COMETERMULTIPLIER,
  };
  if (auto ec = createTable(session, settingsColumns, DB_DATA, TABLE_DATA_DEFAULT_GAME_SETTINGS); ec != DBErrorCode::NONE) {
    return ec;
  }
  std::vector<Column> settingsVariantColumns = {
      COLUMN_DATA_SETTINGS_VARIANT_ID,      COLUMN_DATA_SETTINGS_VARIANT_MOD_ID, COLUMN_DATA_SETTINGS_VARIANT_SETTING,
      COLUMN_DATA_SETTINGS_VARIANT_VARIANT, COLUMN_DATA_SETTINGS_VARIANT_VALUE,
  };
  if (auto ec = createTable(session, settingsVariantColumns, DB_DATA, TABLE_DATA_DEFAULTSETTINGS_VARIANT); ec != DBErrorCode::NONE) {
    return ec;
  }

  std::vector<Column> configColumns = {
      COLUMN_DATA_CONFIG_ID,
      COLUMN_DATA_CONFIG_MOD_ID,
      COLUMN_DATA_CONFIG_MINTERRAINSTARS,
      COLUMN_DATA_CONFIG_UNLIMITEDUNLOAD,
      COLUMN_DATA_CONFIG_TERRAINDEFENSESCALES,
      COLUMN_DATA_CONFIG_TERRAINFIREPOWERSCALES,
  };
  if (auto ec = createTable(session, configColumns, DB_DATA, TABLE_DATA_CONFIG); ec != DBErrorCode::NONE) {
    return ec;
  }
  return DBErrorCode::NONE;
}

}  // namespace

DBErrorCode buildDatabase() {
  sqlutil::Session session;
  sqlutil::Transaction transaction{session};
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