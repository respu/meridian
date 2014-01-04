// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/core/event_source.hpp"

namespace meridian {
namespace core {

event_source::event_source(int fd)
    : fd_(fd)
{
}

} // namespace core
} // namespace meridian
