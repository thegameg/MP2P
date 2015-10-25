#include <mp2p/network/server.hh>
#include <mp2p/network/session.hh>
#include <mp2p/misc/logger.hh>

#include <boost/asio.hpp>
#include <thread>

namespace network
{
  namespace b_asio = boost::asio;

  Server::Server(b_asio::ip::address_v6 addr, port_type port,
                 b_asio::io_service& io_service, dispatcher_type dispatcher)
    : acceptor_{io_service}
    , socket_{io_service}
    , dispatcher_{dispatcher}
  {
    // Use of ipv6 by default, with IPV6_V6ONLY disabled, it will listen to
    // both ipv4 & ipv6.
    // ipv4 addresses will be mapped to ipv6 like this: `::ffff:192.168.0.'
    b_asio::ip::tcp::endpoint endpoint(addr, port);

    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(b_asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    misc::Logger::cout() << "Successfully binded " + addr.to_string() + "/"
                              + std::to_string(port);

    listen(); // Listen for new connections
  }

  void
  Server::listen()
  {
    misc::Logger::cout() << "Listening...";

    acceptor_.async_accept(
      socket_, [this](boost::system::error_code ec)
      {
        if (!ec)
        {
          std::ostringstream s;
          s << std::this_thread::get_id();
          misc::Logger::cout()
            << "Connection accepted. (Thread " + s.str() + ").";

          const auto session = Session::create(std::move(socket_), dispatcher_);

          receive(session);

          // At the end of each request & treatment, we call listen again.
          listen();
        }
        else
          misc::Logger::cerr() << "An error occurred while listening.";
      });
  }
} // namespace network
