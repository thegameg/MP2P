#pragma once

#include <mp2p/network/network.hh>

#include <boost/asio.hpp>

namespace network
{
  class Server
  {
  public:
    // Create a server binding addr:port using io_service.
    // Callback dispatcher after a recieve
    Server(boost::asio::ip::address_v6 addr, uint16_t port,
           boost::asio::io_service& io_service, dispatcher_type dispatcher);

    // Stop the acceptor
    // FIXME : Is this really necessary? What about RAII?
    ~Server();

    // Listen asynchronously for new connections
    void listen();

    // Stop the acceptor
    // FIXME : Is this really necessary? What about RAII?
    void stop();

    bool is_running() const;

  private:
    // The acceptor used to accept new connections and create sessions
    boost::asio::ip::tcp::acceptor acceptor_;

    // The socket used for listening for new connections
    boost::asio::ip::tcp::socket socket_;

    // The recieve callback
    dispatcher_type dispatcher_;
  };
} // namespace network
