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
    Server(boost::asio::ip::address_v6 addr, port_type port,
           boost::asio::io_service& io_service, dispatcher_type dispatcher);

    // Check if the server is running and accepting new connections
    bool is_running() const;

  private:
    // The acceptor used to accept new connections and create sessions
    boost::asio::ip::tcp::acceptor acceptor_;

    // The socket used for listening for new connections
    boost::asio::ip::tcp::socket socket_;

    // The recieve callback
    dispatcher_type dispatcher_;

    // Listen asynchronously for new connections
    void listen();
  };
} // namespace network

#include <mp2p/network/server.hxx>
