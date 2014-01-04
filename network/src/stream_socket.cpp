// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/network/stream_socket.hpp"
#include "meridian/network/exception.hpp"

namespace meridian {
namespace network {

stream_socket::stream_socket(socket_domain domain, int protocol)
    : socket{ ::socket(socket_domain_to_af(domain), SOCK_STREAM, protocol) }
{
}


std::unique_ptr<stream_socket> stream_socket::accept(socket_address & address)
{
    sockaddr_storage addr;
    socklen_t addr_length = sizeof(addr);

    int accept_fd = ::accept(fd(), reinterpret_cast<sockaddr *>(&addr), &addr_length);
    if (accept_fd < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("accept");
    }
    
    address = socket_address(reinterpret_cast<sockaddr *>(&addr), addr_length);
    
    return std::unique_ptr<stream_socket>(new stream_socket(accept_fd));
}

} // namespace network
} // namespace meridian
