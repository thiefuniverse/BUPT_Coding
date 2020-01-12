////
//// Created by thief on 17-2-8.
////
//
//#include <iostream>
//#include <boost/array.hpp>
//#include <boost/bind.hpp>
//#include <boost/asio.hpp>
//
//using boost::asio::ip::tcp;
//using namespace std;
//using boost::asio::io_service;
//
////int main(int argc,char* argv[1])
////{
////    try {
////        if (argc!=2)
////        {
////            cerr<<"Usage: client <host> "<<endl;
////            return 1;
////        }
////
////
////        io_service io;
////        tcp::resolver resolver(io);
////        tcp::resolver::query query(argv[1],"daytime");
////
////        tcp::resolver::iterator endpoint_iterator=resolver.resolve(query);
////        tcp::socket socket(io);
////        boost::asio::connect(socket,endpoint_iterator);
////
////        while (true)
////        {
////            boost::array<char,128> buf;
////
////            boost::system::error_code error;
////            size_t len=socket.read_some(boost::asio::buffer(buf),error);
////            if (error==boost::asio::error::eof)
////            {
////                break;
////            }
////            else if(error)
////            {
////                throw boost::system::system_error(error);
////            }
////
////            cout.write(buf.data(),len);
////
////        }
////    }
////    catch (exception& e)
////    {
////        cerr<<e.what()<<endl;
////    }
////
////
////
////}
//
//void clientHandler(const boost::system::error_code& ec)
//{
//    boost::array<char,6> buf{'t','h','i','e','f'};
//   // sock.write_some(buf, (boost::system::error_code &) ec);
//
//    if (ec)
//    {
//        std::cout<<"error \n";
//    } else
//    {
//
//    }
//}
//int main()
//{
//    try {
//        io_service io;
//        boost::system::error_code error;
//        tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 12345);
//        boost::asio::ip::tcp::socket sock(io);
//        sock.async_connect(ep,clientHandler);
//        io.run();
//    }
//    catch(std::exception& e)
//    {
//        std::cout<<e.what();
//    }
//
//
//
////        if (error)
////        {
////            throw boost::system::system_error(error);
////        }
////
////        // while (true)
////        {
////            //  boost::array<char,8> buf{'t','h','i','e','c','h','a','r'};
////            boost::array<char,2> buf{'t','h'};
////
////            // size_t len=sock.read_some(boost::asio::buffer(buf),error);
////            size_t len=4;
////            sock.write_some(boost::asio::buffer(buf,len),error);
////            sock.read_some(boost::asio::buffer(buf),error);
////
////
////            for (int i = 0; i < buf.size(); ++i) {
////                std::cout<<buf.at(i);
////            }
////            if (error==boost::asio::error::eof)
////            {
////                //  break;
////            }
////            else if(error)
////            {
////                throw boost::system::system_error(error);
////            }
////
////
////        }
////
////
////    }
////    catch (std::exception&e)
////    {
////        std::cout<<e.what();
////    }
//
//
//
//}



#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
    try
    {
//        if (argc != 2)
//        {
//            std::cerr << "Usage: client <host>" << std::endl;
//            return 1;
//        }

        boost::asio::io_service io_service;

        udp::resolver resolver(io_service);
        //  udp::resolver::query query(udp::v4(), argv[1], "daytime");

        //udp::endpoint receiver_endpoint = *resolver.resolve(query);
        udp::endpoint receiver_endpoint = udp::endpoint(udp::v4(),12345);

        udp::socket socket(io_service);
        socket.open(udp::v4());

        boost::array<char, 1> send_buf  = {{ 0 }};
        socket.send_to(boost::asio::buffer(send_buf), receiver_endpoint);

        boost::array<char, 128> recv_buf;
        udp::endpoint sender_endpoint;
        size_t len = socket.receive_from(
                boost::asio::buffer(recv_buf), sender_endpoint);

        std::cout.write(recv_buf.data(), len);
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}

