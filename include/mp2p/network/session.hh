#pragma once

#include <functional>
#include <boost/asio.hpp>

#include <mp2p/network/packet.hh>

namespace network
{
  class Session : public std::enable_shared_from_this<Session>
  {
  public:
    // Factory function
    template <typename... Ts>
    static std::shared_ptr<Session> create(Ts&&... params);

  public:
    // Create a session from an actual socket.
    // The dispatcher is called when a packet is transferred.
    // The delete_handler is removing the actual session
    // from the container containing it.
    Session(boost::asio::ip::tcp::socket&& socket,
            dispatcher_type dispatcher,
            size_t id = unique_id());

    // Same with the previous one.
    // The socket parameters are explicitly speciied
    Session(boost::asio::io_service& io_service,
            const std::string& host,
            uint16_t port,
            dispatcher_type dispatcher
              = [](Packet, Session&)
              { return keep_alive::No; },
            size_t id = unique_id());

    // Used for debug
    ~Session();

    // Returns THE shared_ptr associated to the Session
    std::shared_ptr<Session> ptr();

    // Get the corresponding socket
    boost::asio::ip::tcp::socket& socket_get();

    // Get the address of the remote client
    boost::asio::ip::address remote_address_get() const;

    // Get the buffer containing the header
    std::array<char, sizeof(masks::PACKET_HEADER)>& buff_get();

    // Get the dispatcher
    dispatcher_type dispatcher_get() const;

    // The unique session id
    size_t id_get() const;

    // Creates an unique id for a socket.
    // It's using an atomic integer
    static size_t unique_id();

  private:
    // The socket opened for communication
    boost::asio::ip::tcp::socket socket_;

    // The array containing the header of the packet
    // FIXME : Why is it part of the session ?
    std::array<char, sizeof (masks::PACKET_HEADER)> buff_;

    // The dispatcher to call right after a complete recieve
    dispatcher_type dispatcher_;

    // The unique id of the session
    size_t id_;
  };

  // Recieve the header, then call the callback with a packet
  // and a message size
  void receive_header(std::shared_ptr<Session> s,
                      std::function<void(const Packet&,
                                         dispatcher_type)> receive_body,
                      dispatcher_type callback);

  // Recieve the message according to the packet
  void receive_message(std::shared_ptr<Session> s,
                       const Packet& p,
                       dispatcher_type dispatcher);

  // Recieve a header, then the data according to the header
  void receive(std::shared_ptr<Session> s);

  // Receive a header, treat the packet inside a lambda function
  void receive(std::shared_ptr<Session> s, dispatcher_type callback);

  // Same as receive, but blocking
  void blocking_receive(std::shared_ptr<Session> s);

  // Same as receive, but blocking
  void blocking_receive(std::shared_ptr<Session> s, dispatcher_type callback);

  // Send a packet
  void send(std::shared_ptr<Session> s, const Packet& packet);

  // Send a packet
  void send(std::shared_ptr<Session> s, const Packet& packet,
            dispatcher_type callback);

  // This operation is blocking. It's using a synchronous send
  void blocking_send(std::shared_ptr<Session> s, const Packet& packet);

  // Send a packet using a custom dispatcher
  void blocking_send(std::shared_ptr<Session> s, const Packet& packet,
                     dispatcher_type callback);

  // Compare two Sessions according to their id
  bool operator==(const Session& lhs, const Session& rhs);

  // Send acknowledge (error, or not) to the session according
  // to the packet's header
  void send_ack(Session& session, const Packet& packet, enum error_code ack);

  // Receive acknowledge from the session
  // Throws if any error occurs
  void recv_ack(Session& session);
} // namespace network

#include <mp2p/network/session.hxx>
