// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__socket_option__hpp
#define meridian__network__socket_option__hpp

#include <sys/types.h>
#include <sys/socket.h>

namespace meridian {
namespace network {

template <typename T>
void set_socket_option(int fd, int level, int option, T value)
{
    if (::setsockopt(fd_, level, option, &value, sizeof(T)) < 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("setsockopt");
    }
}


template <>
void set_socket_option(int fd, int level, int option, ip_address const & value)
{
    if (::setsockopt(fd_, level, option, value.addr(), value.length()) < 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("setsockopt");
    }
}

} // namespace network
} // namespace meridian

#endif /* meridian__network__socket_option__hpp */
