// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef meridian__network__socket__hpp
#define meridian__network__socket__hpp

#include "meridian/core/event_source.hpp"
#include "meridian/network/ip_address.hpp"
#include "meridian/network/socket_address.hpp"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <unistd.h>

namespace meridian {
namespace network {

class socket : public core::event_source {
public:
    typedef boost::posix_time::time_duration time_duration;
    static int const INVALID_SOCKET_FD = -1;

    void close();
    void close_noexcept() noexcept;

    void set_socket_option(int level, int option, int value);
    int get_int_socket_option(int level, int option) const;

    void set_socket_option(int level, int option, unsigned value);
    unsigned get_unsigned_socket_option(int level, int option) const;

    void set_socket_option(int level, int option, unsigned char value);
    unsigned char get_unsigned_char_socket_option(int level, int option) const;

    void set_socket_option(int level, int option, ip_address const & value);
    ip_address get_ip_address_socket_option(int level, int option) const;
    
    void set_socket_option(int level, int option, time_duration const & value);
    boost::posix_time::time_duration get_time_duration_socket_option(int level, int option) const;
    
    void set_raw_socket_option(int level, int option, void const * value, size_t length);
    void get_raw_socket_option(int level, int option, void * value, socklen_t & length) const;
    
    //! \brief Sets the socket's send buffer size (@c SO_SNDBUF).
    //!
    //! \param size - the size, in bytes; this is a signed integer to match the match the system call's use of @c int
    
    void set_send_buffer_size(int size);
    int get_send_buffer_size() const;
    void set_receive_buffer_size(int size);
    int get_receive_buffer_size() const;
    void set_send_timeout(time_duration const & timeout);
    time_duration get_send_timeout() const;

    void set_receive_timeout(time_duration const & timeout);
    time_duration get_receive_timeout() const;

    void set_linger(bool on, int seconds);
    std::pair<bool, int /* seconds */> get_linger() const;

    void set_no_delay(bool flag);
    bool get_no_delay() const;

    void set_keep_alive(bool flag);
    bool get_keep_alive() const;

    void set_reuse_address(bool flag);
    bool get_reuse_address() const;

    void set_reuse_port(bool flag);
    bool get_reuse_port() const;

    void set_oob_inline(bool flag);
    bool get_oob_inline() const;

    void set_broadcast(bool flag);
    bool get_broadcast() const;
    
    //! \brief Returns the number of bytes which can be read without blocking.
    //!
    //! \return number of bytes available; 0 if no data is available

    int available();
    
    socket_address address() const;
    socket_address peer_address() const;
    
protected:
    socket();
    socket(int fd);

    void init(socket_domain domain, int type, int proto = 0);
    
    //! \brief Receive bytes on this socket.
    ssize_t receive(void * buffer, size_t length, int flags);
    ssize_t send(void const * buffer, size_t length, int flags);
    ssize_t receive_from(void * buffer, size_t length, int flags, socket_address & address);
    ssize_t send_to(void const * buffer, size_t length, int flags, socket_address const & address);
    
    void bind(socket_address const & address);
    void listen(int backlog);
    void shutdown_receive();
    void shutdown_send();
    void shutdown();
    void shutdown(int how);
    
    inline int fd();
    
private:
    int fd_;
};

#include "meridian/network/socket.ipp"

} // namespace network
} // namespace meridian

#endif /* meridian__network__socket__hpp */
