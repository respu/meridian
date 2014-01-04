// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__stream_socket__hpp
#define meridian__network__stream_socket__hpp

#include "meridian/network/socket.hpp"
#include "meridian/network/socket_domain.hpp"

#include <memory>

namespace meridian {
namespace network {

//! \brief A stream socket providing a reliable, bidirectional, byte-oriented communication channel.
//! \class stream_socket stream_socket.hpp meridian/network/stream_socket.hpp
//! 
class stream_socket : public socket {
public:
    using socket::receive;
    using socket::bind;
    using socket::listen;
    using socket::send;

    explicit stream_socket(socket_domain domain, int protocol = 0);
    explicit stream_socket(int fd) : socket(fd) { }
    std::unique_ptr<stream_socket> accept(socket_address & address);
};

} // namespace network
} // namespace meridian

#endif /* meridian__network__stream_socket__hpp */

         
