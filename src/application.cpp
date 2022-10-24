#include <expresso/application.hpp>
#include <expresso/connection.hpp>

#include <boost/asio.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>

#include <iostream>

namespace expresso {
application::application() : _io_context(1), _socket(_io_context) {}

void application::listen(const boost::asio::ip::tcp::endpoint &endpoint) {
  boost::asio::ip::tcp::acceptor acceptor(_io_context, endpoint);
  do_accept(acceptor);
  _io_context.run();
}

void application::do_accept_(boost::asio::ip::tcp::acceptor &acceptor) {
  acceptor.async_accept(_socket, [&](boost::system::error_code ec) {
    if (!ec) {
      std::make_shared<connection>(std::move(_socket))->start();
    }

    do_accept_(acceptor);
  });
}

} // namespace expresso
