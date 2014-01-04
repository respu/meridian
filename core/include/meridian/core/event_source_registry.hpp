// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__core__event_source_registry__hpp
#define meridian__core__event_source_registry__hpp

#include "meridian/core/event_source.hpp"

#include <memory>

namespace meridian {
namespace core {

//! \brief Manages registration of callbacks on \ref event_source "event_source"s by file descriptor.
//! \class event_source_registry event_source_registry.hpp meridian/core/event_source_registry.hpp
//!
//! An event_source_registry isn't meant to be used by end users---it's an implementation detail of a kind of reactor,
//! such as a meridian::reactor::select_reactor.
//!
//! Internally, it's no secret, the registry uses an intrusive hashtable to manage the association between an OS file
//! descriptor and an event_source.
//!
//! \author Eric Crampton

class event_source_registry {
public:
    //! \brief Construction and initialization of the hashtable.
    //!
    //! \param bucket_count - the initial number of hashtable buckets
    //!
    //! Given that the hashtable keys are OS file descriptors, if you know you're going to have a large number of
    //! concurrently open sockets, for example, you can pass a suitably large enough value here to ensure the hashtable
    //! load factor is appropriate.
    
    explicit event_source_registry(size_t bucket_count = 64);

    //! \brief Move construction.
    
    event_source_registry(event_source_registry && registry);

    //! \brief Copy construction is \a not permitted.

    event_source_registry(event_source_registry const & registry) = delete;

    //! \brief Assignment is \a not permitted.

    event_source_registry & operator=(event_source_registry const & registry) = delete;

    //! \brief Registers a readable callback for an event_source.
    //!
    //! \param source - the event_source to monitor
    //! \param callback - callback to be called for readable events
    //!
    //! If a readable callback is already registered for the given \a source, it is overwritten with the new callback.

    void register_read_callback(event_source & source, event_source::event_callback callback);

    //! \brief Registers a writable callback for an event_source.
    //!
    //! \param source - the event_source to monitor
    //! \param callback - callback to be called for writable events
    //!
    //! If a writable callback is already registered for the given \a source, it is overwritten with the new callback.

    void register_write_callback(event_source & source, event_source::event_callback callback);

    //! \brief Registers an exception callback for an event_source.
    //!
    //! \param source - the event_source to monitor
    //! \param callback - callback to be called for exception events
    //!
    //! If an exception callback is already registered for the given \a source, it is overwritten with the new callback.

    void register_except_callback(event_source & source, event_source::event_callback callback);

    //! \brief Removes the readable callback for an event_source, if any.
    //!
    //! \param source - the event_source whose readable callback is to be removed.
    //!
    //! If no callback is associated, this does nothing.
    
    void remove_read_callback(event_source & source);

    //! \brief Removes the writable callback for an event_source, if any.
    //!
    //! \param source - the event_source whose writable callback is to be removed.
    //!
    //! If no callback is associated, this does nothing.

    void remove_write_callback(event_source & source);

    //! \brief Removes the exception callback for an event_source, if any.
    //!
    //! \param source - the event_source whose exception callback is to be removed.
    //!
    //! If no callback is associated, this does nothing.

    void remove_except_callback(event_source & source);

    //! \brief \c iterator over the internal hashtable of an event_source_registry.
    typedef event_source_unordered_set::iterator iterator;

    //! \brief \c const_iterator over the internal hashtable of an event_source_registry.
    typedef event_source_unordered_set::const_iterator const_iterator;

    //! \brief Returns an iterator pointing to the start of the registration hashtable.
    //!
    //! This is meant to be used only by a reactor which requires peaking into the hashtable for a good reason. For
    //! example, select_reactor requires re-determining the maximum file descriptor in some instances so it may be
    //! passed to \c select (2).
    
    inline iterator begin();

    //! \brief Returns a \c const iterator pointing to the start of the registration hashtable.

    inline const_iterator begin() const;

    //! \brief Returns an iterator pointing one past the end of the registration hashtable.
    
    inline iterator end();

    //! \brief Returns an \c const iterator pointing one past the end of the registration hashtable.

    inline const_iterator end() const;

    //! \brief Finds the event_source registered for a file descriptor.
    //!
    //! \param fd - OS file descriptor
    //!
    //! \return the event_source, or \c nullptr if no event_source is associated with \a fd
    
    inline event_source * find(int fd);

    //! \brief Finds the event_source registered for a file descriptor.
    //!
    //! \param fd - OS file descriptor
    //!
    //! \return the event_source, or \c nullptr if no event_source is associated with \a fd
        
    inline event_source const * find(int fd) const;
    
private:

    //! \brief Convenience method to erase an event_source from the hashtable if it no longer has any callbacks.

    inline void erase_source_without_callback(event_source & source);
    
private:
    typedef event_source_unordered_set::bucket_type   bucket_type;
    typedef event_source_unordered_set::bucket_traits bucket_traits;
    
    std::unique_ptr<bucket_type []> buckets_;
    event_source_unordered_set event_sources_;
};

#include "meridian/core/event_source_registry.ipp"

} // namespace meridian
} // namespace core

#endif /* meridian__core__event_source_registry__hpp */
