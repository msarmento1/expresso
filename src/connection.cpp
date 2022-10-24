#include <expresso/connection.hpp>

#include <chrono>

namespace expresso {
connection::connection(boost::asio::ip::tcp::socket socket)
    : _socket(std::move(socket)), _buffer(8192), _request(), _response(),
      _deadline(_socket.get_executor(), std::chrono::seconds(60)) {}

void connection::start() {
  read_request();
  check_deadline();
}

void connection::read_request() {
  auto self = shared_from_this();

  boost::beast::http::async_read(
      _socket, _buffer, _request,
      [self](boost::beast::error_code ec, std::size_t bytes_transferred) {
        boost::ignore_unused(bytes_transferred);
        if (!ec)
          self->process_request();
      });
}

void connection::process_request() {
  _response.version(_request.version());
  _response.keep_alive(false);

  switch (_request.method()) {
  case boost::beast::http::verb::get:
    _response.result(boost::beast::http::status::ok);
    _response.set(boost::beast::http::field::server, "boost::beast");
    create_response();
    break;

  default:
    // We return responses indicating an error if
    // we do not recognize the request method.
    _response.result(boost::beast::http::status::bad_request);
    _response.set(boost::beast::http::field::content_type, "text/plain");
    boost::beast::ostream(_response.body())
        << "Invalid request-method '" << std::string(_request.method_string())
        << "'";
    break;
  }

  write_response();
}

void connection::create_response() {
  if (_request.target() == "/count") {
    _response.set(boost::beast::http::field::content_type, "text/html");
    boost::beast::ostream(_response.body())
        << "<html>\n"
        << "<head><title>Request count</title></head>\n"
        << "<body>\n"
        << "<h1>Request count</h1>\n"
        << "<p>There have been "
        << " requests so far.</p>\n"
        << "</body>\n"
        << "</html>\n";
  } else if (_request.target() == "/time") {
    _response.set(boost::beast::http::field::content_type, "text/html");
    boost::beast::ostream(_response.body())
        << "<html>\n"
        << "<head><title>Current time</title></head>\n"
        << "<body>\n"
        << "<h1>Current time</h1>\n"
        << "<p>The current time is "
        << " seconds since the epoch.</p>\n"
        << "</body>\n"
        << "</html>\n";
  } else {
    _response.result(boost::beast::http::status::not_found);
    _response.set(boost::beast::http::field::content_type, "text/plain");
    boost::beast::ostream(_response.body()) << "File not found\r\n";
  }
}

void connection::write_response() {
  auto self = shared_from_this();

  _response.content_length(_response.body().size());

  boost::beast::http::async_write(
      _socket, _response, [self](boost::beast::error_code ec, std::size_t) {
        self->_socket.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
        self->_deadline.cancel();
      });
}

void connection::check_deadline() {
  auto self = shared_from_this();

  _deadline.async_wait([self](boost::beast::error_code ec) {
    if (!ec) {
      self->_socket.close(ec);
    }
  });
}
} // namespace expresso