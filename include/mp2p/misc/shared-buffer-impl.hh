#pragma once

#include <mp2p/misc/shared-buffer.hh>

namespace misc
{
  // The abstract structure of the implementation
  // This allows the implementation of the BasicSharedBuffer<CharT> to be
  // chosen at runtime, without having empty containers and null pointers
  // wandering around.
  namespace b_asio = boost::asio;

  template <typename CharT>
  struct BasicSharedBuffer<CharT>::Impl
  {
    using container_type = BasicSharedBuffer<CharT>::container_type;

    Impl() = default;

    Impl(const b_asio::mutable_buffer& buf)
      : buf_{buf}
    {
    }

    virtual ~Impl() = default;

    // Mutable buffer used to interact with b_asio
    b_asio::mutable_buffer buf_;
  };

  // The owning implementation copies the data to an internal container
  // It's used to keep data alive accross function calls
  template <typename CharT>
  struct BasicSharedBuffer<CharT>::OwnerImpl
    : public BasicSharedBuffer<CharT>::Impl
  {
    using super_type = typename BasicSharedBuffer<CharT>::Impl;
    using container_type = typename super_type::container_type;

    // Copy the data in the container
    OwnerImpl(const CharT* data, size_t size)
      : data_{data, data + size}
    {
      super_type::buf_ = b_asio::mutable_buffer{data_.data(), data_.size()};
    }

    OwnerImpl(size_t size)
      : data_(size, '\0')
    {
      super_type::buf_ = b_asio::mutable_buffer{data_.data(), data_.size()};
    }

    OwnerImpl(container_type&& container)
      : data_{std::move(container)}
    {
    }

    OwnerImpl(size_t size, CharT default_char)
      : data_(size, default_char)
    {
      super_type::buf_ = b_asio::mutable_buffer{data_.data(), data_.size()};
    }

    // Underlaying container
    container_type data_;
  };

  // The weak implementation gives the ownership to the constructor
  // of the data
  template <typename CharT>
  struct BasicSharedBuffer<CharT>::WeakImpl
    : public BasicSharedBuffer<CharT>::Impl
  {
    using super_type = typename BasicSharedBuffer<CharT>::Impl;
    using container_type = typename super_type::container_type;

    WeakImpl(const CharT* data, size_t size)
      : super_type{b_asio::mutable_buffer(const_cast<CharT*>(data), size)}
    {
    }
  };
} // namespace misc
