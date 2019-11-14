/* 
 * https://www.boost.org/doc/libs/1_63_0/doc/html/boost_asio/tutorial.html#boost_asio.tutorial.tuttimer5
 */

#include<iostream>
#include<boost/asio.hpp>
#include<boost/bind.hpp> // per passare parametri alle handler functions
#include<boost/date_time/posix_time/posix_time.hpp>

using namespace std;

/*
 * Using asio's asynchronous functionality means having a callback function 
 * that will be called when an asynchronous operation completes.
 */

// one shot
void print(const boost::system::error_code& /*e*/) {
    cout << "Hello, world!" << endl;
}

// repeat
void print_repeat(const boost::system::error_code& /*e*/,
        boost::asio::deadline_timer* t, int* count) {
    if (*count < 5) {
        std::cout << *count << std::endl;
        ++(*count);

        t->expires_at(t->expires_at() + boost::posix_time::seconds(1));
        t->async_wait(boost::bind(print_repeat,
                boost::asio::placeholders::error, t, count));
    }
}

int main() {

    cout << "Please choose sync ('0') or async ('1'): ";

    int sync_selector;
    cin >> sync_selector;
    
    if (sync_selector == 1) {
        cout << "Please choose one-shot ('0') or repeating ('1'): ";
        int next_choice;
        cin >> next_choice;
        sync_selector += next_choice;
    }

    cout << "\nWait 5 seconds please..." << endl;

    boost::asio::io_service io;

    boost::asio::deadline_timer t(io, boost::posix_time::seconds(5));
  
    switch (sync_selector) {
        case 0:
            /// simple timer synchronous
            t.wait();

            std::cout << "Hello, world!" << std::endl;
            ///[end] timer synchronous
            break;

        case 1:
            /// simple timer asynchronous (one shot)
            t.async_wait(&print);

            io.run();
            ///[end] timer asynchronous
            break;

        case 2:
            /// simple timer asynchronous (repeat)
            int count = 0;
            boost::asio::deadline_timer t(io, boost::posix_time::seconds(1));
            t.async_wait(boost::bind(print_repeat,
                    boost::asio::placeholders::error, &t, &count));

            io.run();

            std::cout << "Final count is " << count << std::endl;
            ///[end] timer asynchronous
            break;
    }

    return 0;
}