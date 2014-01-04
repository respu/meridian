// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__reactor__select_reactor__hpp
#define meridian__reactor__select_reactor__hpp

#include "meridian/core/event_source_registry.hpp"
#include "meridian/reactor/scoped_registration.hpp"

#include <boost/optional.hpp>
#include <memory>
#include <sys/select.h>

namespace meridian {
namespace reactor {

class select_reactor {
public:
    select_reactor();
    select_reactor(std::unique_ptr<core::event_source_registry> registry);

    template <event_type EVENT_TYPE>
    using scoped_registration = reactor::scoped_registration<select_reactor, EVENT_TYPE>;
    
    void register_read_callback(core::event_source & source, core::event_source::event_callback callback);
    void register_write_callback(core::event_source & source, core::event_source::event_callback callback);
    void register_except_callback(core::event_source & source, core::event_source::event_callback callback);

    void remove_read_callback(core::event_source & source);
    void remove_write_callback(core::event_source & source);
    void remove_except_callback(core::event_source & source);

    void wait_for_events();
    
private:
    inline void cache_fd_read_register(core::event_source & source) {
        if (!maxfd_ || source.fd() > *maxfd_) {
            maxfd_ = source.fd();
        }
    }
    
    inline void recache_max_fd() {
        maxfd_ = -1;
        
        for (auto bucket : *registry_) {
            maxfd_ = std::max(bucket.fd(), *maxfd_);
        }
    }
    
private:
    std::unique_ptr<core::event_source_registry> registry_;
    boost::optional<int> maxfd_;

    fd_set read_set_;
    fd_set write_set_;
    fd_set except_set_;
};

} // namespace reactor
} // namespace meridian

#endif /* meridian__reactor__select_reactor__hpp */
