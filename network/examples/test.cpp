// Copyright Eric Crampton, 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "meridian/reactor/select_reactor.hpp"
#include "meridian/network/stream_socket.hpp"
#include "meridian/network/ip_address_family.hpp"

#include <functional>

class echo_server {
public:
    echo_server(in_port_t listen_port)
        : accept_socket_(meridian::network::socket_domain::inet)
        , reactor_()
    {
        accept_socket_.set_reuse_address(true);

        accept_socket_.bind(
                meridian::network::socket_address::create_inet_address(
                        meridian::network::ip_address(),
                        listen_port));

        accept_socket_.listen(5);
    }

    void run() {
        meridian::reactor::select_reactor::scoped_registration<meridian::reactor::event_type::read> registration(
                reactor_,
                accept_socket_,
                std::bind(&echo_server::on_connection, this));
        
        while (true) {
            reactor_.wait_for_events();
        }
    }
    
private:
    void on_connection()
    {
        meridian::network::socket_address client_address;
        std::unique_ptr<meridian::network::stream_socket> client = accept_socket_.accept(client_address);
        
        std::cout << "connection from " << client_address << '\n';

        client->send("hello!\n", 7, 0);
        client->close();
    }

private:
    meridian::network::stream_socket accept_socket_;
    meridian::reactor::select_reactor reactor_;
};

int main()
{
    echo_server server(12345);
    server.run();
}
