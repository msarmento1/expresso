#include <expresso/application.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Factorials are computed", "[factorial]") {
  expresso::application app;
  app.listen(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address("0.0.0.0"), 3000));
}