// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__exception__hpp
#define meridian__network__exception__hpp

#include "meridian/core/exception.hpp"

namespace meridian {
namespace network {

//! \brief Base class for exceptions related to the Meridian Network library.
//! \class exception exception.hpp meridian/network/exception.hpp

class exception : public virtual core::exception { };

//! \brief An unsupported operation was attempted.
//! \class unsupported_operation_exception exception.hpp meridian/network/exception.hpp

class unsupported_operation_exception : public virtual core::unsupported_operation_exception, public virtual exception { };

//! \brief An operation was attempted on a socket which didn't hold a valid socket file descriptor.

class invalid_socket_exception : public virtual exception { };

//! \brief Error information type which holds an address.
//!
//! The address is related to the failed operation, e.g., an address which couldn't be parsed as an IPv4 or IPv6
//! address.

typedef boost::error_info<struct errinfo_address_, std::string> errinfo_address;

//! \brief Error information for a failed call to <tt>getaddrinfo</tt>.
//!
//! The value is a string message returned from <tt>gai_strerror</tt>.

typedef boost::error_info<struct errinfo_getaddrinfo_, std::string> errinfo_getaddrinfo;

} // namespace network
} // namespace meridian

#endif /* meridian__network__exception__hpp */

