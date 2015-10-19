#pragma once

#include <mp2p/network/messages.hh>
#include <mp2p/misc/shared-buffer.hh>

#include <vector>
#include <iosfwd>

namespace network
{
  class Packet
  {
  public:
    using message_container = std::vector<masks::message_type>;

    // Create a packet with a pointer to data and a size.
    // The data is copied to a SharedBuffer
    Packet(masks::size_type size, masks::fromto_type fromto,
           masks::what_type what, const masks::CharT* data);

    // Create a packet with a pointer to data and a size.
    // If to_copy is set to copy::Yes, the content is going to be copied
    // to a new buffer.
    // If to_copy is set to copy::No, make sure the pointer to data is
    // going to stay valid during the usage of the packet
    Packet(masks::size_type size, masks::fromto_type fromto,
           masks::what_type what, const masks::CharT* data,
           misc::SharedBuffer::copy to_copy);

    // Create a packet with a message contained inside a SharedBuffer
    Packet(masks::fromto_type fromto, masks::what_type what,
           const misc::SharedBuffer& message);

    // Create a packet with a message contained inside a shared pointer.
    // The shared_ptr is going to be passed to a SharedBuffer.
    Packet(masks::fromto_type fromto, masks::what_type what,
           std::vector<masks::CharT>&& data);

    // Create an empty packet without any message.
    // In order to add new messages, use add_message.
    Packet(const masks::PACKET_HEADER& header);
    Packet(masks::fromto_type fromto, masks::what_type what);

    // Add a message to the packet. Usually used for sending
    void add_message(const masks::message_type& message);

    // Add a message to the packet.
    // Set to_copy to copy::Yes if you want the data
    // to be copied to a new buffer.
    // If not, the data should remain valid during the usage of the packet
    template <typename T>
    void add_message(const T* data, const masks::size_type size,
                     misc::SharedBuffer::copy to_copy);

    // Add a message to the packet by copying the internal data
    // Equivalent to add_message(message.data(), message.size(), copy::Yes)
    void copy_message(const masks::message_type& message);

    // Accessors
    masks::size_type size_get() const;

    masks::fromto_type fromto_get() const;

    masks::what_type what_get() const;

    message_container& message_seq_get();
    const message_container& message_seq_get() const;

    // Serialize the header of the message to a header to a buffer.
    // It should be added to a sending sequence as well.
    const masks::message_type serialize_header() const;

  private:
    masks::PACKET_HEADER header_;
    message_container message_seq_;
  };

  // FromTo to ToFrom
  masks::fromto_type fromto_inverse(masks::fromto_type fromto);

  // Create an empty message of a precise type.
  masks::message_type empty_message(masks::size_type size);

  // Print a packet's header on an output stream
  std::ostream& operator<<(std::ostream& output, const Packet& packet);
} // namespace network

#include <mp2p/network/packet.hxx>
