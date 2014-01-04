// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/network/ip_address.hpp"
#include "meridian/network/exception.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <functional>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

namespace meridian {
namespace network {

ip_address::ip_address()
    : family_{ ip_address_family::IPv4 }
    , addr_{ }
    , scope_{ 0 }
{
    std::memset(&addr_.addr4, 0, sizeof(addr_.addr4));
}


ip_address::ip_address(std::string const & address)
    : family_{ }
    , addr_{ }
    , scope_{ }
{
    boost::optional<ip_address> parsed = parse_address(address);
    if (parsed) {
        swap(*this, *parsed);
    }
    else {
        throw exception() << errinfo_address(address);
    }
}


ip_address::ip_address(void const * address, socklen_t length, std::uint32_t scope)
    : family_{ ip_address_family::IPv4 }
    , addr_{ }
    , scope_{ scope }
{
    switch (length) {
        case sizeof(in_addr):
            family_ = ip_address_family::IPv4;
            assert(scope == 0);
            break;

        case sizeof(in6_addr):
            family_ = ip_address_family::IPv6;
            break;

        default:
            throw exception() << core::exception_message((boost::format("invalid socklen_t value of %1%") % length).str());
    }
            
    std::memcpy(addr(), address, length);
}


ip_address::ip_address(ip_address const & address)
    : family_{ address.family() }
    , addr_{ }
    , scope_{ address.scope() }
{
    std::memcpy(addr(), address.addr(), length());
}


ip_address::ip_address(ip_address && address) noexcept
    : family_{ address.family() }
    , addr_{ }
    , scope_{ address.scope() }
{
    address.family_ = ip_address_family::IPv4;
    std::memset(&address.addr_.addr4, 0, sizeof(address.addr_.addr4));
    address.scope_ = 0;
}


ip_address::ip_address(ip_address_family family)
    : family_{ family }
    , addr_{ }
    , scope_{ }
{
    std::memset(&addr_, 0, family_length(family_));
}


ip_address & ip_address::operator=(ip_address const & rhs)
{
    family_ = rhs.family();
    std::memcpy(addr(), rhs.addr(), rhs.length());
    scope_ = rhs.scope();

    return *this;
}


bool ip_address::is_wildcard() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return addr4()->s_addr == INADDR_ANY;

        case ip_address_family::IPv6:
        {
            std::uint16_t const * words = reinterpret_cast<std::uint16_t const *>(addr6());
            return words[0] == 0 && words[1] == 0 && words[2] == 0 && words[3] == 0
                && words[4] == 0 && words[5] == 0 && words[6] == 0 && words[7] == 0;
        }
    }

    __builtin_unreachable();
}


bool ip_address::is_broadcast() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return addr4()->s_addr == INADDR_NONE;

        case ip_address_family::IPv6:
            return false;
    }

    __builtin_unreachable();
}


bool ip_address::is_loopback() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return (ntohl(addr4()->s_addr) & 0xFF000000) == 0x7F000000;

        case ip_address_family::IPv6:
            return IN6_IS_ADDR_LOOPBACK(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_multicast() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            // 224.0.0.0/24 to 239.0.0.0/24
            return (ntohl(addr4()->s_addr) & 0xF0000000) == 0xE0000000;

        case ip_address_family::IPv6:
            return IN6_IS_ADDR_MULTICAST(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_unicast() const
{
    return !is_wildcard()
        && !is_broadcast()
        && !is_multicast();
}


bool ip_address::is_link_local() const
{
    switch (family()) {
        case ip_address_family::IPv4:
        {
            std::uint32_t const addr = ntohl(addr4()->s_addr);
            return (addr & 0xFF000000) == 0x0A000000           // 10.0.0.0/24
                || (addr & 0xFFFF0000) == 0xC0A80000           // 192.168.0.0/16
                || (addr >= 0xAC100000 && addr <= 0xAC1FFFFF); // 172.16.0.0 to 172.31.255.255
        }

        case ip_address_family::IPv6:
            return IN6_IS_ADDR_SITELOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_site_local() const
{
    switch (family()) {
        case ip_address_family::IPv4:
        {
            std::uint32_t const addr = ntohl(addr4()->s_addr);
            return (addr & 0xFF000000) == 0x0A000000           // 10.0.0.0/24
                || (addr & 0xFFFF0000) == 0xC0A80000           // 192.168.0.0/16
                || (addr >= 0xAC100000 && addr <= 0xAC1FFFFF); // 172.16.0.0 to 172.31.255.255
        }

        case ip_address_family::IPv6:
            return IN6_IS_ADDR_SITELOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_IPv4_compatible() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return true;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_V4COMPAT(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_IPv4_mapped() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return true;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_V4MAPPED(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_well_known_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return (ntohl(addr4()->s_addr) & 0xFFFFFF00) == 0xE0000000;
            
        case ip_address_family::IPv6:
            std::uint16_t const * words = reinterpret_cast<std::uint16_t const *>(addr6());
            return (ntohs(words[0]) & 0xFFF0) == 0xFF00;
    }

    __builtin_unreachable();
}


bool ip_address::is_node_local_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            return false;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_MC_NODELOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_link_local_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            // 224.0.0.0/24
            return (ntohl(addr4()->s_addr) & 0xFF000000) == 0xE0000000;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_MC_LINKLOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_site_local_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            // 239.255.0.0/16
            return (ntohl(addr4()->s_addr) & 0xFFFF0000) == 0xEFFF0000;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_MC_SITELOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_org_local_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
            // 239.192.0.0/16
            return (ntohl(addr4()->s_addr) & 0xFFFF0000) == 0xEFC00000;
            
        case ip_address_family::IPv6:
            return IN6_IS_ADDR_MC_ORGLOCAL(addr6());
    }

    __builtin_unreachable();
}


bool ip_address::is_global_mc() const
{
    switch (family()) {
        case ip_address_family::IPv4:
        {
            // 224.0.1.0 to 238.255.255.255
            std::uint32_t const addr = ntohl(addr4()->s_addr);
            return addr >= 0xE0000100 && addr <= 0xEE000000;
        }
        
        case ip_address_family::IPv6: 
            return IN6_IS_ADDR_MC_GLOBAL(addr6());
    }

    __builtin_unreachable();
}


ip_address mask(ip_address const & address, ip_address const & the_mask)
{
    ip_address null;
    return mask(address, the_mask, null);
}


ip_address mask(ip_address const & address, ip_address const & the_mask, ip_address const & to_set)
{
    if (address.family()     != ip_address_family::IPv4
        || the_mask.family() != ip_address_family::IPv4
        || to_set.family()   != ip_address_family::IPv4) {
        throw unsupported_operation_exception() << core::exception_message("mask is only supported for IPv4 addresses");
    }

    auto addr = address.addr4()->s_addr;
    auto & mask_addr = the_mask.addr4()->s_addr;
    auto & set_addr = to_set.addr4()->s_addr;

    addr &= mask_addr;
    addr |= set_addr & ~mask_addr;

    return ip_address(&addr, sizeof(addr));
}


std::ostream & operator<<(std::ostream & os, ip_address const & address)
{
    // Unfortunately, per the inet_ntop (3) man page, AF_INET6 converts IPv4-mapped IPv6 addresses into an IPv6
    // format. So, we produce IPv4 addresses "by hand" and defer to inet_ntop for IPv6 addresses.

    switch (address.family()) {
        case ip_address_family::IPv4:
        {
            std::uint8_t const * bytes = reinterpret_cast<std::uint8_t const *>(address.addr());
            os << boost::lexical_cast<std::string>(static_cast<int>(bytes[0])) << '.';
            os << boost::lexical_cast<std::string>(static_cast<int>(bytes[1])) << '.';
            os << boost::lexical_cast<std::string>(static_cast<int>(bytes[2])) << '.';
            os << boost::lexical_cast<std::string>(static_cast<int>(bytes[3]));
        }
        break;

        case ip_address_family::IPv6:
        {
            char buffer[INET6_ADDRSTRLEN];

            if (inet_ntop(
                        address.af(),
                        address.addr(),
                        buffer,
                        sizeof(buffer)) == nullptr) {
                throw exception() << boost::errinfo_api_function("getnameinfo")
                                  << boost::errinfo_errno(errno);
            }

            os << buffer;
        }
        break;
    }

    return os;
}


boost::optional<ip_address> parse_IPv4_address(std::string const & address)
{
    if (address.empty()) {
        return boost::optional<ip_address>();
    }

    in_addr ia;
    return inet_aton(address.c_str(), &ia)
        ? ip_address(&ia, sizeof(ia))
        : boost::optional<ip_address>();
}


boost::optional<ip_address> parse_IPv6_address(std::string const & address)
{
    if (address.empty()) {
        return boost::optional<ip_address>();
    }

    addrinfo * pai;
    addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_flags = AI_NUMERICHOST;

    if (getaddrinfo(address.c_str(), nullptr, &hints, &pai) == 0) {
        return ip_address(
                &reinterpret_cast<sockaddr_in6 *>(pai->ai_addr)->sin6_addr,
                sizeof(in6_addr),
                static_cast<int>(reinterpret_cast<sockaddr_in6 *>(pai->ai_addr)->sin6_scope_id));
    }
    else {
        return boost::optional<ip_address>();
    }
}


boost::optional<ip_address> parse_address(std::string const & address)
{
    boost::optional<ip_address> parsed = parse_IPv4_address(address);
    if (parsed) {
        return parsed;
    }

    return parse_IPv6_address(address);
}


bool operator==(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) == 0
        : false;
}


bool operator!=(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) != 0
        : true;
}


bool operator<(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) < 0
        : lhs.length() < rhs.length();
}


bool operator<=(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) <= 0
        : lhs.length() <= rhs.length();
}


bool operator>(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) > 0
        : lhs.length() > rhs.length();
}


bool operator>=(ip_address const & lhs, ip_address const & rhs)
{
    return lhs.length() == rhs.length()
        ? std::memcmp(lhs.addr(), rhs.addr(), lhs.length()) >= 0
        : lhs.length() >= rhs.length();
}


} // namespace network
} // namespace meridian
