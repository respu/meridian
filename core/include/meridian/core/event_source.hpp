// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__core__event_source__hpp
#define meridian__core__event_source__hpp

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>
#include <functional>

namespace meridian {
namespace core {

struct event_source_tag;

//! \brief Intrusive hashtable base hook for event_source.
typedef boost::intrusive::unordered_set_base_hook<boost::intrusive::tag<event_source_tag>>
event_source_base_hook;

//! \brief Represents a file descriptor which can be monitored for events.
//! \class event_source event_source.hpp meridian/core/event_source.hpp
//!
//! The file descriptor can be associated with any kinds of monitorable entity (e.g., sockets, pipes, an inotify (Linux)
//! instance, etc.). The event_source is a object which can be intrusively placed into a hashtable (an
//! event_source_unordered_set) which manages association with the OS file descriptor. An event_source tracks the
//! callback, if any, associated with each of the monitorable events: readable, writable, and exception.
//!
//! Associating a callback with a monitorable event cannot be done directly on the event_source. You must use a reactor,
//! such as the meridian::reactor::select_reactor. Often, a reactor will have work to be done each time an event_source
//! callback is registered, so registration must be done via functions on that reactor.
//!
//! \author Eric Crampton

class event_source : public event_source_base_hook {
protected:
    //! \brief Construction.
    //!
    //! \param fd - the OS file descriptor
    //!
    //! The file descriptor @a may be an invalid file descriptor (e.g., -1). However, it must be reset using
    //! reset_fd(int) before any attempts are made to register a callback.
    
    event_source(int fd);

    //! \brief Reset the OS file descriptor associated with this event_source.
    //!
    //! \param fd - the new OS file descriptor
    //!
    //! This function may not be called if this event_source has any callbacks associated with it.

    inline void reset_fd(int fd);

public:
    //! \brief Callback function type.
    //!
    //! Note that this function takes no parameters and returns @c void. If you would like to have data associated with
    //! a callback at registration time, the recommended method is to use @c std::bind to pass the data. There are
    //! numerous examples distributed with Meridian which show this.
    
    typedef std::function<void ()> event_callback;

    //! \brief Return the callback for the readable event, if any.
    //!
    //! \return the callback, or an empty function if no callback is associated
    
    inline event_callback read_callback() const;

    //! \brief Return the callback for the writable event, if any.
    //!
    //! \return the callback, or an empty function if no callback is associated

    inline event_callback write_callback() const;

    //! \brief Return the callback for the exception event, if any.
    //!
    //! \return the callback, or an empty function if no callback is associated

    inline event_callback except_callback() const;

    //! \brief Returns true if this event_source has any registered callbacks.

    inline bool has_callback() const;

    //! \brief Returns the OS file descriptor for this event_source.
    
    inline int fd() const;

    //! \brief Equality operator for two \ref event_source "event_source"s.
    //!
    //! This is used in the creation of the Boost Intrusive hashtable which holds \ref event_source "event_source"s by
    //! file descriptor.
    
    struct equal {
        inline bool operator()(event_source const & lhs, event_source const & rhs) const;
    };

    //! \brief Hashing function for an event_source.
    //!
    //! This is used in the creation of the Boost Intrusive hashtable which holds \ref event_source "event_source"s by
    //! file descriptor.
    
    struct hash {
        inline size_t operator()(event_source const & source) const;
    };

private:
    friend class event_source_registry;

    //! \brief Sets the readable callback for this event_source.
    //!
    //! \param callback - the callback
    //!
    //! Overwrites any previously set readable callback.
    
    inline void set_read_callback(event_callback callback);

    //! \brief Sets the writable callback for this event_source.
    //!
    //! \param callback - the callback
    //!
    //! Overwrites any previously set writable callback.

    inline void set_write_callback(event_callback callback);

    //! \brief Sets the exception callback for this event_source.
    //!
    //! \param callback - the callback
    //!
    //! Overwrites any previously set exception callback.

    inline void set_except_callback(event_callback callback);

    //! \brief Removes the readable callback for this event_source.
    //!
    //! This is more readable than calling set_read_callback with an empty function.
    
    inline void remove_read_callback();

    //! \brief Removes the writable callback for this event_source.
    //!
    //! This is more readable than calling set_write_callback with an empty function.

    inline void remove_write_callback();

    //! \brief Removes the exception callback for this event_source.
    //!
    //! This is more readable than calling set_except_callback with an empty function.

    inline void remove_except_callback();
    
private:
    int fd_;
    event_callback read_callback_;
    event_callback write_callback_;
    event_callback except_callback_;
};

//! \brief An intrusive hashtable mapping file descriptors to \ref event_source "event_source"s.
typedef boost::intrusive::unordered_set<
    event_source,
    boost::intrusive::base_hook<event_source_base_hook>,
    boost::intrusive::equal<event_source::equal>,
    boost::intrusive::hash<event_source::hash>
    >
event_source_unordered_set;

#include "event_source.ipp"

} // namespace core
} // namespace meridian

#endif /* meridian__core__event_source__hpp */
