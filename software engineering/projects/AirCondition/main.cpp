#include <iostream>
#include "AirMaster.h"
#include <boost/asio.hpp>
#include <boost/array.hpp>
using boostTcp=boost::asio::ip::tcp;
using boostAddress=boost::asio::ip::address;

int main() {
    std::cout << "Hello, World!" << std::endl;
    int serverPort=52000;
    boost::asio::io_service service;

    boostTcp ::socket socket(service);
    boostTcp ::endpoint ep(boostAddress::from_string("127.0.0.1"),52000);
    boostTcp ::acceptor acceptor(service, boostTcp::endpoint(boostTcp::v4(),52000));
    boost::array<char,1024> buff;
    boostTcp::socket sock(service);
    acceptor.accept(sock);
    int bytes=sock.read_some(boost::asio::buffer(buff));

   // std::string msg(buff, bytes);
    std::string a;
    for (int i = 0; i < buff.size(); ++i) {
        std::cout<<buff[i]<<"";
        a+=buff[i];
    }
    a=a.substr(0,a.size()-2);
    json m=json::parse(a);
    std::cout<<"\nreceive:   "<<m<<std::endl;
    std::cout<<std::endl<<bytes;
    sock.close();

    return 0;
}