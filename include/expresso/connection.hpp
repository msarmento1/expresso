#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <memory>

namespace expresso {
class connection : public std::enable_shared_from_this<connection> {
public:
  connection(boost::asio::ip::tcp::socket socket);

  // Initiate the asynchronous operations associated with the connection.
  void start();

private:
  boost::asio::ip::tcp::socket _socket;
  boost::beast::flat_buffer _buffer;
  boost::beast::http::request<boost::beast::http::dynamic_body> _request;
  boost::beast::http::response<boost::beast::http::dynamic_body> _response;
  boost::asio::steady_timer _deadline;

  void read_request();
  void process_request();
  void create_response();
  void write_response();
  void check_deadline();
};
} // namespace expresso