//#include<mysqlx/xdevapi.h>
#include<boost/mysql.hpp>
#include<boost/asio.hpp>
#include<boost/json.hpp>
#include<string>
#include<iostream>
#include<memory>
#include<boost/algorithm/string.hpp>
#include<optional>
#include<iomanip>

std::optional<int64_t> getOptionalInt64(boost::json::value const& v, std::string_view name) {
	auto const& obj = v.as_object();
	if (auto value_ptr = obj.if_contains(name)) {
		return value_ptr->as_int64();
	}
	return {};
}

int main(int argc, char** argv) {
	namespace asio = boost::asio;
	namespace mysql = boost::mysql;

	try {

		asio::io_context ctx;
		asio::ssl::context ssl_ctx{ asio::ssl::context::tls_client };
		mysql::tcp_ssl_connection conn{ ctx.get_executor(), ssl_ctx };

		asio::ip::tcp::resolver resolver{ ctx.get_executor() };

		auto endpoints = resolver.resolve("localhost", "3306");

		mysql::handshake_params params{
			"dev",
			"dev",
			""
		};

		std::cout << "Now Connecting to server." << std::endl;
		conn.connect(*endpoints.begin(), params);
		conn.set_meta_mode(mysql::metadata_mode::full);
		std::cout << "Connection successful." << std::endl;

		std::string sql = "select * from RESOURCE.PACK where NAME = ? and EXPIRED is null";
		std::vector<mysql::field> parameters;
		std::string name = "default";
		parameters.emplace_back(name);

		mysql::results results;
		auto statement = conn.prepare_statement(sql);
		conn.execute(
			statement.bind(parameters.begin(), parameters.end()),
			results
		);
		
		std::cout << "SQL Execution concluded." << std::endl;

		size_t columns = results.meta().size();
		size_t rows = results.size();
		std::vector<std::vector<std::string>> resultsMatrix;
		auto & headersRow = resultsMatrix.emplace_back();
		for (auto const& column : results.meta()) {
			headersRow.emplace_back(column.column_name());
		}
		for (auto const& row : results.rows()) {
			auto& matrixRow = resultsMatrix.emplace_back();
			for (auto const& object : row) {
				std::stringstream ss;
				ss << object;
				matrixRow.emplace_back(ss.str());
			}
		}

		std::vector<size_t> columnMaxWidths;
		for (size_t i = 0; i < resultsMatrix.at(0).size(); i++) {
			auto& columnMaxWidth = columnMaxWidths.emplace_back(0);
			for (size_t j = 0; j < resultsMatrix.size(); j++) {
				columnMaxWidth = std::max(columnMaxWidth, resultsMatrix.at(j).at(i).size());
			}
		}

		bool printHeader = true;
		for (auto const& row : resultsMatrix) {
			std::stringstream ss;
			for (size_t i = 0; i < columnMaxWidths.size(); i++) {
				std::cout << ' ' << std::setw(columnMaxWidths.at(i)) << row.at(i) << ' ' << '|';
				ss << '-' << std::setfill('-') << std::setw(columnMaxWidths.at(i)) << "" << '-' << '|';
			}
			std::cout << "\n";
			if (printHeader) {
				std::cout << ss.str() << "\n";
				printHeader = false;
			} 
		}

		conn.close();
	} 
	catch (mysql::error_with_diagnostics const& e) {
		std::cerr << "MYSQL ERROR: " << e.code() << std::endl;
		std::cerr << e.get_diagnostics().client_message() << std::endl;
		std::cerr << e.get_diagnostics().server_message() << std::endl;
	}
	catch (...) {
		try {
			std::rethrow_exception(std::current_exception());
		}
		catch (std::exception const& e) {
			std::cerr << "FATAL ERROR: " << e.what() << std::endl;
		}
	}
}

//int main(int argc, char** argv) {
//	std::unique_ptr<mysqlx::Session> session_ptr;
//	try {
//		session_ptr = std::make_unique<mysqlx::Session>(
//			mysqlx::SessionOption::HOST, "localhost",
//			mysqlx::SessionOption::PORT, 33060,
//			mysqlx::SessionOption::USER, "dev",
//			mysqlx::SessionOption::PWD, "dev"
//		);
//
//		session_ptr->startTransaction();
//		
//
//
//		std::string sql = R"SQL(
//		insert into
//			DATA.IMAGE_RESOURCE
//			(
//				MOD_ID, `KEY`, `TYPE`, SMALLIMAGE, LARGEIMAGE, ARMYCOLOR, `ORDER`, ORIENTATION, VARIANT
//			)
//		values
//		(
//			?, ?, ?, ?, ?, ?, ?, ?, ?
//		)
//		)SQL";
//
//		auto statement = session_ptr->sql(sql);
//		statement.bind(-1, "infantry", "unit", "iVBORw0KGgoAAAANSUhEUgAAABAAAAAQCAIAAACQkWg2AAABhGlDQ1BJQ0MgcHJvZmlsZQAAKJF9kT1Iw0AcxV9TpSoVBzuIiGSoThZERRxLFYtgobQVWnUwufQLmjQkKS6OgmvBwY/FqoOLs64OroIg+AHi6OSk6CIl/i8ptIjx4Lgf7+497t4BQqPCVLNrElA1y0jFY2I2tyoGXhHAKPwQ0CsxU0+kFzPwHF/38PH1LsKzvM/9OfqVvMkAn0gcZbphEW8Qz25aOud94hArSQrxOfGEQRckfuS67PIb56LDAs8MGZnUPHGIWCx2sNzBrGSoxDPEYUXVKF/Iuqxw3uKsVmqsdU/+wmBeW0lzneYI4lhCAkmIkFFDGRVYiNCqkWIiRfsxD/+w40+SSyZXGYwcC6hCheT4wf/gd7dmYXrKTQrGgO4X2/4YAwK7QLNu29/Htt08AfzPwJXW9lcbwNwn6fW2Fj4CBraBi+u2Ju8BlzvA0JMuGZIj+WkKhQLw", "", "os", 0, nullptr, nullptr);
//		auto results = statement.execute();
//
//		session_ptr->commit();
//	}
//	catch (mysqlx::Error const& e) {
//		std::cerr << "MySQL Error: " << e.what() << std::endl;
//	}
//	catch (...) {
//		if(session_ptr)
//			session_ptr->rollback();
//		try {
//			std::rethrow_exception(std::current_exception());
//		}
//		catch (std::exception const& e) {
//			std::cerr << "Problem executing SQL: " << e.what() << std::endl;
//		}
//	}
//}
