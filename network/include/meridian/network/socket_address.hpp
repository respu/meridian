// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__socket_address__hpp
#define meridian__network__socket_address__hpp

#include "meridian/network/socket_domain.hpp"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

namespace meridian {
namespace network {

class ip_address;

//! \brief A socket address.
//! \class socket_address socket_address.hpp meridian/network/socket_address.hpp
//!
//! Encapsulates the concept of a socket domain (e.g., @c AF_INET) and related data (e.g., an IPv4 address and
//! port). This class can hold any kind of socket address. Depending upon the socket domain held, certain methods may or
//! may not be called.

class socket_address {
public:
    //////////////////////////////////////////////////////////////////////////////////
    //! \name Construction, move construction, assignment
    //////////////////////////////////////////////////////////////////////////////////

    socket_address() { }
    
    //! \brief Creates a socket address from a @c sockaddr pointer and length.
    //!
    //! \param address - a pointer to a raw @c sockaddr
    //! \param length - length of the data pointed to by @a address
    //!
    //! The socket domain is automatically determined by the length of @c address.
    
    socket_address(sockaddr * address, socklen_t length);

    //! \brief Copy construction.
    //!
    //! \param address - address to be copied
    
    socket_address(socket_address const & address);

    //! \brief Move construction.
    //!
    //! \param address - address to be "moved"

    socket_address(socket_address && address);

    //! \brief Assignment operator.
    //!
    //! \param address - address to be copied

    socket_address & operator=(socket_address const & address);

    //////////////////////////////////////////////////////////////////////////////////
    //! \name Static constructors
    //////////////////////////////////////////////////////////////////////////////////
    
    //! \brief Constructs an Internet (\c AF_INET for IPv4 or \c AF_INET6 for IPv6) socket address.
    //!
    //! \param host - the IP address for the socket_address
    //! \param port - the port for the socket_address
    //!
    //! The family (IPv4 or IPv6) will be chosen automatically based on the IP address.
    //!
    //! \return a newly constructed socket_address
    
    static socket_address create_inet_address(ip_address const & host, in_port_t port);

    //! \brief Constructs a local domain (\c AF_UNIX) socket address from a string.
    //!
    //! \param path - local domain socket path
    //!
    //! \return a newly constructed socket_address

    static socket_address create_unix_address(std::string const & path);

    //! \brief Constructs a local domain (\c AF_UNIX) socket address from a character array.
    //!
    //! \param path - local domain socket path
    //! \param path_length - length of the string in \a path
    //!
    //! \return a newly constructed socket_address

    static socket_address create_unix_address(char const * path, size_t path_length);

    //////////////////////////////////////////////////////////////////////////////////
    //! \name Raw access to socket address fundamentals
    //////////////////////////////////////////////////////////////////////////////////
    
    //! \brief Returns the length of the underlying socket address.
    //!
    //! This is the length of the data pointed to by the pointer returned from addr(). Practically speaking, this is the
    //! length of an \c sockaddr_in, \c sockaddr_in6, or \c sockaddr_un.
    
    inline socklen_t length() const;

    //! \brief Returns a pointer to the underlying socket address structure.
    //!
    //! In reality, this is a pointer to a \c sockaddr_in, \c sockaddr_in6, or \c sockaddr_un, as appropriate.

    inline sockaddr * addr();

    //! \brief Returns a \c const pointer to the underlying socket address structure.
    //!
    //! In reality, this is a pointer to a \c sockaddr_in, \c sockaddr_in6, or \c sockaddr_un, as appropriate.

    inline sockaddr const * addr() const;

    //! \brief Returns the domain (\c inet, \c inet6, or \c unix) of this socket_address.

    //////////////////////////////////////////////////////////////////////////////////
    //! \name Accessors
    //////////////////////////////////////////////////////////////////////////////////
    
    inline socket_domain domain() const;
    
    //! \brief Returns this socket_address's host.
    //!
    //! This is only valid for \ref socket_address "socket_address"es in the \c inet (AF_INET) or \c inet6 (AF_INET6)
    //! domains. If called for a \c unix (AF_UNIX) address, an unsupported_operation_exception is thrown.
    
    ip_address host() const; // AF_INET, AF_INET6 only

    //! \brief Returns this socket_address's port.
    //!
    //! This is only valid for \ref socket_address "socket_address"es in the \c inet (AF_INET) or \c inet6 (AF_INET6)
    //! domains. If called for a \c unix (AF_UNIX) address, an unsupported_operation_exception is thrown.

    in_port_t port() const;

    //! \brief Returns this socket_address's local domain path (as a character array).
    //!
    //! This is only valid for \ref socket_address "socket_address"es in the \c unix (AF_UNIX) domain. If called for an
    //! \c inet (AF_INET) or \c inet6 (AF_INET6) address, an unsupported_operation_exception is thrown.

    char const * path_ptr() const;

    //! \brief Returns this socket_address's local domain path (as a string).
    //!
    //! This is only valid for \ref socket_address "socket_address"es in the \c unix (AF_UNIX) domain. If called for an
    //! \c inet (AF_INET) or \c inet6 (AF_INET6) address, an unsupported_operation_exception is thrown.

    std::string path() const;
        
private:
    inline sockaddr_in  const * in_addr_ptr() const;
    inline sockaddr_in6 const * in6_addr_ptr() const;
    inline sockaddr_un  const * un_addr_ptr() const;

    sockaddr_storage addr_;
    socklen_t length_;
};

//! \name Input/output.
//! \relates socket_address
//! (Non-member functions.)

//! \brief Writes a socket_address to a stream.
//!
//! \param os - stream to write to
//! \param address - address to be written
//!
//! Writes an address using \c getnameinfo (2) (e.g., 127.0.0.1:22).
//!
//! \return the stream, for chaining

std::ostream & operator<<(std::ostream & os, socket_address const & address);

#include "meridian/network/socket_address.ipp"

} // namespace network
} // namespace meridian

#endif /* meridian__network__socket_address__hpp */
