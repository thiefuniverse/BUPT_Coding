//#include <iostream>
//#include <ctime>
//#include <string>
//#include <boost/array.hpp>
//#include <boost/thread.hpp>
//#include <boost/asio.hpp>
//
////using boost::asio::ip::tcp;
//using boost::asio::io_service;
////using boost::asio::ip;
//
//std::string make_daytime_string()
//{
//    using namespace std;
//    time_t now=time(0);
//    return ctime(&now);
//}
//
//
////int main()
////{
////    try {
////        io_service io;
////
////        tcp::acceptor acceptor(io,tcp::endpoint(tcp::v4(),12345));
////
////        while (true)
////        {
////            tcp::socket socket(io);
////            acceptor.accept(socket);
////            std::string message=make_daytime_string();
////            boost::system::error_code ignored;
////            boost::asio::write(socket,boost::asio::buffer(message),ignored);
////        }
////
////    }
////    catch (std::exception& e)
////    {
////        std::cerr<<e.what()<<std::endl;
////
////    }
////    return  0;
////
////
////}
//typedef boost::shared_ptr<boost::asio::ip::tcp::socket> socket_ptr;
//
//
//void handle_accept(socket_ptr& sock,const boost::system::error_code& ec,
//                   io_service& io,boost::asio::ip::tcp::acceptor& acc
//)
//{
//    if (ec)
//    {
//        return;
//    } else
//    {
//        socket_ptr sock(new boost::asio::ip::tcp::socket(io));
//        acc.async_accept(*sock,boost::bind(handle_accept,sock,ec,io,acc));
//
//    }
//}
//void client_session(socket_ptr sock)
//{
//
//    boost::system::error_code error;
//    while (true)
//    {
//        boost::array<char, 6> data;
//
//        size_t len=sock->read_some(boost::asio::buffer(data),error);
//        if(error==boost::asio::error::eof)
//        {
//            break;
//        }
//        data.at(0)='g';    acc.async_accept(*sock,boost::bind(handle_accept,sock,error));
//
//        if (len>0)
//        {
//            std::cout<<len<<std::endl;
//          //  sock->write_some(boost::asio::buffer(data));
//            boost::asio::write(*sock,boost::asio::buffer("ok",2));
//        }
//    else
//        {
//            std::cout<<"no data\n";
//        }
//  }
//}
//int main()
//{
//
//    io_service io;
//    boost::system::error_code error;
//    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(),12345);
//    boost::asio::ip::tcp::acceptor acc(io,ep);
//    socket_ptr sock(new boost::asio::ip::tcp::socket(io));
//    acc.async_accept(*sock,boost::bind(handle_accept,sock,error));
//
//    boost::thread(boost::bind(client_session,sock));
//
//
//
//}
//


//
// server.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2016 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <ctime>
#include <iostream>
#include <string>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

std::string make_daytime_string()
{
    using namespace std; // For time_t, time and ctime;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try
    {
        boost::asio::io_service io_service;

        udp::socket socket(io_service, udp::endpoint(udp::v4(), 12345));

        for (;;)
        {
            boost::array<char, 1> recv_buf;
            udp::endpoint remote_endpoint;
            boost::system::error_code error;
            socket.receive_from(boost::asio::buffer(recv_buf),
                                remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);

            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message),
                           remote_endpoint, 0, ignored_error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}