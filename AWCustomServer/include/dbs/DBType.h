#pragma once
#include <Error.h>

#include <string>

namespace db {
struct Column {
  std::string_view columnName;
  std::string_view columnType;
  bool nullable;
  bool autoIncrement;

  constexpr Column() : Column("", "") {}
  constexpr Column(std::string_view columnName, std::string_view columnType, bool nullable = false, bool autoIncrement = false)
      : columnName(columnName), columnType(columnType), nullable(nullable), autoIncrement(autoIncrement) {}

  constexpr Column& setColumnName(std::string_view name) {
    this->columnName = name;
    return *this;
  }
  constexpr Column& setColumnType(std::string_view type) {
    this->columnType = type;
    return *this;
  }
  constexpr Column& setNullable(bool nullable) {
    this->nullable = nullable;
    return *this;
  }
  constexpr Column& setAutoIncrement(bool autoIncrement) {
    this->autoIncrement = autoIncrement;
    return *this;
  }

  constexpr auto operator<=>(Column const& o) const = default;
};

constexpr std::string_view SQL_DROP_SCHEMA = "drop database if exists `{}`";
constexpr std::string_view SQL_CREATE_SCHEMA = "create schema `{}`";
constexpr std::string_view SQL_CREATE_TABLE = "create table `{}`.`{}` ({})";
constexpr std::string_view SQL_CREATE_COLUMN = "`{}` {} {} null {}";
constexpr std::string_view SQL_DEFINE_PRIMARY_KEY = "primary key (`{}`)";
constexpr std::string_view SQL_INSERT = "insert into `{}`.`{}` ({}) values {}";
constexpr std::string_view SQL_SELECT = "select {} from `{}`.`{}` {} {}";

constexpr std::string_view SQL_INT = "int";
constexpr std::string_view SQL_SMALLSTRING = "varchar(45)";
constexpr std::string_view SQL_LARGESTRING = "varchar(512)";
constexpr std::string_view SQL_TEXT = "text";
constexpr std::string_view SQL_BOOL = "tinyint";
constexpr std::string_view SQL_DATETIME = "datetime";
constexpr std::string_view SQL_BLOB = "blob";
constexpr std::string_view SQL_DOUBLE = "double";
}  // namespace db