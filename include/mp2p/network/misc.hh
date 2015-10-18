#pragma once

#include <mp2p/network/blocks.hh>

#include <boost/asio.hpp>
#include <string>

namespace network
{
  boost::asio::ip::tcp::resolver::iterator
      resolve_host(const std::string& host, std::string port = "");
  boost::asio::ip::tcp::resolver::iterator
      resolve_host(const std::string& host, uint16_t port);

  boost::asio::ip::tcp::endpoint endpoint_from_host(const std::string& host,
                                                    const std::string port = "");
  boost::asio::ip::tcp::endpoint endpoint_from_host(const std::string& host,
                                                    uint16_t port);

  boost::asio::ip::address one_ip_from_host(const std::string& host,
                                            const std::string port = "");
  boost::asio::ip::address one_ip_from_host(const std::string& host,
                                            uint16_t port);

  boost::asio::ip::address_v6 get_ipv6(const std::string& str);
  boost::asio::ip::address_v6 get_ipv6(const masks::CharT* ch, size_t size);

  std::string binary_to_string_ipv6(const masks::CharT* ch, size_t size);

  network::masks::ADDR get_addr(const std::string& str, masks::port_type port);

  network::masks::partsize_type get_part_size(masks::fsize_type fsize,
                                              masks::partnum_type partnum,
                                              masks::partnum_type parts);
}

#include <mp2p/network/misc.hxx>
