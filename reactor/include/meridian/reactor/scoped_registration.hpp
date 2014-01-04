// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__reactor__scoped_registration__hpp
#define meridian__reactor__scoped_registration__hpp

namespace meridian {
namespace reactor{

enum event_type : uint8_t {
    read,
    write,
    except
};


template <typename REACTOR_TYPE, event_type EVENT_TYPE>
class scoped_registration {
public:
    scoped_registration(
            REACTOR_TYPE & reactor,
            core::event_source & source,
            core::event_source::event_callback callback);

    void remove();
    
    ~scoped_registration();

private:
    REACTOR_TYPE & reactor_;
    core::event_source * source_;
};

#include "meridian/reactor/scoped_registration.ipp"

} // namespace reactor
} // namespace meridian

#endif /* meridian__reactor__scoped_registration__hpp */
