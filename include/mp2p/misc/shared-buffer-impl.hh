#pragma once

#include <mp2p/misc/shared-buffer.hh>

namespace misc
{
  // The abstract structure of the implementation
  // This allows the implementation of the SharedBuffer to be chosen
  // at runtime, without having empty containers and null pointers
  // wandering around.
  struct SharedBuffer::SharedBufferImpl
  {
    using CharT = SharedBuffer::CharT;
    using container_type = SharedBuffer::container_type;

    SharedBufferImpl() = default;

    SharedBufferImpl(const boost::asio::mutable_buffer& buf)
      : buf_{buf}
    {
    }

    virtual ~SharedBufferImpl() = default;

    // Mutable buffer used to interact with boost::asio
    boost::asio::mutable_buffer buf_;
  };

  // The owning implementation copies the data to an internal container
  // It's used to keep data alive accross function calls
  struct OwnerImpl : SharedBuffer::SharedBufferImpl
  {
    using super_type = SharedBufferImpl;

    // Copy the data in the container
    OwnerImpl(const super_type::CharT* data, size_t size)
      : data_{data, data + size}
    {
      buf_ = boost::asio::mutable_buffer{data_.data(), data_.size()};
    }

    OwnerImpl(size_t size)
      : data_(size, '\0')
    {
      buf_ = boost::asio::mutable_buffer{data_.data(), data_.size()};
    }

    OwnerImpl(super_type::container_type&& container)
      : data_{std::move(container)}
    {
    }

    OwnerImpl(size_t size, super_type::CharT default_char)
      : data_(size, default_char)
    {
      buf_ = boost::asio::mutable_buffer{data_.data(), data_.size()};
    }

    // Underlaying container
    container_type data_;
  };

  // The weak implementation gives the ownership to the constructor
  // of the data
  struct WeakImpl : SharedBuffer::SharedBufferImpl
  {
    using super_type = SharedBufferImpl;

    WeakImpl(const super_type::CharT* data, size_t size)
      : super_type{boost::asio::mutable_buffer(
          const_cast<super_type::CharT*>(data), size)}
    {
    }
  };
} // namespace misc
