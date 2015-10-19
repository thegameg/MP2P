#include <mp2p/misc/init.hh>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdexcept>

namespace misc
{
  void
  check_system()
  {
    // FIXME : Use boost asio to check.
    int sock = ::socket(PF_INET6, SOCK_STREAM, 0);

    if (sock == -1)
      throw std::logic_error("Fatal error: unable to initialize"
                             " network check.");

    int val = 0;
    unsigned len = 0;
    if (::getsockopt(sock, IPPROTO_IPV6, IPV6_V6ONLY, &val, &len))
    {
      close(sock);
      throw std::logic_error("Fatal error: IPV6_V6ONLY must be false.");
    }
  }
} // namespace misc
