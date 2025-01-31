#pragma once
#include<string>
#include<format>
#include<print>

namespace net {
	struct RestError {
		std::string message;
		enum class Type {
			BAD_REQUEST, INTERNAL_ERROR, INVALID_DATA, NOT_FOUND
		};
		Type type;
		RestError():RestError("", Type::NOT_FOUND) {}
		RestError(std::string message, Type type) : message(std::move(message)), type(type) {}
	};
}

template<>
struct std::formatter<net::RestError::Type> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		auto it = ctx.begin();
		while(it != ctx.end() && *it != '}') {
			++it;
		}
		return it;
	}

	template<class FmtContext>
	auto format(net::RestError::Type type, FmtContext & ctx) const {
		std::string s;
		switch(type) {
		case net::RestError::Type::BAD_REQUEST:
			s = "BAD_REQUEST";
			break;
		case net::RestError::Type::INTERNAL_ERROR:
			s = "INTERNAL_ERROR";
			break;
		case net::RestError::Type::INVALID_DATA:
			s = "INVALID_DATA";
			break;
		case net::RestError::Type::NOT_FOUND:
			s = "NOT_FOUND";
			break;
		default:
			s = "UNKNOWN";
		}
		return std::ranges::copy(std::move(s), ctx.out()).out;
	}
};