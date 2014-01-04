// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

socket_domain socket_domain_from_length(socklen_t length)
{
    static_assert(sizeof(sockaddr_in) != sizeof(sockaddr_in6), "sockaddr_in and sockaddr_in6 sizes must be different");
    static_assert(sizeof(sockaddr_in) != sizeof(sockaddr_un),  "sockaddr_in and sockaddr_un sizes must be different");

    switch (length) {
        case sizeof(sockaddr_in):
            return socket_domain::inet;

        case sizeof(sockaddr_in6):
            return socket_domain::inet6;

        case sizeof(sockaddr_un):
            return socket_domain::unix;
    }

    __builtin_unreachable();
}


int socket_domain_to_af(socket_domain domain)
{
    return domain;
}
