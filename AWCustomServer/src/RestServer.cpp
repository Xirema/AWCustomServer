#include <RestServer.h>

#include <atomic>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/mysql.hpp>
#include <iostream>
#include <mutex>
#include <variant>
#include<print>
#include<format>

namespace net {
namespace networking = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;
using tcp = networking::ip::tcp;
namespace ssl = boost::asio::ssl;

namespace {
#ifdef NDEBUG
constexpr bool DEBUGGING = false;
#else
constexpr bool DEBUGGING = true;
#endif
}  // namespace

void loadServerCertificate(ssl::context &ctx, SSLCert const &sslCert) {
  ctx.set_options(
      ssl::context::default_workarounds |
      ssl::context::no_sslv2 |
      ssl::context::single_dh_use);
  ctx.use_certificate_chain(
      boost::asio::buffer(sslCert.cert.data(), sslCert.cert.size()));
  ctx.use_private_key(
      boost::asio::buffer(sslCert.key.data(), sslCert.key.size()),
      ssl::context::file_format::pem);
}

namespace {
void workFunc(std::atomic_bool &shouldStop, networking::io_context &ioContext) {
  if constexpr (DEBUGGING) {
    std::println("DEBUG: workFunc()");
  }
  while (!shouldStop) {
    try {
      ioContext.run();
    } catch (...) {
      try {
        std::rethrow_exception(std::current_exception());
      } catch (std::exception const &e) {
        std::println(std::cerr, "EXCEPTION THROWN: {}", e.what());
      }
    }
  }
}
}  // namespace
class RestServerImpl {
  friend class RestServer;
  friend class RestServerSession;
  std::vector<std::thread> threads;
  HTTPFunctionMap functions;
  std::atomic_bool shouldStop;
  networking::io_context ioContext;
  std::unique_ptr<networking::executor_work_guard<networking::io_context::executor_type>> work_ptr;
  tcp::acceptor acceptor;
  std::mutex funcMutex;
  std::optional<SSLCert> sslCert;
  ssl::context sslContext;
  bool doPrintRequests = false;

  class RestServerSession : public std::enable_shared_from_this<RestServerSession> {
   protected:
    friend RestServerImpl;
    RestServerImpl *parent;
    std::variant<beast::tcp_stream, ssl::stream<beast::tcp_stream>> stream_v;
    beast::flat_buffer buffer;
    using string_request = http::request<http::string_body>;
    using string_response = http::response<http::string_body>;
    bool printRequests = false;
    std::chrono::steady_clock::time_point start;
    std::string currentEndpoint;

    void run() {
      networking::dispatch(
          networking::make_strand(parent->ioContext),
          [ptr = shared_from_this()] {
            ptr->do_handshake();
          });
    }

    void do_handshake() {
      struct Visitor {
        RestServerSession &p;
        void operator()(beast::tcp_stream &stream) const {
          stream.expires_after(std::chrono::seconds(30));
          networking::dispatch(
              stream.get_executor(),
              [ptr = p.shared_from_this()] {
                ptr->handle_handshake({});
              });
        }
        void operator()(ssl::stream<beast::tcp_stream> &stream) const {
          beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(30));
          stream.async_handshake(
              ssl::stream_base::server,
              [ptr = p.shared_from_this()](beast::error_code ec) {
                ptr->handle_handshake(ec);
              });
        }

        Visitor(RestServerSession &p) : p(p) {}
      } v{*this};
      std::visit(v, stream_v);
    }

    void handle_handshake(beast::error_code ec) {
      if (ec) {
        std::println(std::cerr, "Problem performing handshake: {}", ec.what());
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
            });
      } else if (auto ptr = std::get_if<ssl::stream<beast::tcp_stream>>(&stream_v)) {
        http::async_read(
            *ptr,
            buffer,
            *parser_ptr,
            [ptr = shared_from_this(), parser = parser_ptr](beast::error_code ec, size_t bytes_transferred) {
              ptr->handle_read(*parser, ec, bytes_transferred);
            });
      }
    }

    void do_close() {
      if (auto ptr = std::get_if<beast::tcp_stream>(&stream_v)) {
        ptr->socket().shutdown(tcp::socket::shutdown_send);
      } else if (auto ptr = std::get_if<ssl::stream<beast::tcp_stream>>(&stream_v)) {
        beast::get_lowest_layer(*ptr).socket().shutdown(tcp::socket::shutdown_send);
      }
    }

    void handle_read(http::request_parser<http::string_body> &request_parser, beast::error_code ec, size_t bytes_transferred) {
      start = std::chrono::steady_clock::now();
      currentEndpoint = std::string{request_parser.get().target()};
      if (ec == http::error::end_of_stream) {
        do_close();
        return;
      }
      if (ec == beast::error::timeout) {
        // Timeouts are not serious errors, and can be ignored.
        return;
      }
      if (ec) {
        std::println(std::cerr, "Problem reading from Socket: {}", ec.what());
        return;
      }
      handle_request(request_parser.get());
    }

    void handle_request(string_request request) {
      if (printRequests) {
        std::stringstream ss;
        ss << request;
        std::println("{}", ss.str());
      }
      // Returns a bad request response
      auto const bad_request =
          [&request](RestError const &e) {
            http::status status = [type = e.type] {
              if (type == RestErrorType::BAD_REQUEST)
                return http::status::bad_request;
              if (type == RestErrorType::INTERNAL_ERROR)
                return http::status::internal_server_error;
              if (type == RestErrorType::INVALID_DATA)
                return http::status::bad_request;
              if (type == RestErrorType::NOT_FOUND)
                return http::status::not_found;
              return http::status::unknown;
            }();
            string_response response{status, request.version()};
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
          [&request](beast::string_view target) {
            string_response response{http::status::not_found, request.version()};
            response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            response.set(http::field::content_type, "text/html");
            response.set(http::field::access_control_allow_origin, "*");
            response.keep_alive(request.keep_alive());
            response.body() = "The resource '" + std::string(target) + "' was not found.";
            response.content_length(response.body().size());
            response.prepare_payload();
            return response;
          };

      string_response response{http::status::ok, request.version()};
      response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
      response.set(http::field::content_type, "application/json");
      response.set(http::field::access_control_allow_origin, "*");
      response.keep_alive(request.keep_alive());
      HTTPHeaders headers;
      headers.target = std::string{request.target()};
      for (auto const &val : request.base()) {
        headers.httpHeaders[std::string{val.name_string()}] = std::string{val.value()};
      }

      HTTPVerb verb;
      auto method = request.method();
      switch (method) {
        case http::verb::get:
          verb = HTTPVerb::GET;
          break;
        case http::verb::post:
          verb = HTTPVerb::POST;
          break;
        case http::verb::put:
          verb = HTTPVerb::PUT;
          break;
        case http::verb::delete_:
          verb = HTTPVerb::DELETE;
          break;
        case http::verb::options:
          verb = HTTPVerb::OPTIONS;
          break;
        default:
          return do_send(bad_request(RestError("An error occurred: 'Unhandled HTTP-Method'", RestErrorType::BAD_REQUEST)));
      }

      try {
        if (verb == HTTPVerb::OPTIONS) {
          string_response response{http::status::no_content, request.version()};
          response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
          response.set(http::field::access_control_allow_origin, "*");
          response.set(http::field::access_control_allow_methods, "GET, POST, PUT, DELETE");
          response.set(http::field::access_control_allow_headers, "*");
          response.set(http::field::access_control_max_age, "86400");
          response.keep_alive(request.keep_alive());
          response.prepare_payload();
          return do_send(response);
        } else {
          HTTPFunctionDefinition definition{.verb = verb, .name = headers.target};
          if (auto it = parent->functions.find(definition); it != parent->functions.end()) {
            response.body() = it->second(headers, request.body());
            response.content_length(response.body().size());
          } else {
            return do_send(not_found(headers.target));
          }
        }
      } catch (RestError const &e) {
        return do_send(bad_request(e));
      } catch (boost::mysql::error_with_diagnostics const &e) {
        return do_send(bad_request(RestError("Internal SQL Error: '" + std::string(e.get_diagnostics().client_message()) + "'/'" + std::string(e.get_diagnostics().server_message()) + "'", RestErrorType::INTERNAL_ERROR)));
      } catch (...) {
        try {
          std::rethrow_exception(std::current_exception());
        } catch (std::exception const &e) {
          return do_send(bad_request(RestError("Internal Error: " + std::string(e.what()), net::RestErrorType::INTERNAL_ERROR)));
        }
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
            });
      } else if (auto ptr = std::get_if<ssl::stream<beast::tcp_stream>>(&stream_v)) {
        http::async_write(
            *ptr,
            *response_ptr,
            [ptr = shared_from_this(), res = response_ptr](beast::error_code ec, size_t bytes_transferred) {
              ptr->handle_send(res->need_eof(), ec, bytes_transferred);
            });
      }
    }

    void handle_send(bool close, beast::error_code ec, size_t bytes_transferred) {
      if constexpr (DEBUGGING) {
        auto duration = std::chrono::steady_clock::now() - start;
        std::println("Duration of request '{}': {}", currentEndpoint, duration);
      }
      if (ec) {
        std::println(std::cerr, "Problem writing to Socket: {}", ec.what());
        return;
      }
      if (close) {
        return do_close();
      }
      do_read();
    }

   public:
    RestServerSession(
        RestServerImpl *parent,
        tcp::socket socket,
        ssl::context &context,
        bool useSsl) : parent(parent),
                       stream_v([&socket, &context, useSsl]() -> std::variant<beast::tcp_stream, ssl::stream<beast::tcp_stream>> {
	    if (useSsl) {
	      return ssl::stream<beast::tcp_stream>(std::move(socket), context);
	    }
	    else {
	      return beast::tcp_stream(std::move(socket));
	    } }()),
      start(std::chrono::steady_clock::now())
    {
    }
  };

  void stop() {
    shouldStop = true;
    work_ptr.reset();
    ioContext.stop();
  }

  void join() {
    for (auto &thread : threads) {
      thread.join();
    }
  }

  void do_accept() {
    if constexpr (DEBUGGING) {
      std::println("DEBUG: do_accept();");
    }
    acceptor.async_accept(
        networking::make_strand(ioContext),
        [this](beast::error_code ec, tcp::socket socket) {
          handle_accept(ec, std::move(socket));
        });
    if constexpr (DEBUGGING) {
      std::println("DEBUG: do_accept()_end;");
    }
  }

  void handle_accept(beast::error_code ec, tcp::socket socket) {
    if constexpr (DEBUGGING) {
      std::println("DEBUG: Connection Accepted!");
    }
    if (ec) {
      std::println(std::cerr, "Problem Accepting Connection: {}", ec.what());
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
  RestServerImpl(
      std::string_view name,
      uint16_t port,
      std::optional<SSLCert> sslCert,
      uint32_t maxThreadCount) : ioContext(maxThreadCount <= 1'024 ? maxThreadCount : 1'024),
                                 work_ptr(std::make_unique<networking::executor_work_guard<networking::io_context::executor_type>>(ioContext.get_executor())),
                                 acceptor(networking::make_strand(ioContext)),
                                 sslCert(std::move(sslCert)),
                                 sslContext(ssl::context::tlsv12_server) {
    shouldStop = false;
    // Does 1024 make sense as an upper limit? IDK lol
    maxThreadCount = maxThreadCount <= 1'024 ? maxThreadCount : 1'024;
    if constexpr (DEBUGGING) {
      std::println("DEBUG: Num of Threads: {}", maxThreadCount);
    }
    for (uint32_t i = 0; i < maxThreadCount; i++) {
      threads.emplace_back([this] { workFunc(shouldStop, ioContext); });
    }
    auto address = networking::ip::make_address("0.0.0.0");
    tcp::endpoint endpoint{address, port};
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

void RestServer::start(HTTPFunctionMap functions) {
  impl->functions = std::move(functions);
  impl->do_accept();
  if constexpr (DEBUGGING) {
    std::println("DEBUG: NOw Accepting Connections!");
  }
}

void RestServer::printRequests(bool value) {
  impl->printRequests(value);
}

std::optional<std::string_view> HTTPHeaders::getHeader(std::string const &header) const {
  if (auto it = this->httpHeaders.find(header); it != this->httpHeaders.end()) {
    return it->second;
  }
  return {};
}
std::optional<std::string_view> HTTPHeaders::getParameter(std::string const &parameter) const {
  if (auto it = this->httpParameters.find(parameter); it != this->httpParameters.end()) {
    return it->second;
  }
  return {};
}
}  // namespace net
