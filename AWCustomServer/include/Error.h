#pragma once
#include<string>
#include<format>
#include<print>

#include<boost/describe.hpp>

namespace net {
  BOOST_DEFINE_ENUM_CLASS(RestErrorType, NONE, BAD_REQUEST, INTERNAL_ERROR, INVALID_DATA, NOT_FOUND)
	struct RestError {
		std::string message;
		RestErrorType type;
		RestError():RestError("", RestErrorType::NOT_FOUND) {}
		RestError(std::string message, RestErrorType type) : message(std::move(message)), type(type) {}
	};
}

namespace db {
  BOOST_DEFINE_ENUM_CLASS(DBErrorCode, NONE, TABLE_NOT_FOUND, RESULTS_EMPTY, UNKNOWN, DB_WRONG_VERSION)
}

template<>
struct std::formatter<net::RestErrorType> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		auto it = ctx.begin();
		while(it != ctx.end() && *it != '}') {
			++it;
		}
		return it;
	}

	template<class FmtContext>
	auto format(net::RestErrorType type, FmtContext & ctx) const {
		std::string s;
		switch(type) {
		case net::RestErrorType::BAD_REQUEST:
			s = "BAD_REQUEST";
			break;
		case net::RestErrorType::INTERNAL_ERROR:
			s = "INTERNAL_ERROR";
			break;
		case net::RestErrorType::INVALID_DATA:
			s = "INVALID_DATA";
			break;
		case net::RestErrorType::NOT_FOUND:
			s = "NOT_FOUND";
			break;
		default:
			s = "UNKNOWN";
		}
		return std::ranges::copy(std::move(s), ctx.out()).out;
	}
};