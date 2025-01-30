#pragma once
#include<memory>
#include<thread>
#include<functional>
#include<string>
#include<vector>
#include<unordered_map>
#include<string_view>
#include<optional>
namespace net {
	struct HTTPHeaders {
		std::string target;
		std::unordered_map<std::string, std::string> httpHeaders;
		std::unordered_map<std::string, std::string> httpParameters;

		std::optional<std::string_view> getHeader(std::string const& header) const;
		std::optional<std::string_view> getParameter(std::string const& parameter) const;
	};
	using GETFunc = std::function<std::string(HTTPHeaders const&)>;
	using POSTFunc = std::function<std::string(HTTPHeaders const&, std::string)>;
	using PUTFunc = std::function<std::string(HTTPHeaders const&, std::string)>;
	//using DELETEFunc = std::function<std::string(HTTPHeaders const&, std::string)>;
	struct RestError {
		std::string message;
		enum class Type {
			BAD_REQUEST, INTERNAL_ERROR, INVALID_DATA, NOT_FOUND
		};
		Type type;
		RestError():RestError("", Type::NOT_FOUND) {}
		RestError(std::string message, Type type) : message(std::move(message)), type(type) {}
	};
	struct SSLCert {
		std::string key;
		std::string cert;
	};
	class RestServerImpl;
	class RestServer {
		std::unique_ptr<RestServerImpl> impl;
	public:
		RestServer(std::string_view name, uint16_t port, std::optional<SSLCert> sslCert = {}, uint32_t maxThreadCount = std::thread::hardware_concurrency());
		void start(
			std::unordered_map<std::string, GETFunc> getFunctions, 
			std::unordered_map<std::string, POSTFunc> postFunctions,
			std::unordered_map<std::string, PUTFunc> putFunctions
		);
		void printRequests(bool value);
		RestServer(RestServer const&) = delete;
		~RestServer();
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