// https://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tutdaytime1

#include<iostream>
#include<boost/array.hpp>
#include<boost/asio.hpp>

using namespace std;
using boost::asio::ip::tcp;

/*
 * 
 */
int main(int argc, char** argv) {
    try {
        if (argc != 3) {
            std::cerr << "Usage: client <host> <port>" << std::endl;
            return 1;
        }
        
        // connection versione tutorial ufficiale
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        // tcp::resolver::query query(argv[1], "daytime");
        tcp::resolver::query query(argv[1], argv[2]); // <host> <port> i.e. ("127.0.0.1", "2002")
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

        tcp::socket socket(io_service);
        boost::asio::connect(socket, endpoint_iterator);
        // [end] connection versione tutorial
         
        /*
         * // altra versione forse più comrensibile con messaggio di richesta
        boost::asio::io_service io_service;
        //socket creation
        tcp::socket socket(io_service);
        //connection
        socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2002));  
       
        //request/message from client
        const string msg = "Hello from Client!\n";
        boost::system::error_code error;
        boost::asio::write(socket, boost::asio::buffer(msg), error);
         * 
         */

        for (;;) {
            boost::array<char, 128> buf;
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buf), error);

            if (error == boost::asio::error::eof)
                break; // Connection closed cleanly by peer.
            else if (error)
                throw boost::system::system_error(error); // Some other error.

            std::cout.write(buf.data(), len);
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

