// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__ip_address_family__hpp
#define meridian__network__ip_address_family__hpp

#include <cstdint>
#include <iosfwd>
#include <netinet/in.h>
#include <unistd.h>

namespace meridian {
namespace network {

//! \name IP address family enumeration.

//! \brief An Internet Protocol (IP) address family.
//! \enum ip_address_family ip_address_family.hpp meridian/network/ip_address_family.hpp
//!
//! Denotes an IPv4 or IPv6 address family.

enum ip_address_family : std::uint8_t {
    IPv4, /*!< The IPv4 address family */
    IPv6, /*!< The IPv6 address family */
};

inline socklen_t family_length(ip_address_family family);

//! \brief Output stream operator for ip_address_family.
//!
//! Writes either "IPv4" or "IPv6", as appropriate.

std::ostream & operator<<(std::ostream & os, ip_address_family family);

//! \brief Input stream operator for ip_address_family.
//!
//! Parses either "IPv4" or "IPv6".

std::istream & operator>>(std::istream & is, ip_address_family & family);

#include "meridian/network/ip_address_family.ipp"

} // namespace network
} // namespace meridian

#endif /* meridian__network__ip_address_family__hpp */
