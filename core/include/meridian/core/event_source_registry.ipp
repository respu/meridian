// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

event_source_registry::iterator event_source_registry::begin()
{
    return event_sources_.begin();
}


event_source_registry::const_iterator event_source_registry::begin() const
{
    return event_sources_.begin();
}


event_source_registry::iterator event_source_registry::end()
{
    return event_sources_.end();
}


event_source_registry::const_iterator event_source_registry::end() const
{
    return event_sources_.end();
}


event_source * event_source_registry::find(int fd)
{
    auto it = event_sources_.find(fd);
    return it != event_sources_.end() ? &*it : nullptr;
}


event_source const * event_source_registry::find(int fd) const
{
    auto it = event_sources_.find(fd);
    return it != event_sources_.end() ? &*it : nullptr;
}


void event_source_registry::erase_source_without_callback(event_source & source)
{
    if (!source.has_callback()) {
        event_sources_.erase(source);
    }
}
