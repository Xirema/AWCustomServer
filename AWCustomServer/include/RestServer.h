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