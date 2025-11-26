#pragma once
#include <dbs/DBType.h>

#include <boost/mysql.hpp>
#include <iostream>
#include <optional>
#include <ranges>

namespace sqlutil {
namespace mysql = boost::mysql;
struct Session {
  boost::asio::io_context context;
  boost::asio::ssl::context sslContext;
  boost::mysql::tcp_ssl_connection connection;

  Session();
  ~Session();
  Session(Session const&) = delete;
};

class Transaction {
  Session& session;
  bool committed = false;

 public:
  Transaction(Session& session);
  ~Transaction();
  Transaction(Transaction const&) = delete;
  void commit();
};

template <typename T>
std::optional<T> get(mysql::field_view const& value) {
  if (value.is_null()) {
    return {};
  }
  if constexpr (std::is_same_v<int64_t, T>) {
    return value.as_int64();
  } else if constexpr (std::is_same_v<uint64_t, T>) {
    return value.as_uint64();
  } else if constexpr (std::is_same_v<std::string, T>) {
    return std::string{value.as_string()};
  } else if constexpr (std::is_same_v<int32_t, T>) {
    return static_cast<int32_t>(value.as_int64());
  } else if constexpr (std::is_same_v<uint32_t, T>) {
    return static_cast<uint32_t>(value.as_uint64());
  } else if constexpr (std::is_same_v<bool, T>) {
    return static_cast<bool>(value.as_int64());
  } else if constexpr (std::is_same_v<double, T>) {
    return value.as_double();
  }
  throw std::runtime_error("Unsupported type in get<T>");
}

using ParameterPack = std::vector<mysql::field>;

template <typename T>
void set(T& var, mysql::field_view const& value) {
  auto opt = get<T>(value);
  if (!opt) {
    throw std::runtime_error("Null value for non-nullable field");
  }
  var = *opt;
}

template <typename T>
void set(std::optional<T>& var, mysql::field_view const& value) {
  var = get<T>(value);
}

template <>
inline std::optional<mysql::datetime> get<mysql::datetime>(mysql::field_view const& value) {
  if (value.is_null()) {
    return {};
  }
  return value.as_datetime();
}

std::string inline dateToString(mysql::datetime const& time) { return std::format("{:%F}", time.as_time_point()); }

std::string inline dateToString(mysql::date const& time) { return std::format("{:%F}", time.as_time_point()); }

mysql::datetime inline stringToDate(std::string s) {
  std::stringstream ss{s};
  mysql::datetime::time_point tp;
  ss >> std::chrono::parse("%F", tp);
  return mysql::datetime{tp};
}

std::string inline to_string(mysql::field_kind kind) {
  std::stringstream ss;
  ss << kind;
  return ss.str();
}

template <typename T>
void emplaceField(ParameterPack& pack, T&& t) {
  pack.emplace_back(t);
}

template <typename T>
void emplaceField(ParameterPack& pack, std::optional<T> const& t) {
  if (!t) {
    pack.emplace_back(nullptr);
  } else {
    pack.emplace_back(*t);
  }
}

template <typename T>
void emplaceField(ParameterPack& pack, std::optional<T>& t) {
  if (!t) {
    pack.emplace_back(nullptr);
  } else {
    pack.emplace_back(*t);
  }
}

template <typename... Types>
ParameterPack toParameterPack(Types&&... values) {
  ParameterPack pack;
  (emplaceField(pack, std::forward<Types>(values)), ...);
  return pack;
}

template <typename Range>
concept ColumnRange = std::is_same_v<db::Column, std::ranges::range_value_t<Range>>;

template <typename Range>
  requires std::is_same_v<std::string_view, std::ranges::range_value_t<Range>>
std::string createInsertColumns(Range&& columns) {
  using namespace std::string_literals;
  return columns | std::views::transform([](std::string_view const& column) { return std::format("`{}`", column); }) | std::views::join_with(","s) |
         std::ranges::to<std::string>();
}
std::string createInsertColumns(ColumnRange auto&& columns) {
  using namespace std::string_literals;
  return createInsertColumns(columns | std::views::transform([](db::Column const& column) { return column.columnName; }));
}
std::string parameterizeField(mysql::field const& field);
std::string parameterizePack(ParameterPack const& pack);
std::pair<std::string, ParameterPack> createInsertValues(std::vector<ParameterPack> const& values);
std::string columnCreateString(db::Column const& column);

std::string createColumns(ColumnRange auto&& columns) {
  using namespace std::string_literals;
  db::Column const* primaryKey = nullptr;
  for (auto& column : columns) {
    if (primaryKey && column.autoIncrement) {
      throw std::runtime_error("Cannot define more than one column to be autoIncremented!");
    }
    if (column.autoIncrement) {
      primaryKey = &column;
    }
  }
  std::string retString = columns | std::views::transform(columnCreateString) | std::views::join_with(", "s) | std::ranges::to<std::string>();
  if (primaryKey) {
    retString += ", "s + std::format(db::SQL_DEFINE_PRIMARY_KEY, primaryKey->columnName);
  }
  return retString;
}

db::DBErrorCode createTable(sqlutil::Session& session, ColumnRange auto&& columns, std::string_view schema, std::string_view table) {
  mysql::results results;
  std::string columnDefines = createColumns(columns);
  session.connection.execute(std::format(db::SQL_CREATE_TABLE, schema, table, columnDefines), results);
  return db::DBErrorCode::NONE;
}

template <typename TypeMapper, typename TypeRange, typename Type = std::ranges::range_value_t<TypeRange>>
  requires requires(TypeMapper tm, Type t) {
    { tm(t) } -> std::convertible_to<ParameterPack>;
  }
std::pair<mysql::statement, ParameterPack> createInsertStatement(sqlutil::Session& session, TypeRange&& values, TypeMapper&& tm,
                                                                 ColumnRange auto&& columns, std::string_view databaseName,
                                                                 std::string_view tableName) {
  auto insertColumns = createInsertColumns(columns);
  auto parameters = values | std::views::transform(tm);
  auto [valuesString, parametersFlattened] = createInsertValues(parameters | std::ranges::to<std::vector>());
  auto statement = session.connection.prepare_statement(std::format(db::SQL_INSERT, databaseName, tableName, insertColumns, valuesString));
  return std::make_pair(statement, parametersFlattened);
}

template <typename TypeMapper, typename TypeRange, typename Type = std::ranges::range_value_t<TypeRange>>
  requires requires(TypeMapper tm, Type t) {
    { tm(t) } -> std::convertible_to<ParameterPack>;
  }
mysql::results doInsert(sqlutil::Session& session, TypeRange&& values, TypeMapper&& tm, ColumnRange auto&& columns, std::string_view databaseName,
                        std::string_view tableName) {
  auto [statement, parameters] = createInsertStatement(session, std::forward<TypeRange>(values), std::forward<TypeMapper>(tm),
                                                       std::forward<decltype(columns)>(columns), databaseName, tableName);
  mysql::results results;
  session.connection.execute(statement.bind(parameters.begin(), parameters.end()), results);
  return results;
}

using ColumnFilter = std::pair<db::Column, mysql::field_view>;
using ColumnOrder = std::pair<db::Column, bool>;

template <typename RowMapper, typename Type = std::invoke_result_t<RowMapper, mysql::row_view>>
  requires requires(RowMapper rm, mysql::row_view row) {
    { rm(row) } -> std::convertible_to<Type>;
  }
std::vector<Type> doSelect(sqlutil::Session& session, RowMapper&& rowMapper, ColumnRange auto&& columns, std::string_view databaseName,
                           std::string_view tableName, std::vector<ColumnFilter> filter = {}, std::optional<ColumnOrder> order = {}) {
  using namespace std::string_literals;
  auto columnNameMapper = [](db::Column const& column) { return std::format("`{}`", column.columnName); };
  auto columnsString = columns | std::views::transform(columnNameMapper) | std::views::join_with(", "s) | std::ranges::to<std::string>();
  std::string whereClause = "";
  ParameterPack values;
  if (filter.size() > 0) {
    constexpr std::string_view WHERE_CLAUSE = "where {}";
    auto filterMapper = [](ColumnFilter const& filter) { return std::format("`{}` = ?", filter.first.columnName); };
    auto conditionsString = filter | std::views::transform(filterMapper) | std::views::join_with(" and "s) | std::ranges::to<std::string>();
    values = filter | std::views::values | std::ranges::to<ParameterPack>();
    whereClause = std::format(WHERE_CLAUSE, conditionsString);
  }

  constexpr std::string_view ORDER_CLAUSE = "order by `{}` {}";
  std::string orderClause = "";
  if (order) {
    auto&& [column, ascending] = *order;
    orderClause = std::format(ORDER_CLAUSE, column.columnName, ascending ? "ASC" : "DESC");
  }

  auto selectStatement =
      session.connection.prepare_statement(std::format(db::SQL_SELECT, columnsString, databaseName, tableName, whereClause, orderClause));
  mysql::results results;
  session.connection.execute(selectStatement.bind(values.begin(), values.end()), results);
  std::vector<Type> ret;
  for (auto const& row : results.rows()) {
    ret.emplace_back(rowMapper(row));
  }
  return ret;
};

void printError(mysql::error_with_diagnostics const& err, std::ostream& out = std::cerr);
}  // namespace sqlutil