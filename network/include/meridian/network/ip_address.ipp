// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

void const * ip_address::addr() const
{
    return &addr_;
}


void * ip_address::addr()
{
    return &addr_;
}


in_addr const * ip_address::addr4() const
{
    return (family() == ip_address_family::IPv4) ? &addr_.addr4 : nullptr;
}


in6_addr const * ip_address::addr6() const
{
    return (family() == ip_address_family::IPv6) ? &addr_.addr6 : nullptr;
}


socklen_t ip_address::length() const
{
    return family_length(family_);

    __builtin_unreachable();
}


constexpr size_t ip_address::max_length()
{
    return sizeof(in_addr) > sizeof(in6_addr)
        ? sizeof(in_addr)
        : sizeof(in6_addr);
}


ip_address_family ip_address::family() const
{
    return family_;
}


std::uint32_t ip_address::scope() const
{
    return scope_;
}


int ip_address::af() const
{
    switch (family_) {
        case ip_address_family::IPv4: return AF_INET;
        case ip_address_family::IPv6: return AF_INET6;
    }

    __builtin_unreachable();
}


void swap(ip_address & lhs, ip_address & rhs)
{
    std::swap(lhs.family_, rhs.family_);
    std::swap(lhs.addr_,   rhs.addr_);
    std::swap(lhs.scope_,  rhs.scope_);
}
