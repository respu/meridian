// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

void event_source::reset_fd(int fd)
{
    assert(!event_source_base_hook::is_linked());
    assert(!has_callback());
    fd_ = fd;
}


event_source::event_callback event_source::read_callback() const
{
    return read_callback_;
}


event_source::event_callback event_source::write_callback() const
{
    return write_callback_;
}


event_source::event_callback event_source::except_callback() const
{
    return except_callback_;
}


bool event_source::has_callback() const
{
    return read_callback_ || write_callback_ || except_callback_;
}


void event_source::set_read_callback(event_callback callback)
{
    read_callback_ = callback;
}


void event_source::set_write_callback(event_callback callback)
{
    write_callback_ = callback;
}


void event_source::set_except_callback(event_callback callback)
{
    except_callback_ = callback;
}


void event_source::remove_read_callback()
{
    read_callback_ = 0;
}


void event_source::remove_write_callback()
{
    write_callback_ = 0;
}


void event_source::remove_except_callback()
{
    except_callback_ = 0;
}


int event_source::fd() const
{
    return fd_;
}


bool event_source::equal::operator()(event_source const & lhs, event_source const & rhs) const
{
    return lhs.fd_ == rhs.fd_;
}


size_t event_source::hash::operator()(event_source const & source) const
{
    return source.fd_;
}
