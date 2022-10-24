#pragma once

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>

#include <cstdint>
#include <string>
#include <unordered_map>

namespace expresso {
class application {
public:
  application();

  void listen(const boost::asio::ip::tcp::endpoint &endpoint);

private:
  boost::asio::io_context _io_context;
  boost::asio::ip::tcp::socket _socket;

  void do_accept_(boost::asio::ip::tcp::acceptor &acceptor);
};
} // namespace expresso
