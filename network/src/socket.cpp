// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/core/ioctl.hpp"
#include "meridian/network/socket.hpp"
#include "meridian/network/exception.hpp"

#include <sys/types.h>
#include <sys/socket.h>

namespace {

using namespace meridian::network;

inline socket_address get_address_impl(
        int fd,
        char const * function_name,
        std::function<int (int, sockaddr *, socklen_t *)> f)
{
    if (fd == socket::INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    sockaddr_storage addr;
    socklen_t addr_length = sizeof(addr);

    if (f(fd, reinterpret_cast<sockaddr *>(&addr), &addr_length) < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function(function_name);
    }

    return socket_address(reinterpret_cast<sockaddr *>(&addr), addr_length);
}

}

namespace meridian {
namespace network {

void socket::close()
{
    if (::close(fd_) != 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("close");
    }
}


void socket::close_noexcept() noexcept
{
    ::close(fd_);
}


void socket::set_socket_option(int level, int option, int value)
{
    set_raw_socket_option(level, option, &value, sizeof(value));
}


int socket::get_int_socket_option(int level, int option) const
{
    int value;
    socklen_t length = sizeof(value);
    get_raw_socket_option(level, option, &value, length);
    assert(length == sizeof(value));

    return value;
}


void socket::set_socket_option(int level, int option, unsigned value)
{
    set_raw_socket_option(level, option, &value, sizeof(value));
}


unsigned socket::get_unsigned_socket_option(int level, int option) const
{
    unsigned value;
    socklen_t length = sizeof(value);
    get_raw_socket_option(level, option, &value, length);
    assert(length == sizeof(value));

    return value;
}


void socket::set_socket_option(int level, int option, unsigned char value)
{
    set_raw_socket_option(level, option, &value, sizeof(value));
}


unsigned char socket::get_unsigned_char_socket_option(int level, int option) const
{
    unsigned char value;
    socklen_t length = sizeof(value);
    get_raw_socket_option(level, option, &value, length);
    assert(length == sizeof(value));

    return value;
}


void socket::set_socket_option(int level, int option, ip_address const & value)
{
    set_raw_socket_option(level, option, value.addr(), value.length());
}


ip_address socket::get_ip_address_socket_option(int level, int option) const
{
    char buffer[ip_address::max_length()];
    socklen_t length = sizeof(buffer);
    get_raw_socket_option(level, option, buffer, length);

    return ip_address(buffer, length);
}


void socket::set_socket_option(int level, int option, time_duration const & value)
{
    timeval tv;
    long usec = value.total_microseconds();

    tv.tv_sec = usec / 1000000;
    tv.tv_usec = usec % 1000000;

    socklen_t length = sizeof(tv);
    set_raw_socket_option(level, option, &tv, length);
}


socket::time_duration socket::get_time_duration_socket_option(int level, int option) const
{
    timeval tv;
    socklen_t length = sizeof(tv);
    get_raw_socket_option(level, option, &tv, length);
    assert(length == sizeof(tv));

    return boost::posix_time::microseconds(tv.tv_sec * 1000000 + tv.tv_usec);
}


void socket::set_raw_socket_option(int level, int option, void const * value, size_t length)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    if (::setsockopt(fd_, level, option, value, length) < 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("setsockopt");
    }
}


void socket::get_raw_socket_option(int level, int option, void * value, socklen_t & length) const
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    if (::getsockopt(fd_, level, option, value, &length) < 0) {
        throw exception() << boost::errinfo_errno(errno) << boost::errinfo_api_function("getsockopt");
    }
}


socket_address socket::address() const
{
    return get_address_impl(fd_, "getsockname", ::getsockname);
}


socket_address socket::peer_address() const
{
    return get_address_impl(fd_, "getpeername", ::getpeername);
}


void socket::set_send_buffer_size(int size)
{
    set_socket_option(SOL_SOCKET, SO_SNDBUF, size);
}


int socket::get_send_buffer_size() const
{
    return get_int_socket_option(SOL_SOCKET, SO_SNDBUF);
}


void socket::set_receive_buffer_size(int size)
{
    set_socket_option(SOL_SOCKET, SO_RCVBUF, size);
}


int socket::get_receive_buffer_size() const
{
    return get_int_socket_option(SOL_SOCKET, SO_RCVBUF);
}


void socket::set_send_timeout(time_duration const & timeout)
{
    set_socket_option(SOL_SOCKET, SO_SNDTIMEO, timeout);
}


socket::time_duration socket::get_send_timeout() const
{
    return get_time_duration_socket_option(SOL_SOCKET, SO_SNDTIMEO);
}


void socket::set_reuse_address(bool flag)
{
    int const value = flag ? 1 : 0;
    set_socket_option(SOL_SOCKET, SO_REUSEADDR, value);
}

int socket::available()
{
    return core::ioctl<int>(fd_, FIONREAD);
}


socket::socket()
    : core::event_source(INVALID_SOCKET_FD)
    , fd_(INVALID_SOCKET_FD)
{
}


socket::socket(int fd)
    : core::event_source(fd)
    , fd_(fd)
{
    if (fd_ < 0) {
        throw exception() << core::exception_message("invalid fd");
    }
}


void socket::init(socket_domain domain, int type, int proto)
{
    assert(fd_ == INVALID_SOCKET_FD);

    if ((fd_ = ::socket(socket_domain_to_af(domain), type, proto)) < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("recv");
    }

#if defined(__MACH__) && defined(__APPLE__) || defined(__FreeBSD__)
    set_socket_option(SOL_SOCKET, SO_NOSIGPIPE, 1);
#endif
}


ssize_t socket::receive(void * buffer, size_t length, int flags)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    ssize_t result = ::recv(fd_, buffer, length, flags);
    if (result < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("recv");
    }
    else {
        return result;
    }
}


ssize_t socket::send(void const * buffer, size_t length, int flags)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    ssize_t result = ::send(fd_, buffer, length, flags);
    if (result < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("send");
    }
    else {
        return result;
    }
}


ssize_t socket::receive_from(void * buffer, size_t length, int flags, socket_address & address)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    sockaddr_storage addr;
    socklen_t addr_length = sizeof(addr);

    ssize_t result = ::recvfrom(fd_, buffer, length, flags, reinterpret_cast<sockaddr *>(&addr), &addr_length);
    if (result < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("send");
    }
    else {
        address = socket_address(reinterpret_cast<sockaddr *>(&addr), addr_length);

        return result;
    }
}


ssize_t socket::send_to(void const * buffer, size_t length, int flags, socket_address const & address)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    ssize_t result = ::sendto(fd_, buffer, length, flags, address.addr(), address.length());
    if (result < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("send");
    }
    else {
        return result;
    }
}


void socket::bind(socket_address const & address)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    if (::bind(fd_, address.addr(), address.length()) < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("bind");
    }
}


void socket::listen(int backlog)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    if (::listen(fd_, backlog) < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("listen");
    }
}


void socket::shutdown_receive()
{
    shutdown(SHUT_RD);
}


void socket::shutdown_send()
{
    shutdown(SHUT_WR);
}


void socket::shutdown()
{
    shutdown(SHUT_RDWR);
}


void socket::shutdown(int how)
{
    if (fd_ == INVALID_SOCKET_FD) {
        throw invalid_socket_exception();
    }

    if (::shutdown(fd_, how) < 0) {
        throw exception()
            << boost::errinfo_errno(errno)
            << boost::errinfo_api_function("shutdown");
    }
}

} // namespace network
} // namespace meridian
