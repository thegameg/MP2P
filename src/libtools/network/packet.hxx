#pragma once

#include <network.hh>
#include <masks/blocks.hh>

namespace network
{
  template <typename...Messages>
  Packet::Packet(size_type size,
                 fromto_type fromto,
                 what_type what,
                 Messages...messages)
    : header_{size, {fromto, what} },
      message_{messages...}
  {
  }

  inline void Packet::add_message(const message_type& message)
  {
    message_.push_back(message);
  }

  inline size_type Packet::size_get() const
  {
    return header_.size;
  }

  inline fromto_type Packet::fromto_get() const
  {
    return header_.type.fromto;
  }

  inline what_type Packet::what_get() const
  {
    return header_.type.what;
  }

  inline const message_type Packet::message_get() const
  {
    return message_[0];
  }

  inline message_type empty_message(size_type size)
  {
    return utils::shared_buffer(size).buffer_get();
  }

}
