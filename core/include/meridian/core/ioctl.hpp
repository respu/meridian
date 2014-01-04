// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__core__ioctl__hpp
#define meridian__core__ioctl__hpp

#include "meridian/core/exception.hpp"

#include <sys/ioctl.h>

namespace meridian {
namespace core {

//! \brief Performs an \c ioctl on a device.
//!
//! This wraps \c ioctl (2) providing two advantages:
//!
//!   * automatic conversion into given template type \c T
//!   * throws an exception if the underlying \c ioctl fails
//!
//! \param device - the device
//! \param request - the \c ioctl request
//! \param result - the result of the \c ioctl

template <typename T>
void ioctl(int device, int request, T & result)
{
    result = ioctl<T>(device, request);
}

//! \brief Performs an \c ioctl on a device.
//!
//! Similar to ioctl<T>(int, int, T &), but returns the result of the underlying \c ioctl as a value.

template <typename T>
T ioctl(int device, int request)
{
    T result = T();
    if (::ioctl(device, request, &result) < 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("ioctl");
    }
    return result;
}

} // namespace core
} // namespace meridian

#endif /* meridian__core__ioctl__hpp */
