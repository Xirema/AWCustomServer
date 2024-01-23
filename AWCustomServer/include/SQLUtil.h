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

	class Transaction {
		Session& session;
		bool committed = false;
	public:
		Transaction(Session& session);
		~Transaction();
		Transaction(Transaction const&) = delete;
		void commit();
	};
}