#pragma once

namespace network
{
  inline bool
  Server::is_running() const
  {
    return acceptor_.is_open();
  }
} // namespace network
