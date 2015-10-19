#pragma once

#include "shared-buffer.hh"
#include "shared-buffer-impl.hh"

namespace misc
{
  inline SharedBuffer::const_iterator
  SharedBuffer::begin() const
  {
    return &impl_->buf_;
  }

  inline SharedBuffer::const_iterator
  SharedBuffer::end() const
  {
    return &impl_->buf_ + 1;
  }

  inline SharedBuffer::CharT*
  SharedBuffer::data()
  {
    return boost::asio::buffer_cast<SharedBuffer::CharT*>(impl_->buf_);
  }

  inline const SharedBuffer::CharT*
  SharedBuffer::data() const
  {
    return boost::asio::buffer_cast<const SharedBuffer::CharT*>(impl_->buf_);
  }

  inline const SharedBuffer::value_type
  SharedBuffer::buffer_get() const
  {
    return impl_->buf_;
  }

  inline size_t
  SharedBuffer::size() const
  {
    return boost::asio::buffer_size(impl_->buf_);
  }

  inline const std::string
  SharedBuffer::string_get() const
  {
    return std::string(data(), data() + size());
  }

  inline SharedBuffer::operator boost::asio::const_buffer() const
  {
    return boost::asio::const_buffer(impl_->buf_);
  }
} // namespace misc
