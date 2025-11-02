#include <Properties.h>
#include <SQLUtil.h>

#include <ranges>

namespace sqlutil {
Session::Session() : sslContext(boost::asio::ssl::context::tls_client), connection(context.get_executor(), sslContext) {
  auto const& props = properties::Properties::instance();
  boost::asio::ip::tcp::resolver resolver{context.get_executor()};

  auto endpoints = resolver.resolve(props.getString("dbHostName"), props.getString("dbPort"));
  std::string username = props.getString("dbUser");
  std::string password = props.getString("dbPass");
  std::string database = props.getString("dbName");
  boost::mysql::handshake_params params{username, password, database};

  connection.connect(*endpoints.begin(), params);
  connection.set_meta_mode(boost::mysql::metadata_mode::full);
}

Session::~Session() { connection.close(); }

Transaction::Transaction(Session& session) : session(session) {
  boost::mysql::results results;
  this->session.connection.execute("START TRANSACTION", results);
}

Transaction::~Transaction() {
  try {
    if (!committed) {
      boost::mysql::results results;
      session.connection.execute("ROLLBACK", results);
    }
  } catch (...) {
    // TODO: Proper Logging
    std::cerr << "Unable to Rollback Transaction!" << std::endl;
  }
}
void Transaction::commit() {
  boost::mysql::results results;
  session.connection.execute("COMMIT", results);
  committed = true;
}

using namespace std::string_literals;
std::string createInsertColumns(std::vector<std::string_view> const& columns) {
  return columns | std::views::transform([](std::string_view const& column) { return std::format("`{}`", column); }) | std::views::join_with(","s) |
         std::ranges::to<std::string>();
}

std::string createInsertColumns(std::vector<db::Column> const& columns) {
  return createInsertColumns(columns | std::views::transform([](db::Column const& column) { return column.columnName; }) |
                             std::ranges::to<std::vector<std::string_view>>());
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

std::string columnCreateString(db::Column const& column) {
  return std::format(db::SQL_CREATE_COLUMN, column.columnName, column.columnType, column.nullable ? "" : "not",
                     column.autoIncrement ? "auto_increment" : "");
}

std::string createColumns(std::vector<db::Column>& columns) {
  db::Column* primaryKey = nullptr;
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
    retString += ", "s + std::format(db::SQL_DEFINE_PRIMARY_KEY, primaryKey->columnName);
  }
  return retString;
}

db::DBErrorCode createTable(sqlutil::Session& session, std::vector<db::Column>& columns, std::string_view schema, std::string_view table) {
  mysql::results results;
  std::string columnDefines = createColumns(columns);
  session.connection.execute(std::format(db::SQL_CREATE_TABLE, schema, table, columnDefines), results);
  return db::DBErrorCode::NONE;
}


void printError(mysql::error_with_diagnostics const& err, std::ostream& out) {
  std::println(out, "MySQL Error: {} - {}", err.code().value(), err.what());
  std::println(out, "Client Message: {}", err.get_diagnostics().client_message());
  std::println(out, "Server Message: {}", err.get_diagnostics().server_message());
}
}  // namespace sqlutil