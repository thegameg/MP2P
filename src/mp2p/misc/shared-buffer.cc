#include <mp2p/misc/shared-buffer.hh>

#include <memory>

namespace misc
{
  SharedBuffer::SharedBuffer(size_t size)
    : impl_{std::make_shared<OwnerImpl>(size)}
  {
  }

  SharedBuffer::SharedBuffer(container_type&& container)
    : impl_{std::make_shared<OwnerImpl>(std::move(container))}
  {
  }

  SharedBuffer::SharedBuffer(CharT* data, size_t size, copy to_copy)
  {
    if (to_copy == copy::Yes)
      impl_ = std::make_shared<OwnerImpl>(data, size);
    else
      impl_ = std::make_shared<WeakImpl>(data, size);
  }

  SharedBuffer::SharedBuffer(const CharT* data, size_t size, copy to_copy)
  {
    if (to_copy == copy::Yes)
      impl_ = std::make_shared<OwnerImpl>(data, size);
    else
      impl_ = std::make_shared<WeakImpl>(data, size);
  }
} // namespace misc
