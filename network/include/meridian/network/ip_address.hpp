// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__ip_address__hpp
#define meridian__network__ip_address__hpp

#include "meridian/network/ip_address_family.hpp"

#include <cassert>
#include <cstdint>
#include <iosfwd>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

#include <boost/exception/all.hpp>
#include <boost/optional.hpp>

namespace meridian {
namespace network {

//! \brief An Internet Protocol (IP) address.
//! \class ip_address ip_address.hpp meridian/network/ip_address.hpp
//!
//! Supports both IPv4 and IPv6 address families. Relational operators (<tt>&lt;</tt>, <tt>&lt;=</tt>, <tt>&gt;</tt>,
//! <tt>&gt;=</tt>) are supported, but you must not interpret any special meaning into the result, other than that the
//! results are consistent. Comparison (<tt>==</tt> and <tt>!=</tt>) operators are supported as well, but an IPv4
//! address is never considered equal to an IPv6 address, even if the IPv6 address is IPv4 compatible and the addresses
//! are the same.
//!
//! Some code for IPv4 address classification was graciously taken from the Poco Project (also licensed under the Boost
//! Software License, Version 1.0).
//!
//! \author Eric Crampton

class ip_address {
public:
    //////////////////////////////////////////////////////////////////////////////////
    //! \name Construction, move construction, assignment
    //////////////////////////////////////////////////////////////////////////////////

    //! \brief Default constructor which creates a wildcard (zero) IPv4 address.
    //!
    //! The wildcard zero IPv4 address is also known as \c 0.0.0.0.
    
    ip_address();

    //! \brief Creates an IP address from the string containing an IP address in presentation format (dotted decimal for
    //!        IPv4, hex string for IPv6).
    //!
    //! \param address - the IP address string

    explicit ip_address(std::string const & address);

    //! \brief Creates an IP address from a raw set of bytes and a length.
    //!
    //! \param address - the address bytes, typically a pointer to an \c in_addr or \c in6_addr structure
    //! \param length - the number of bytes pointed to by \a address, typically \c sizeof(in_addr) or
    //!        \c sizeof(in6_addr)
    //! \param scope - IPv6 scope (must be 0 for IPv4 addresses)

    explicit ip_address(void const * address, socklen_t length, std::uint32_t scope = 0);

    //! \brief Copy constructor.
    //!
    //! \param address - the address to be copied
    
    ip_address(ip_address const & address);

    //! \brief Move constructor.
    //!
    //! \param address - the address to be moved

    ip_address(ip_address && address) noexcept;

    //! \brief Creates a wildcard (zero) IP address for the given address family.
    //!
    //! \param family - the address family
    //!
    //! For IPv4, the wilcard address is \c 0.0.0.0. For IPv6, the wildcard address is <tt>::</tt>.
    
    explicit ip_address(ip_address_family family);

    //! \brief Assignment operator.
    //!
    //! \param rhs - the address to be copied
    //!
    //! \return this ip_address, for chaining
    
    ip_address & operator=(ip_address const & rhs);

    //////////////////////////////////////////////////////////////////////////////////
    //! \name Raw access to IP address fundamentals.
    //////////////////////////////////////////////////////////////////////////////////

    //! \brief Returns a raw address structure.
    //!
    //! The bytes are, in fact, a pointer to an \c in_addr (for IPv4 addresses) or an \c in6_addr (for IPv6
    //! addresses). This can be useful to obtain the bytes for integration into legacy BSD socket API call.s
    //!
    //! \return raw address structure bytes
    
    inline void const * addr() const;

    //! \brief Returns a raw address structure.
    //!
    //! The bytes are, in fact, a pointer to an \c in_addr (for IPv4 addresses) or an \c in6_addr (for IPv6
    //! addresses). This can be useful to obtain the bytes for integration into legacy BSD socket API call.s
    //!
    //! \return raw address structure bytes

    inline void * addr();

    //! \brief Returns a raw IPv4 address structure.
    //!
    //! \return a pointer to an \c in_addr if this IP address is an IPv4 address; otherwise, returns \c nullptr

    inline in_addr const * addr4() const;

    //! \brief Returns a raw IPv6 address structure.
    //!
    //! \return a pointer to an \c in6_addr if this IP address is an IPv6 address; otherwise, returns \c nullptr
    
    inline in6_addr const * addr6() const;

    //! \brief Returns the number of bytes for the raw address structure returned by \a addr.
    //!
    //! \return \c sizeof(in_addr) (for IPv4 addresses); \c sizeof(in6_addr) (for IPv6 addresses)

    inline socklen_t length() const;

    //! \brief Returns the maximum size of an address structure.
    //!
    //! \return This is the larger of \c sizeof(in_addr) and \c sizeof(in6_addr).

    static inline constexpr size_t max_length();
    
    
    //! \brief Returns the address family (IPv4 or IPv6) of this IP address.
    //!
    //! \return \c ip_address_family::IPv4 or \c ip_address_family::IPv6

    inline ip_address_family family() const;

    //! \brief Returns the IPv6 scope.
    //!
    //! \return IPv6 scope; for IPv4 addresses, returns 0.

    inline std::uint32_t scope() const;

    //! Returns the POSIX address family.
    //!
    //! \return \c AF_INET (for IPv4 addresses) or \c AF_INET6 (for IPv6 addresses)
    
    inline int af() const;
    
    friend inline void swap(ip_address & lhs, ip_address & rhs);

    //////////////////////////////////////////////////////////////////////////////////
    //! \name IP address classification functions.
    //////////////////////////////////////////////////////////////////////////////////

    //! \brief Returns true if this address is a wildcard (all zero) address.
    //!
    //! A wildcard address is a special local address which usually means "any address" for bind operations.
    
    bool is_wildcard() const;

    //! \brief Returns true if this address is a broadcast address.
    //!
    //! Only an IPv4 address of all 1s is a broadcast address.

    bool is_broadcast() const;

    //! \brief Returns true if this address is a loopback address.
    //!
    //! IPv4 loopback addresses are in the range of 127.0.0.1 to 127.255.255.255. For IPv6, the loopback address is ::1.

    bool is_loopback() const;

    //! \brief Returns true if this address is a multicast address.
    //!
    //! IPv4 multicast addresses are 224.0.0.0/25 to 239.0.0.0/24. IPv6 multicast addresses have the prefix of ff00::/8.

    bool is_multicast() const;

    //! \brief Returns true if this address is a unicast address.
    //!
    //! A unicast address is an address which is:
    //!
    //!   * not a wildcard address;
    //!   * not a broadcast address; and,
    //!   * not a multicast address.

    bool is_unicast() const;

    //! \brief Returns true if this address is a link local address.
    //!
    //! IPv4 link local addresses are in the range 169.254.0.0/16. IPv6 link local addresses are in the range ff80::/10.

    bool is_link_local() const;
    
    //! \brief Returns true if this address is a site local address.
    //!
    //! IPv4 site local addresses are in one of these ranges:
    //!
    //!   * 10.0.0.0/24
    //!   * 192.168.0.0/16
    //!   * 172.16.0.0 to 172.31.255.255
    //!
    //! IPv6 site local addresses start with one of the following (hexidecimal):
    //!
    //!   * 0xfec
    //!   * 0xfed
    //!   * 0xfee
    //!   * 0xfef

    bool is_site_local() const;

    //! \brief Returns true if this address is IPv4 compatible.
    //!
    //! By definition, all IPv4 addresses are IPv4 compatible. An IPv4 compatible IPv6 address is a special address
    //! assigned to an IPv6-capable device which is a dual stack device speaking both IPv4 and IPv6. The first 96 bits
    //! are zero, with the last 32 bits being an IPv4 address.

    bool is_IPv4_compatible() const;

    //! \brief Returns true if this address is IPv4 mapped.
    //!
    //! An IPv4 mapped address is an IPv4 address which has been mapped onto the IPv6 address space and are used for
    //! devices which are only IPv4 compatible. An IPv4 mapped address is an IPv6 address which starts with 80 bits of
    //! zeroes followed by 16 bits of ones, followed by a 32-bit IPv4 address.

    bool is_IPv4_mapped() const;

    //! \brief Returns true if this is a well-known multicast address.
    //!
    //! IPv4 well-known multicast addresses are in the range 224.0.0.0/8. IPv6 well-known multicast addresses are in the
    //! range FF0x:x:x:x:x:x:x:x.

    bool is_well_known_mc() const;

    //! \brief Returns true if this is a node-local multicast address.
    //!
    //! There is no such thing as a node-local multicast address in IPv4. IPv6 node-local multicast addresses are in the
    //! range FFx1:x:x:x:x:x:x:x.

    bool is_node_local_mc() const;

    //! \brief Returns true if this is a link-local multicast address.
    //!
    //! IPv4 link-local multicast addresses are in the range 224.0.0.0/24 (which overlaps with the well-known multicast
    //! address range). IPv6 link-local multicast addresses are in the range FFx2:x:x:x:x:x:x:x.

    bool is_link_local_mc() const;

    //! \brief Returns true if this is a site-local multicast address.
    //!
    //! IPv4 site-local multicast addresses are in the range 239.255.0.0/16. IPv6 site-local multicast adddresses are in
    //! the range FFx5:x:x:x:x:x:x:x.

    bool is_site_local_mc() const;

    //! \brief Returns true if this is an organization-local multicast address.
    //!
    //! IPv4 organization-local multicast addresses are in the range 239.192.0.0/16. IPv6 organization-local multicast
    //! addresses are in the range FFx8:x:x:x:x:x:x:x.

    bool is_org_local_mc() const;

    //! \brief Returns true if this is a global multicast address.
    //!
    //! IPv4 global multicast addresses are in the range 224.0.1.0 to 238.255.255.255. IPv6 global multicast addresses
    //! are in the range FFxF:x:x:x:x:x:x:x.

    bool is_global_mc() const;
    
private:
    ip_address_family family_;

    union {
        in_addr  addr4;
        in6_addr addr6;
    } addr_;

    std::uint32_t scope_; // IPv6 only
};

//! \name Masking.

//! \brief Masks an IP address using the given netmask.
//
//! The new address is <tt>address &amp; the_mask</tt>.
//!
//! \param address - the address to be masked
//! \param the_mask - the netmask, typically an IPv4 netmask
//! \throws unsupported_operation_exception if a non-IPv4 is passed to the function

ip_address mask(ip_address const & address, ip_address const & the_mask);

//! \brief Masks an IP address using the given netmask.
//!
//! The new address is <tt>(address &amp; the_mask) | (to_set &amp; !the_mask)</tt>.
//!
//! \param address - the address to be masked
//! \param the_mask - the netmask, typically an IPv4 netmask
//! \param to_set - TODO
//! \throws unsupported_operation_exception if a non-IPv4 is passed to the function

ip_address mask(ip_address const & address, ip_address const & the_mask, ip_address const & to_set);

//! \name Input/output.

std::ostream & operator<<(std::ostream & os, ip_address const & address);

//! \name Parsing functions.
//! \relates ip_address
//!
//! These functions try to create an ip_address from a string. The ip_address(std::string const &) constructor will
//! throw if the address cannot be parsed. These functions will simply return an empty optional value instead.

//! \brief Attempts to parse an IPv4 address.
//!
//! \param address - presentation (dotted decimal) format IPv4 address string
//!
//! \return the ip_address if parsed successfully, an empty optional if the address couldn't be parsed

boost::optional<ip_address> parse_IPv4_address(std::string const & address);

//! \brief Attempts to parse an IPv6 address.
//!
//! \param address - presentation (hex string) format IPv6 address string
//!
//! \return the ip_address if parsed successfully, an empty optional if the address couldn't be parsed

boost::optional<ip_address> parse_IPv6_address(std::string const & address);

//! \brief Attempts to parse an IP address.
//!
//! \param address - presentation format (dotted decimal IPv4 address or hex string IPv6 address) address string
//!
//! Tries to parse as an IPv4 address first. If unsuccessful, tries to parse as in IPv6 address.
//!
//! \return the ip_address if parsed successfully, an empty optional if the address couldn't be parsed

boost::optional<ip_address> parse_address(std::string const & address);

//! \name Comparison operators.
//! \relates ip_address

//! \brief Equality operator.
//!
//! Returns true if the addresses strictly match on a bit-by-bit basis. IPv4 addresses and IPv6 addresses are @a never
//! considered equal, even in the case of an equivalently IPv4 mapped IPv6 address.

bool operator==(ip_address const & lhs, ip_address const & rhs);

//! \brief In-equality operator.
//!
//! Returns true if the addresses don't match on a bit-by-bit basis. Note that IPv4 and IPv6 addresses are @a always
//! considered unequal, even in the case of an equivalently IPv4 mapped IPv6 address.

bool operator!=(ip_address const & lhs, ip_address const & rhs);

//! \brief Less than operator.
//!
//! Provides an ordering of addresses. Do not interpret any special meaning into the ordering.

bool operator< (ip_address const & lhs, ip_address const & rhs);

//! \brief Less than or equal operator.
//!
//! Provides an ordering of addresses. Do not interpret any special meaning into the ordering.

bool operator<=(ip_address const & lhs, ip_address const & rhs);

//! \brief Greater than operator.
//!
//! Provides an ordering of addresses. Do not interpret any special meaning into the ordering.

bool operator> (ip_address const & lhs, ip_address const & rhs);

//! \brief Greater than or equal operator.
//!
//! Provides an ordering of addresses. Do not interpret any special meaning into the ordering.

bool operator>=(ip_address const & lhs, ip_address const & rhs);

//! \name Utility functions.

//! \brief Swaps two addresses.
//!
//! Swaps the contents of two ip_address instances.

inline void swap(ip_address & lhs, ip_address & rhs);

#include "ip_address.ipp"

} // namespace network
} // namespace meridian

#endif /* meridian__network__ip_address__hpp */
