#include <mp2p/network/session.hh>
#include <mp2p/network/messages.hh>
#include <mp2p/network/misc.hh>
#include <mp2p/misc/shared-buffer.hh>
#include <mp2p/misc/logger.hh>

#include <thread>

namespace network
{
  namespace b_asio = boost::asio;
  using namespace network::masks;
  using copy = misc::SharedBuffer::copy;

  namespace
  {
    template <typename Func>
    void
    receive_header(SessionPtr socket, dispatcher_type callback, Func f)
    {
      b_asio::async_read(
        socket->socket_get(),
        b_asio::buffer(&*socket->buff_get().begin(), socket->buff_get().size()),
        b_asio::transfer_exactly(sizeof(PACKET_HEADER)),
        [
          s = std::move(socket),
          call = std::move(callback),
          then = std::forward<Func>(f)
        ](boost::system::error_code ec, size_t msg_length)
        {
          if (!ec && msg_length == sizeof(PACKET_HEADER))
          {
            const auto header
              = reinterpret_cast<const PACKET_HEADER*>(s->buff_get().data());

            misc::Logger::cout() << "[" + std::to_string(s->id()) + "] "
                                      + "Receiving a message of size: "
                                      + std::to_string(header->size);

            // We're not using the constructor of packet with the full
            // header
            // like Packet{*header} because we don't want the size to
            // be specified inside the header yet.
            // Adding the empty message is going to update the size
            Packet p{header->type.fromto, header->type.what};

            // Allocate a buffer to read the message into
            p.add_message(empty_message(header->size));

            // Read the whole message
            then(std::move(s), std::move(p), std::move(call));
          }
          else if (ec != b_asio::error::eof)
            misc::Logger::cerr() << "[" + std::to_string(s->id()) + "] "
                                      + "Error while getting size: "
                                      + ec.message();
        });
    }

    template <typename P>
    void
    receive_message(SessionPtr socket, P&& packet, dispatcher_type call)
    {
      b_asio::async_read(socket->socket_get(), packet.message_seq_get().front(),
                         b_asio::transfer_exactly(packet.size_get()),
                         [
                           s = std::move(socket),
                           p = std::move(packet),
                           callback = std::move(call)
                         ](boost::system::error_code ec, size_t)
                         {
                           if (!ec)
                           {
                             const auto result = callback(std::move(p), *s);

                             if (result == keep_alive::Yes)
                               receive(std::move(s));
                           }
                           else
                             misc::Logger::cerr()
                               << "[" + std::to_string(s->id()) + "] "
                                    + "Error: " + ec.message();
                         });
    }
  }

  Session::Session(b_asio::ip::tcp::socket&& socket, dispatcher_type dispatcher,
                   size_t id)
    : socket_{std::move(socket)}
    , dispatcher_{std::move(dispatcher)}
    , id_{id}
  {
    std::ostringstream s;
    s << std::this_thread::get_id();
    misc::Logger::cout() << "[" + std::to_string(id_) + "] "
                              + "Opened session (tid=" + s.str() + ")";
  }

  Session::Session(b_asio::io_service& io_service, const std::string& host,
                   port_type port, dispatcher_type dispatcher, size_t id)
    : socket_{io_service}
    , dispatcher_{std::move(dispatcher)}
    , id_{id}
  {
    b_asio::ip::tcp::endpoint endpoint
      = network::endpoint_from_host(host, port);

    misc::Logger::cout() << "[" + std::to_string(id_) + "] "
                              + "Opening session (" + host + " - "
                              + std::to_string(port) + ")";

    socket_.connect(endpoint); // Connect to the endpoint

    std::ostringstream s;
    s << std::this_thread::get_id();
    misc::Logger::cout() << "[" + std::to_string(id_) + "] "
                              + "Opened session (tid=" + s.str() + ")";
  }

  Session::~Session()
  {
    misc::Logger::cout() << "[" + std::to_string(id_) + "] " + "Closed session";
  }

  void
  receive(SessionPtr s)
  {
    receive(s, s->dispatcher_get());
  }

  void
  receive(SessionPtr s, dispatcher_type callback)
  {
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    receive_header(s, std::move(callback), receive_message<Packet>);
  }

  void
  blocking_receive(SessionPtr s)
  {
    blocking_receive(s, s->dispatcher_get());
  }

  void
  blocking_receive(SessionPtr s, dispatcher_type callback)
  {
    std::ostringstream ss;
    ss << std::this_thread::get_id();

    std::array<char, sizeof(masks::PACKET_HEADER)> packet_buff;

    try
    {
      b_asio::read(s->socket_get(),
                   b_asio::buffer(&*packet_buff.begin(), packet_buff.size()));
    }
    catch (const std::exception& e)
    {
      misc::Logger::cerr() << "[" + std::to_string(s->id()) + "] "
                                + "Error while getting size: "
                                + std::string(e.what());
      return;
    }

    const auto header
      = reinterpret_cast<const PACKET_HEADER*>(packet_buff.data());
    misc::Logger::cout() << "[" + std::to_string(s->id()) + "] "
                              + "Receiving a message of size: "
                              + std::to_string(header->size);
    Packet p{header->type.fromto, header->type.what,
             empty_message(header->size)};
    try
    {
      b_asio::read(s->socket_get(), p.message_seq_get().front(),
                   b_asio::transfer_exactly(header->size));
    }
    catch (const std::exception& e)
    {
      misc::Logger::cerr() << "[" + std::to_string(s->id()) + "] " + "Error: "
                                + std::string(e.what());
      return;
    }

    auto result = callback(std::move(p), *s);

    if (result == keep_alive::Yes)
      blocking_receive(s, std::move(callback));
  }

  void
  send(SessionPtr s, const Packet& packet)
  {
    send(std::move(s), packet, [](auto, auto&)
         {
           return keep_alive::No;
         });
  }

  void
  send(SessionPtr socket, const Packet& packet, dispatcher_type call)
  {
    // FIXME : Ugly
    auto p = std::make_shared<Packet>(packet);
    auto& seq = p->message_seq_get();
    seq.insert(seq.begin(), packet.serialize_header());

    b_asio::async_write(
      socket->socket_get(), seq,
      [ s = std::move(socket), callback = std::move(call), p ](
        boost::system::error_code ec, size_t)
      {
        if (!ec)
          callback(std::move(*p), *s);
        else
          misc::Logger::cerr() << "[" + std::to_string(s->id()) + "] "
                                    + "Error while sending: " + ec.message();
      });
  }

  void
  blocking_send(SessionPtr s, const Packet& packet)
  {
    blocking_send(s, packet, [](auto, auto&)
                  {
                    return keep_alive::No;
                  });
  }

  void
  blocking_send(SessionPtr s, const Packet& packet, dispatcher_type callback)
  {
    auto seq = packet.message_seq_get();
    seq.insert(seq.begin(), packet.serialize_header());
    try
    {
      b_asio::write(s->socket_get(), seq);
      callback(std::move(packet), *s);
    }
    catch (const std::exception& e)
    {
      misc::Logger::cerr() << "[" + std::to_string(s->id()) + "] "
                                + "Error while sending: "
                                + std::string(e.what());
    }
  }

  size_t
  Session::unique_id()
  {
    static std::atomic_size_t id{1};
    return id++;
  }

  void
  send_ack(Session& session, const Packet& packet, error_code ack)
  {
    const fromto_type fromto_src = packet.fromto_get();
    const fromto_type fromto_dst = fromto_inverse(fromto_src);
    const masks::ack response{ack};

    Packet to_send{fromto_dst, ack_w};
    to_send.add_message(&response, sizeof(response), copy::No);
    blocking_send(session.ptr(), std::move(to_send));
  }

  void
  recv_ack(Session& session)
  {
    blocking_receive(session.ptr(), [](Packet p, Session&)
                     {
                       const CharT* data = p.message_seq_get().front().data();
                       const ack& ack_code
                         = *reinterpret_cast<const ack*>(data);
                       if (ack_code != error_code::success)
                       {
                         std::ostringstream ss;
                         ss << "Error : " << (int)ack_code;
                         throw std::logic_error(ss.str());
                       }

                       return keep_alive::No;
                     });
  }
} // namespace network
