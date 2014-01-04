// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

socklen_t family_length(ip_address_family family)
{
    switch (family) {
        case ip_address_family::IPv4: return sizeof(in_addr);
        case ip_address_family::IPv6: return sizeof(in6_addr);
    }

    __builtin_unreachable();
}
