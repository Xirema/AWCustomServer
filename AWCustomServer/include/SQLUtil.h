#pragma once
#include<boost/mysql.hpp>

namespace sqlutil {
	struct Session {
		boost::asio::io_context context;
		boost::asio::ssl::context sslContext;
		boost::mysql::tcp_ssl_connection connection;

		Session();
		~Session();
		Session(Session const&) = delete;
	};
}