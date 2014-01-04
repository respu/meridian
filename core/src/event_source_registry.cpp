// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/core/event_source_registry.hpp"

namespace meridian {
namespace core {

event_source_registry::event_source_registry(size_t bucket_count)
    : buckets_{ new bucket_type[bucket_count] }
    , event_sources_{ bucket_traits(buckets_.get(), bucket_count) }
{
}


event_source_registry::event_source_registry(event_source_registry && registry)
    : buckets_(std::move(registry.buckets_))
    , event_sources_(std::move(registry.event_sources_))
{
}


void event_source_registry::register_read_callback(event_source & source, event_source::event_callback callback)
{
    if (!source.is_linked()) {
        event_sources_.insert(source);
    }

    source.set_read_callback(callback);
}


void event_source_registry::register_write_callback(event_source & source, event_source::event_callback callback)
{
    if (!source.is_linked()) {
        event_sources_.insert(source);
    }

    source.set_write_callback(callback);
}


void event_source_registry::register_except_callback(event_source & source, event_source::event_callback callback)
{
    if (!source.is_linked()) {
        event_sources_.insert(source);
    }

    source.set_except_callback(callback);
}


void event_source_registry::remove_read_callback(event_source & source)
{
    assert(source.is_linked());
    source.remove_read_callback();
    erase_source_without_callback(source);
}


void event_source_registry::remove_write_callback(event_source & source)
{
    assert(source.is_linked());
    source.remove_write_callback();
    erase_source_without_callback(source);
}


void event_source_registry::remove_except_callback(event_source & source)
{
    assert(source.is_linked());
    source.remove_except_callback();
    erase_source_without_callback(source);
}

} // namespace core
} // namespace meridian
