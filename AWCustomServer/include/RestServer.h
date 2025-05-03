#pragma once
#include<memory>
#include<thread>
#include<functional>
#include<string>
#include<vector>
#include<unordered_map>
#include<string_view>
#include<optional>
#include<Error.h>

#include<boost/describe.hpp>
#include<boost/container_hash/hash.hpp>

namespace net {
  BOOST_DEFINE_FIXED_ENUM_CLASS(HTTPVerb, uint8_t, GET, PUT, POST, DELETE, OPTIONS, HEAD, CONNECT, TRACE, PATCH)
  struct HTTPFunctionDefinition {
    HTTPVerb verb{HTTPVerb::HEAD};
    std::string name;
  };
  BOOST_DESCRIBE_STRUCT(HTTPFunctionDefinition, (), (verb, name))
  using boost::describe::operators::operator==;
	struct HTTPHeaders {
		std::string target;
		std::unordered_map<std::string, std::string> httpHeaders;
		std::unordered_map<std::string, std::string> httpParameters;

		std::optional<std::string_view> getHeader(std::string const& header) const;
		std::optional<std::string_view> getParameter(std::string const& parameter) const;
	};
	using HTTPFunc = std::function<std::string(HTTPHeaders const&, std::string)>;
  using GETFunc = std::function<std::string(HTTPHeaders const&)>;
	struct SSLCert {
		std::string key;
		std::string cert;
	};

  using HTTPFunctionMap = std::unordered_map<HTTPFunctionDefinition, HTTPFunc, boost::hash<HTTPFunctionDefinition>>;

	class RestServerImpl;
	class RestServer {
		std::unique_ptr<RestServerImpl> impl;
	public:
		RestServer(std::string_view name, uint16_t port, std::optional<SSLCert> sslCert = {}, uint32_t maxThreadCount = std::thread::hardware_concurrency());
		void start(HTTPFunctionMap functionMap);
		void printRequests(bool value);
		RestServer(RestServer const&) = delete;
		~RestServer();
	};
}
