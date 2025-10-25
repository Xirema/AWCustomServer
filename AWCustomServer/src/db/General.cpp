#include<DBFunctions.h>
#include <SQLUtil.h>
#include<iostream>
#include<print>

#include<dbs/DBTablesV0.0.1.h>

namespace mysql = boost::mysql;
using ParameterPack = std::vector<mysql::field>;

namespace db {
  std::expected<std::string, DBErrorCode> check_db_version() {
    try {
      sqlutil::Session session;
      std::string checkVersionString = R"SQL(
        select 
          VERSION 
        from 
          GENERAL.META
      )SQL";
      auto statement = session.connection.prepare_statement(checkVersionString);
      mysql::results results;
      session.connection.execute(statement.bind(), results);
      if (results.size() != 1) {
        return std::unexpected(DBErrorCode::RESULTS_EMPTY);
      }
      return results.rows().at(0).at(0).as_string();
    } catch (mysql::error_with_diagnostics const& err) {
      std::println(std::cerr, "DB Error: {} - {}", err.code().value(), err.what());
      std::println(std::cerr, "Client Message: {}", err.get_diagnostics().client_message());
      std::println(std::cerr, "Server Message: {}", err.get_diagnostics().server_message());
      auto code = DBErrorCode::UNKNOWN;
      if (err.code() == mysql::common_server_errc::er_bad_db_error) { 
        code = DBErrorCode::TABLE_NOT_FOUND;
      }
      return std::unexpected(code);
    }
  }
}