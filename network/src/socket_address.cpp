// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/network/socket_address.hpp"
#include "meridian/network/ip_address.hpp"
#include "meridian/network/exception.hpp"

#include <boost/format.hpp>
#include <cstring>

namespace meridian {
namespace network {

socket_address::socket_address(sockaddr * address, socklen_t length)
    : addr_{ }
    , length_{ length }
{
    std::memcpy(&addr_, address, length_);

    switch (socket_domain_from_length(length)) {
        case socket_domain::inet:
            if (in_addr_ptr()->sin_family != AF_INET) {
                throw exception() << core::exception_message("socket_address constructor called with inconsistent data for AF_INET");
            }
            break;

        case socket_domain::inet6:
            if (in6_addr_ptr()->sin6_family != AF_INET6) {
                throw exception() << core::exception_message("socket_address constructor called with inconsistent data for AF_INET6");
            }
            break;

        case socket_domain::unix:
            if (un_addr_ptr()->sun_family != AF_UNIX) {
                throw exception() << core::exception_message("socket_address constructor called with inconsistent data for AF_UNIX");
            }
            break;
    }
}                


socket_address::socket_address(socket_address const & address)
    : addr_{ }
    , length_{ address.length_ }
{
}


socket_address::socket_address(socket_address && address)
    : addr_(std::move(address.addr_))
    , length_{ address.length_ }
{
    address.length_ = 0;
}


socket_address & socket_address::operator=(socket_address const & address)
{
    addr_ = address.addr_;
    length_ = address.length_;
    return *this;
}


socket_address socket_address::create_inet_address(ip_address const & host, in_port_t port)
{
    sockaddr_storage addr;

    switch (host.family()) {
        case ip_address_family::IPv4:
        {
            sockaddr_in * addr4 = reinterpret_cast<sockaddr_in *>(&addr);
            addr4->sin_family = AF_INET;
            addr4->sin_port   = htons(port);
            addr4->sin_addr   = *host.addr4();

            return socket_address(reinterpret_cast<sockaddr *>(addr4), sizeof(*addr4));
        }
        
        case ip_address_family::IPv6:
        {
            sockaddr_in6 * addr6 = reinterpret_cast<sockaddr_in6 *>(&addr);
            addr6->sin6_family = AF_INET6;
            addr6->sin6_port   = htons(port);
            addr6->sin6_addr   = *host.addr6();

            return socket_address(reinterpret_cast<sockaddr *>(addr6), sizeof(*addr6));
        }
    }

    __builtin_unreachable();
}


socket_address socket_address::create_unix_address(std::string const & path)
{
    return create_unix_address(path.c_str(), path.length());
}


socket_address socket_address::create_unix_address(char const * path, size_t path_length)
{
    sockaddr_un addr;
    addr.sun_family = AF_UNIX;

    size_t const MAX_LENGTH = sizeof(addr.sun_path) - 1;
    if (path_length > MAX_LENGTH) {
        throw unsupported_operation_exception()
            << core::exception_message((boost::format("unix path of length %1% longer than maximum allowed length of %2%")
                                        % path_length
                                        % MAX_LENGTH).str());
    }

    strncpy(addr.sun_path, path, MAX_LENGTH);
    
    return socket_address(reinterpret_cast<sockaddr *>(&addr), sizeof(addr));
}


ip_address socket_address::host() const
{
    void const * addr_ptr = nullptr;
    size_t addr_length = 0;
    
    switch (socket_domain_from_length(length())) {
        case socket_domain::inet:
            addr_ptr = in_addr_ptr();
            addr_length = sizeof(*in_addr_ptr());
            break;

        case socket_domain::inet6:
            addr_ptr = in6_addr_ptr();
            addr_length = sizeof(*in6_addr_ptr());
            break;

        case socket_domain::unix:
            throw unsupported_operation_exception()
                << core::exception_message("cannot call socket_address::host() with socket_domain::unix");
    }

    return ip_address(addr_ptr, addr_length);
}


in_port_t socket_address::port() const
{
    switch (socket_domain_from_length(length())) {
        case socket_domain::inet:
            return ntohs(in_addr_ptr()->sin_port);

        case socket_domain::inet6:
            return ntohs(in6_addr_ptr()->sin6_port);

        case socket_domain::unix:
            throw unsupported_operation_exception()
                << core::exception_message("cannot call socket_address::port() with socket_domain::unix");
    }

    __builtin_unreachable();
}


char const * socket_address::path_ptr() const
{
    switch (socket_domain_from_length(length())) {
        case socket_domain::inet:
        case socket_domain::inet6:
            throw unsupported_operation_exception()
                << core::exception_message("cannot call socket_address::path_ptr() with socket_domain::inet or inet6");

        case socket_domain::unix:
            return un_addr_ptr()->sun_path;
    }

    __builtin_unreachable();
}


std::string socket_address::path() const
{
    return path_ptr();
}


std::ostream & operator<<(std::ostream & os, socket_address const & address)
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    int result = getnameinfo(
            address.addr(), address.length(),
            host, sizeof(host),
            service, sizeof(service),
            NI_NUMERICHOST | NI_NUMERICSERV);

    if (result != 0) {
        throw exception()
            << core::exception_message(gai_strerror(result))
            << boost::errinfo_api_function("getnameinfo");
    }

    os << host << ":" << service;
    
    return os;
}

} // namespace network
} // namespace meridian
