// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

template <typename REACTOR_TYPE, event_type EVENT_TYPE>
scoped_registration<REACTOR_TYPE, EVENT_TYPE>::scoped_registration(
        REACTOR_TYPE & reactor,
        core::event_source & source,
        core::event_source::event_callback callback)
    : reactor_(reactor)
    , source_(&source)
{
    switch (EVENT_TYPE) {
        case event_type::read:
            reactor_.register_read_callback(*source_, callback);
            break;

        case event_type::write:
            reactor_.register_write_callback(*source_, callback);
            break;

        case event_type::except:
            reactor_.register_except_callback(*source_, callback);
            break;
    }
}


template <typename REACTOR_TYPE, event_type EVENT_TYPE>
void scoped_registration<REACTOR_TYPE, EVENT_TYPE>::remove()
{
    reactor_.remove_read_callback(source_);
    source_ = nullptr;
}


template <typename REACTOR_TYPE, event_type EVENT_TYPE>
scoped_registration<REACTOR_TYPE, EVENT_TYPE>::~scoped_registration()
{
    if (!source_) {
        return;
    }
        
    switch (EVENT_TYPE) {
        case event_type::read:
            reactor_.remove_read_callback(*source_);
            break;

        case event_type::write:
            reactor_.remove_write_callback(*source_);
            break;

        case event_type::except:
            reactor_.remove_except_callback(*source_);
            break;
    }
}
