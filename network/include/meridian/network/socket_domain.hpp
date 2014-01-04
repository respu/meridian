// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__socket_domain__hpp
#define meridian__network__socket_domain__hpp

#include <cstdint>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/un.h>

namespace meridian {
namespace network {

//! \brief A socket domain.
//!
//! This provides strong typing over the different socket domain types (\c AF_INET, \c AF_INET6, and \c AF_UNIX).

enum socket_domain : std::uint8_t {
    inet  = AF_INET,  /*!< corresponds to \c AF_INET  */
    inet6 = AF_INET6, /*!< corresponds to \c AF_INET6 */
    unix  = AF_UNIX   /*!< corresponds to \c AF_UNIX  */
};

//! \brief Determines the socket_domain from the length of a \c sockaddr structure.
//!
//! The four different \c sockaddr structures (\c sockaddr_in, \c sockaddr_in6, and \c sockaddr_un) have differing
//! length. Given only the length of a \c sockaddr structure, this returns a strongly typed socket_domain.

inline socket_domain socket_domain_from_length(socklen_t length);

//! \brief Returns an address family given a socket domain.
//!
//! This serves as a simple way to convert from a strongly typed socket_domain (e.g., \c inet) into a weakly typed
//! integer (e.g., \c AF_INET). Note that this method is most certainly inlined into a no-op.

inline int socket_domain_to_af(socket_domain domain);

#include "meridian/network/socket_domain.ipp"

} // namespace network
} // namespace meridian

#endif /* meridian__network__socket_domain__hpp */
