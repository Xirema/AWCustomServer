#include<RestServer.h>
#include<atomic>
#include<boost/beast.hpp>
#include<boost/beast/ssl.hpp>
#include<iostream>
#include<boost/beast/version.hpp>
#include<boost/asio/dispatch.hpp>
#include<boost/asio/strand.hpp>
#include<boost/asio/ssl.hpp>
#include<mutex>
#include<variant>

namespace net {
	namespace networking = boost::asio;
	namespace beast = boost::beast;
	namespace http = beast::http;
	using tcp = networking::ip::tcp;
	namespace ssl = boost::asio::ssl;

	void loadServerCertificate(ssl::context& ctx, SSLCert const& sslCert) {
		ctx.set_options(
			ssl::context::default_workarounds |
			ssl::context::no_sslv2 |
			ssl::context::single_dh_use
		);
		ctx.use_certificate_chain(
			boost::asio::buffer(sslCert.cert.data(), sslCert.cert.size())
		);
		ctx.use_private_key(
			boost::asio::buffer(sslCert.key.data(), sslCert.key.size()),
			ssl::context::file_format::pem
		);
	}

	namespace {
		void workFunc(std::atomic_bool& shouldStop, networking::io_context& ioContext) {
			while (!shouldStop) {
				try {
					ioContext.run();
				}
				catch (...) {
					try {
						std::rethrow_exception(std::current_exception());
					}
					catch (std::exception const& e) {
						std::cerr << "EXCEPTION THROWN: " << e.what() << std::endl;
					}
				}
			}
		}
	}
	class RestServerImpl {
		friend class RestServer;
		friend class RestServerSession;
		struct CaseInsensitiveStringComparator {
			bool operator()(std::string const& a, std::string const& b) const {
				return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {return std::toupper(a) == std::toupper(b); });
			}
			bool operator()(std::string_view a, std::string const& b) const {
				return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {return std::toupper(a) == std::toupper(b); });
			}
			bool operator()(std::string const& a, std::string_view b) const {
				return std::equal(a.begin(), a.end(), b.begin(), b.end(), [](char a, char b) {return std::toupper(a) == std::toupper(b); });
			}
			size_t operator()(std::string a) const {
				for (char& c : a)
					c = std::toupper(c);
				return std::hash<std::string>{}(a);
			}
			size_t operator()(std::string_view str) const {
				std::string a{ str };
				for (char& c : a)
					c = std::toupper(c);
				return std::hash<std::string>{}(a);
			}
		};
		std::vector<std::thread> threads;
		std::unordered_map<std::string, GETFunc, CaseInsensitiveStringComparator, CaseInsensitiveStringComparator> getFunctions;
		std::unordered_map<std::string, POSTFunc, CaseInsensitiveStringComparator, CaseInsensitiveStringComparator> postFunctions;
		std::atomic_bool shouldStop;
		networking::io_context ioContext;
		tcp::acceptor acceptor;
		std::mutex funcMutex;
		std::optional<SSLCert> sslCert;
		ssl::context sslContext;
		bool doPrintRequests = false;

		class RestServerSession : public std::enable_shared_from_this<RestServerSession> {
		protected:
			friend RestServerImpl;
			RestServerImpl* parent;
			//beast::tcp_stream stream;
			std::variant<beast::tcp_stream, beast::ssl_stream<beast::tcp_stream>> stream_v;
			beast::flat_buffer buffer;
			using string_request = http::request<http::string_body>;
			using string_response = http::response<http::string_body>;
			bool printRequests = false;

			void run() {
				networking::dispatch(
					networking::make_strand(parent->ioContext),
					[ptr = shared_from_this()] {
						ptr->do_handshake();
					}
				);
			}

			void do_handshake() {
				struct Visitor {
					RestServerSession& p;
					void operator()(beast::tcp_stream& stream) const {
						stream.expires_after(std::chrono::seconds(30));
						networking::dispatch(
							stream.get_executor(),
							[ptr = p.shared_from_this()] {
								ptr->handle_handshake({});
							}
						);
					}
					void operator()(beast::ssl_stream<beast::tcp_stream>& stream) const {
						beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
						stream.async_handshake(
							ssl::stream_base::server,
							[ptr = p.shared_from_this()](beast::error_code ec) {
								ptr->handle_handshake(ec);
							}
						);
					}

					Visitor(RestServerSession& p) :
						p(p) {}
				} v{ *this };
				std::visit(v, stream_v);
			}

			void handle_handshake(beast::error_code ec) {
				if (ec) {
					std::cerr << "Problem performing handshake: " << ec.what() << std::endl;
				}

				do_read();
			}

			void do_read() {
				std::shared_ptr<string_request> request_ptr = std::make_shared<string_request>();
				std::shared_ptr<http::request_parser<http::string_body>> parser_ptr = std::make_shared<http::request_parser<http::string_body>>();
				parser_ptr->body_limit(std::numeric_limits<uint64_t>::max());
				if (auto ptr = std::get_if<beast::tcp_stream>(&stream_v)) {
					http::async_read(
						*ptr,
						buffer,
						*parser_ptr,
						[ptr = shared_from_this(), parser = parser_ptr](beast::error_code ec, size_t bytes_transferred) {
							ptr->handle_read(*parser, ec, bytes_transferred);
						}
					);
				}
				else if (auto ptr = std::get_if<beast::ssl_stream<beast::tcp_stream>>(&stream_v)) {
					http::async_read(
						*ptr,
						buffer,
						*parser_ptr,
						[ptr = shared_from_this(), parser = parser_ptr](beast::error_code ec, size_t bytes_transferred) {
							ptr->handle_read(*parser, ec, bytes_transferred);
						}
					);
				}
			}

			void do_close() {
				if (auto ptr = std::get_if<beast::tcp_stream>(&stream_v)) {
					ptr->socket().shutdown(tcp::socket::shutdown_send);
				}
				else if (auto ptr = std::get_if<beast::ssl_stream<beast::tcp_stream>>(&stream_v)) {
					beast::get_lowest_layer(*ptr).socket().shutdown(tcp::socket::shutdown_send);
				}
			}

			void handle_read(http::request_parser<http::string_body>& request_parser, beast::error_code ec, size_t bytes_transferred) {
				if (ec == http::error::end_of_stream) {
					do_close();
					return;
				}
				if (ec == beast::error::timeout) {
					//Timeouts are not serious errors, and can be ignored.
					return;
				}
				if (ec) {
					std::cerr << "Problem reading from Socket: " << ec.what() << std::endl;
					return;
				}
				handle_request(request_parser.get());
			}

			void handle_request(string_request request) {
				if (printRequests) {
					std::cout << request << std::endl;
				}
				// Returns a bad request response
				auto const bad_request =
					[&request](RestError const& e)
				{
					http::status status = [type = e.type] {
						if (type == RestError::Type::BAD_REQUEST)
							return http::status::bad_request;
						if (type == RestError::Type::INTERNAL_ERROR)
							return http::status::internal_server_error;
						if (type == RestError::Type::INVALID_DATA)
							return http::status::bad_request;
						return http::status::unknown;
					}();
					string_response response{ status, request.version() };
					response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					response.set(http::field::content_type, "text/html");
					response.set(http::field::access_control_allow_origin, "*");
					response.keep_alive(request.keep_alive());
					response.body() = e.message;
					response.content_length(response.body().size());
					response.prepare_payload();
					return response;
				};

				// Returns a not found response
				auto const not_found =
					[&request](beast::string_view target)
				{
					string_response response{ http::status::not_found, request.version() };
					response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					response.set(http::field::content_type, "text/html");
					response.set(http::field::access_control_allow_origin, "*");
					response.keep_alive(request.keep_alive());
					response.body() = "The resource '" + std::string(target) + "' was not found.";
					response.content_length(response.body().size());
					response.prepare_payload();
					return response;
				};

				string_response response{ http::status::ok, request.version() };
				response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
				response.set(http::field::content_type, "application/json");
				response.set(http::field::access_control_allow_origin, "*");
				response.keep_alive(request.keep_alive());
				HTTPHeaders headers;
				headers.target = std::string{ request.target() };
				for (auto const& val : request.base()) {
					headers.httpHeaders[std::string{ val.name_string() }] = std::string{ val.value() };
				}

				if (request.method() == http::verb::get) {
					if (auto it = parent->getFunctions.find(headers.target); it != parent->getFunctions.end()) {
						try {
							response.body() = it->second(headers);
						}
						catch (RestError const& e) {
							return do_send(bad_request(e));
						}
						response.content_length(response.body().size());
					}
					else {
						return do_send(not_found(headers.target));
					}
				}
				else if (request.method() == http::verb::post) {
					if (auto it = parent->postFunctions.find(headers.target); it != parent->postFunctions.end()) {
						try {
							response.body() = it->second(headers, request.body());
						}
						catch (RestError const& e) {
							return do_send(bad_request(e));
						}
						response.content_length(response.body().size());
					}
					else {
						return do_send(not_found(headers.target));
					}
				}
				else if (request.method() == http::verb::options) {
					string_response response{ http::status::no_content, request.version() };
					response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					response.set(http::field::access_control_allow_origin, "*");
					response.set(http::field::access_control_allow_methods, "GET, POST");
					response.set(http::field::access_control_allow_headers, "*");
					response.set(http::field::access_control_max_age, "86400");
					response.keep_alive(request.keep_alive());
					response.prepare_payload();
					return do_send(response);
				}
				else {
					return do_send(bad_request(RestError("An error occurred: 'Unknown HTTP-Method'", RestError::Type::BAD_REQUEST)));
				}

				response.prepare_payload();
				return do_send(response);
			}

			void do_send(string_response response) {
				std::shared_ptr<string_response> response_ptr = std::make_shared<string_response>(std::move(response));

				if (auto ptr = std::get_if<beast::tcp_stream>(&stream_v)) {
					http::async_write(
						*ptr,
						*response_ptr,
						[ptr = shared_from_this(), res = response_ptr](beast::error_code ec, size_t bytes_transferred) {
							ptr->handle_send(res->need_eof(), ec, bytes_transferred);
						}
					);
				}
				else if (auto ptr = std::get_if<beast::ssl_stream<beast::tcp_stream>>(&stream_v)) {
					http::async_write(
						*ptr,
						*response_ptr,
						[ptr = shared_from_this(), res = response_ptr](beast::error_code ec, size_t bytes_transferred) {
							ptr->handle_send(res->need_eof(), ec, bytes_transferred);
						}
					);
				}
			}

			void handle_send(bool close, beast::error_code ec, size_t bytes_transferred) {
				if (ec) {
					std::cerr << "Problem writing to Socket: " << ec.what() << std::endl;
					return;
				}
				if (close) {
					return do_close();
				}
				do_read();
			}
		public:
			RestServerSession(RestServerImpl* parent, tcp::socket socket, ssl::context& context, bool useSsl) :
				parent(parent),
				stream_v([&socket, &context, useSsl]() -> std::variant<beast::tcp_stream, beast::ssl_stream<beast::tcp_stream>> {
				if (useSsl) {
					return beast::ssl_stream<beast::tcp_stream>(std::move(socket), context);
				}
				else {
					return beast::tcp_stream(std::move(socket));
				}
					}())
			{

			}
		};

		void stop() {
			shouldStop = true;
			ioContext.stop();
		}

		void join() {
			for (auto& thread : threads) {
				thread.join();
			}
		}

		std::optional<GETFunc> registerGET(std::string_view name, GETFunc func) {
			std::string n{ name };
			if (auto it = getFunctions.find(n); it != getFunctions.end()) {
				std::swap(it->second, func);
				return func;
			}
			else {
				getFunctions[n] = std::move(func);
				return {};
			}
		}
		std::optional<GETFunc> unregisterGET(std::string_view name) {
			std::string n{ name };
			if (auto it = getFunctions.find(n); it != getFunctions.end()) {
				auto ret = make_optional(std::move(it->second));
				getFunctions.erase(it);
				return ret;
			}
			else {
				return {};
			}
		}
		std::optional<POSTFunc> registerPOST(std::string_view name, POSTFunc func) {
			std::string n{ name };
			if (auto it = postFunctions.find(n); it != postFunctions.end()) {
				std::swap(it->second, func);
				return func;
			}
			else {
				postFunctions[n] = std::move(func);
				return {};
			}
		}
		std::optional<POSTFunc> unregisterPOST(std::string_view name) {
			std::string n{ name };
			if (auto it = postFunctions.find(n); it != postFunctions.end()) {
				auto ret = make_optional(std::move(it->second));
				postFunctions.erase(it);
				return ret;
			}
			else {
				return {};
			}
		}

		void do_accept() {
			acceptor.async_accept(
				networking::make_strand(ioContext),
				[this](beast::error_code ec, tcp::socket socket) {
					handle_accept(ec, std::move(socket));
				}
			);
		}

		void handle_accept(beast::error_code ec, tcp::socket socket) {
			if (ec) {
				std::cerr << "Problem Accepting Connection: " << ec.what() << std::endl;
				return;
			}
			auto session = std::make_shared<RestServerSession>(this, std::move(socket), sslContext, sslCert ? true : false);
			session->printRequests = doPrintRequests;
			session->run();
			do_accept();
		}

		void printRequests(bool value) {
			doPrintRequests = value;
		}

	public:
		RestServerImpl(std::string_view name, uint16_t port, std::optional<SSLCert> sslCert, uint32_t maxThreadCount) :
			sslCert(std::move(sslCert)),
			ioContext(maxThreadCount <= 1'024 ? maxThreadCount : 1'024),
			acceptor(networking::make_strand(ioContext)),
			sslContext(ssl::context::tlsv12_server) {
			shouldStop = false;
			//Does 1024 make sense as an upper limit? IDK lol
			maxThreadCount = maxThreadCount <= 1'024 ? maxThreadCount : 1'024;
			for (uint32_t i = 0; i < maxThreadCount; i++) {
				threads.emplace_back([this] {workFunc(shouldStop, ioContext); });
			}
			auto address = networking::ip::make_address("0.0.0.0");
			tcp::endpoint endpoint{ address, port };
			if (this->sslCert) {
				loadServerCertificate(sslContext, *this->sslCert);
			}

			acceptor.open(endpoint.protocol());
			acceptor.set_option(networking::socket_base::reuse_address(true));
			acceptor.bind(endpoint);
			acceptor.listen();
		}
	};

	RestServer::RestServer(std::string_view name, uint16_t port, std::optional<SSLCert> sslCert, uint32_t maxThreadCount) {
		impl = std::make_unique<RestServerImpl>(name, port, std::move(sslCert), maxThreadCount);
	}

	RestServer::~RestServer() {
		impl->stop();
		impl->join();
	}

	void RestServer::start(std::unordered_map<std::string, GETFunc> getFunctions, std::unordered_map<std::string, POSTFunc> postFunctions) {
		for (auto& [name, func] : getFunctions) {
			impl->getFunctions[name] = std::move(func);
		}
		for (auto& [name, func] : postFunctions) {
			impl->postFunctions[name] = std::move(func);
		}
		impl->do_accept();
	}

	void RestServer::printRequests(bool value) {
		impl->printRequests(value);
	}
}