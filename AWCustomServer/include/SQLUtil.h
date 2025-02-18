#pragma once
#include <boost/mysql.hpp>
#include<optional>

namespace sqlutil
{
	namespace mysql = boost::mysql;
	struct Session
	{
		boost::asio::io_context context;
		boost::asio::ssl::context sslContext;
		boost::mysql::tcp_ssl_connection connection;

		Session();
		~Session();
		Session(Session const &) = delete;
	};

	class Transaction
	{
		Session &session;
		bool committed = false;

	public:
		Transaction(Session &session);
		~Transaction();
		Transaction(Transaction const &) = delete;
		void commit();
	};

	template <typename T>
	std::optional<T> get(mysql::field_view const &value)
	{
		if (value.is_null())
		{
			return {};
		}
		if constexpr (std::is_same_v<int64_t, T>)
		{
			return value.as_int64();
		}
		else if constexpr (std::is_same_v<uint64_t, T>)
		{
			return value.as_uint64();
		}
		else if constexpr (std::is_same_v<std::string, T>)
		{
			return std::string{value.as_string()};
		}
		else if constexpr (std::is_same_v<int32_t, T>)
		{
			return static_cast<int32_t>(value.as_int64());
		}
		else if constexpr (std::is_same_v<uint32_t, T>)
		{
			return static_cast<uint32_t>(value.as_uint64());
		}
		else if constexpr (std::is_same_v<bool, T>)
		{
			return static_cast<bool>(value.as_int64());
		}
		throw std::runtime_error("Unsupported type in get<T>");
	}

	template <typename T>
	void set(T &var, mysql::field_view const &value)
	{
		auto opt = get<T>(value);
		if (!opt)
		{
			throw std::runtime_error("Null value for non-nullable field");
		}
		var = *opt;
	}

	template <typename T>
	void set(std::optional<T> &var, mysql::field_view const &value)
	{
		var = get<T>(value);
	}

	template <>
	inline std::optional<mysql::datetime> get<mysql::datetime>(mysql::field_view const &value)
	{
		if (value.is_null())
		{
			return {};
		}
		return value.as_datetime();
	}

	std::string inline dateToString(mysql::datetime const &time)
	{
		return std::format("{:%F}", time.as_time_point());
	}

	std::string inline dateToString(mysql::date const &time)
	{
		return std::format("{:%F}", time.as_time_point());
	}

	mysql::datetime inline stringToDate(std::string s)
	{
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
}