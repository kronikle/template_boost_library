// https://www.boost.org/doc/libs/1_71_0/doc/html/boost_asio/tutorial/tutdaytime6.html
//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2019 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>


using boost::asio::ip::udp;

std::string make_daytime_string() {
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

class udp_server {
public:

    udp_server(boost::asio::io_context& io_context, int _port)
    : socket_(io_context, udp::endpoint(udp::v4(), _port)) {
        start_receive();
    }

private:

    void start_receive() {
        socket_.async_receive_from(
                boost::asio::buffer(recv_buffer_), remote_endpoint_,
                boost::bind(&udp_server::handle_receive, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
            std::size_t /*bytes_transferred*/) {
        if (!error) {
            boost::shared_ptr<std::string> message(
                    new std::string(make_daytime_string()));

            socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
                    boost::bind(&udp_server::handle_send, this, message,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

            start_receive();
        }
    }

    void handle_send(boost::shared_ptr<std::string> /*message*/,
            const boost::system::error_code& /*error*/,
            std::size_t /*bytes_transferred*/) {
    }

    udp::socket socket_;
    udp::endpoint remote_endpoint_;
    boost::array<char, 1> recv_buffer_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        udp_server server(io_context, 2002);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}