// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/reactor/select_reactor.hpp"
#include "meridian/reactor/exception.hpp"

#include <sys/select.h>

namespace meridian {
namespace reactor {

select_reactor::select_reactor()
    : registry_(new core::event_source_registry)
    , maxfd_()
{
    FD_ZERO(&read_set_);
    FD_ZERO(&write_set_);
    FD_ZERO(&except_set_);
}


select_reactor::select_reactor(std::unique_ptr<core::event_source_registry> registry)
    : registry_(registry.release())
    , maxfd_()
{
    FD_ZERO(&read_set_);
    FD_ZERO(&write_set_);
    FD_ZERO(&except_set_);
}


void select_reactor::register_read_callback(core::event_source & source, core::event_source::event_callback callback)
{
    assert(callback);
    registry_->register_read_callback(source, callback);
    cache_fd_read_register(source);

    FD_SET(source.fd(), &read_set_);
}


void select_reactor::register_write_callback(core::event_source & source, core::event_source::event_callback callback)
{
    assert(callback);
    registry_->register_write_callback(source, callback);
    cache_fd_read_register(source);

    FD_SET(source.fd(), &write_set_);
}


void select_reactor::register_except_callback(core::event_source & source, core::event_source::event_callback callback)
{
    assert(callback);
    registry_->register_except_callback(source, callback);
    cache_fd_read_register(source);

    FD_SET(source.fd(), &except_set_);
}


void select_reactor::remove_read_callback(core::event_source & source)
{
    registry_->remove_read_callback(source);
    maxfd_ = boost::optional<int>();

    FD_CLR(source.fd(), &read_set_);
}


void select_reactor::wait_for_events()
{
    if (!maxfd_) {
        recache_max_fd();
    }
    
    fd_set read_set = read_set_;
    fd_set write_set = write_set_;
    fd_set except_set = except_set_;

    timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
        
    int result = ::select(*maxfd_ + 1, &read_set, &write_set, &except_set, &tv);
    if (result < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("select");
    }

    for (int fd = 0; fd <= *maxfd_ && result; ++fd) {
        core::event_source * source = nullptr;

        if (FD_ISSET(fd, &read_set)) {
            source = registry_->find(fd);
            assert(source);
            source->read_callback()();
        }

        if (FD_ISSET(fd, &write_set)) {
            if (!source) {
                source = registry_->find(fd);
                assert(source);
            }
            source->write_callback()();
        }

        if (FD_ISSET(fd, &except_set)) {
            if (!source) {
                source = registry_->find(fd);
                assert(source);
            }
            source->except_callback()();
        }

        if (source) {
            --result;
        }
    }
}

} // namespace reactor
} // namespace meridian
