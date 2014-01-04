// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/network/ip_address_family.hpp"

#include <iostream>

namespace meridian {
namespace network {

std::ostream & operator<<(std::ostream & os, ip_address_family family)
{
    os << (family == ip_address_family::IPv4 ? "IPv4" : "IPv6");
    return os;
}


std::istream & operator>>(std::istream & is, ip_address_family & family)
{
    std::string temp;
    is >> temp;

    if (temp == "IPv4") {
        family = ip_address_family::IPv4;
    }
    else if (temp == "IPv6") {
        family = ip_address_family::IPv6;
    }
    else {
        is.setstate(std::ios_base::failbit);
    }

    return is;
}

} // namespace network
} // namespace meridian
