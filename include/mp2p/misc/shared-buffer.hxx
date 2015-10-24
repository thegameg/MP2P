#pragma once

#include <mp2p/misc/shared-buffer.hh>
#include <mp2p/misc/shared-buffer-impl.hh>

#include <memory>

namespace misc
{
  namespace b_asio = boost::asio;

  template <typename CharT>
  BasicSharedBuffer<CharT>::BasicSharedBuffer(size_t size)
    : impl_{std::make_shared<OwnerImpl>(size)}
  {
  }

  template <typename CharT>
  BasicSharedBuffer<CharT>::BasicSharedBuffer(container_type&& container)
    : impl_{std::make_shared<OwnerImpl>(std::move(container))}
  {
  }

  template <typename CharT>
  BasicSharedBuffer<CharT>::BasicSharedBuffer(CharT* data, size_t size,
                                              copy to_copy)
  {
    if (to_copy == copy::Yes)
      impl_ = std::make_shared<OwnerImpl>(data, size);
    else
      impl_ = std::make_shared<WeakImpl>(data, size);
  }

  template <typename CharT>
  BasicSharedBuffer<CharT>::BasicSharedBuffer(const CharT* data, size_t size,
                                              copy to_copy)
  {
    if (to_copy == copy::Yes)
      impl_ = std::make_shared<OwnerImpl>(data, size);
    else
      impl_ = std::make_shared<WeakImpl>(data, size);
  }

  template <typename CharT>
  inline typename BasicSharedBuffer<CharT>::const_iterator
  BasicSharedBuffer<CharT>::begin() const
  {
    return &impl_->buf_;
  }

  template <typename CharT>
  inline typename BasicSharedBuffer<CharT>::const_iterator
  BasicSharedBuffer<CharT>::end() const
  {
    return &impl_->buf_ + 1;
  }

  template <typename CharT>
  inline CharT*
  BasicSharedBuffer<CharT>::data()
  {
    return b_asio::buffer_cast<CharT*>(impl_->buf_);
  }

  template <typename CharT>
  inline const CharT*
  BasicSharedBuffer<CharT>::data() const
  {
    return b_asio::buffer_cast<const CharT*>(impl_->buf_);
  }

  template <typename CharT>
  inline const typename BasicSharedBuffer<CharT>::value_type
  BasicSharedBuffer<CharT>::buffer_get() const
  {
    return impl_->buf_;
  }

  template <typename CharT>
  inline size_t
  BasicSharedBuffer<CharT>::size() const
  {
    return b_asio::buffer_size(impl_->buf_);
  }

  template <typename CharT>
  inline const std::string
  BasicSharedBuffer<CharT>::string_get() const
  {
    return std::string(data(), data() + size());
  }

  template <typename CharT>
  inline BasicSharedBuffer<CharT>::operator b_asio::const_buffer() const
  {
    return b_asio::const_buffer(impl_->buf_);
  }
} // namespace misc
