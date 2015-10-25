#pragma once

#include <cstdint>
#include <functional>

namespace network
{
  // Error codes according to the protocol
  enum class error_code : uint8_t
  {
    success = 0,        // Success
    error = 1,          // Unknown error
    file_not_found = 3, // File not found
    hash_failed = 4,    // File hash failed
    invalid_packet = 5, // Invalid packet
    missing_storage = 6,
    incomplete_file = 7,
    redundancy = 11 // Not enough storages
  };

  // Keep the connection alive
  enum class keep_alive
  {
    Yes,
    No
  };

  class Session;
  class Packet;

  using dispatcher_type = std::function<keep_alive(Packet, Session&)>;
  using port_type = uint16_t;
} // namespace network
