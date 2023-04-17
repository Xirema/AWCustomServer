#include<SQLUtil.h>
#include<Properties.h>

namespace sqlutil {
	Session::Session():
	sslContext(boost::asio::ssl::context::tls_client),
	connection(context.get_executor(), sslContext) 
	{
		auto const& props = properties::Properties::instance();
		boost::asio::ip::tcp::resolver resolver{ context.get_executor() };

		auto endpoints = resolver.resolve(props.getString("dbHostName"), props.getString("dbPort"));
		std::string username = props.getString("dbUser");
		std::string password = props.getString("dbPass");
		std::string database = props.getString("dbName");
		boost::mysql::handshake_params params{
			username,
			password,
			database
		};

		connection.connect(*endpoints.begin(), params);
		connection.set_meta_mode(boost::mysql::metadata_mode::full);
	}

	Session::~Session() {
		connection.close();
	}
}