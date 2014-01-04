// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

socklen_t socket_address::length() const
{
    return length_;
}


sockaddr * socket_address::addr()
{
    return reinterpret_cast<sockaddr *>(&addr_);
}


sockaddr const * socket_address::addr() const
{
    return reinterpret_cast<sockaddr const *>(&addr_);
}


socket_domain socket_address::domain() const
{
    return socket_domain_from_length(length());
}


sockaddr_in const * socket_address::in_addr_ptr() const
{
    return reinterpret_cast<sockaddr_in const *>(&addr_);
}


sockaddr_in6 const * socket_address::in6_addr_ptr() const
{
    return reinterpret_cast<sockaddr_in6 const *>(&addr_);
}


sockaddr_un const * socket_address::un_addr_ptr() const
{
    return reinterpret_cast<sockaddr_un const *>(&addr_);
}
