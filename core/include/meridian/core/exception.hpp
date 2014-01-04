// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__core__exception__hpp
#define meridian__core__exception__hpp

#include <boost/exception/all.hpp>
#include <stdexcept>

namespace meridian {
namespace core {

//! \brief Base class for all Meridian exceptions.

class exception
    : virtual public std::exception
    , virtual public boost::exception
{
};

//! \brief An unsupported operation was attempted.

class unsupported_operation_exception : virtual public exception { };

//! \brief Boost \c error_info which is a message that can be attached to an exception.

typedef boost::error_info<struct tag_exception_message, std::string> exception_message;

} // namespace core
} // namespace meridian

#endif /* meridian__core__exception__hpp */
