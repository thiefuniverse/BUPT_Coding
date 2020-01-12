//
// Created by thief on 17-3-4.
//

#ifndef DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
#define DNS_RELAY_SERVER_DNS_RELAY_SERVER_H

#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <bitset>
#include "DNS_Database.h"

#define  RECE_BUFFER_SIZE 1024
using boost::asio::ip::udp;


class DNS_Data;

class DNS_Relay_Server {
public:
    enum RR_TYPE{CNAME,A,AAAA,PTR,HINFO,MX,NS}; // ...etc.

    DNS_Relay_Server(int port,std::string dnsFile,std::string remoteDNS,int debugClass);
    void start();
    void stop();
    void resovleDNSData(boost::array<char,RECE_BUFFER_SIZE> &receiveBuf,DNS_Data& dnsData);
    void handleDNSRequest(udp::socket& sendSocket,DNS_Data& request,udp::endpoint& remoteEndPoint);

    // construct Resource Records
    std::string rrConstructor(std::string name,int type,int class_in,int ttl,int rlength, std::string address);

private:
    std::string queryDomainName(std::string qr);
    int serverPort;
    boost::asio::io_service ioService;
    DNS_Database dnsDatabase;
    std::string remoteDNS;
    int debugClass;

};


class DNS_Data
{
public:

    DNS_Data();
    DNS_Data(boost::array<char,RECE_BUFFER_SIZE> &dataBuf);
    void resolveData(boost::array<char,RECE_BUFFER_SIZE> &dataBuf);


    std::string getHeaderData();


    std::string getQName();

    void debugPrintDNSData(int debugClass);

    void setANCOUNT(const std::bitset<16> &ANCOUNT);

    void setQR(const std::bitset<1> &QR);

    const std::bitset<16> &getQTYPE() const;

    void setQTYPE(const std::bitset<16> &QTYPE);

    void setQCLASS(const std::bitset<16> &QCLASS);

    void setRD(const std::bitset<1> &RD);

    void setRA(const std::bitset<1> &RA);

    void setRCODE(const std::bitset<4> &RCODE);

private:
    int resolveQuestionSec(boost::array<char,RECE_BUFFER_SIZE> &dataBuf,int currentFlag);
    std::string getFlag();

    std::bitset<16> ID;
    std::bitset<1> QR;
    std::bitset<4> OPCODE;
    std::bitset<1> AA;
    std::bitset<1> TC;
    std::bitset<1> RD;
    std::bitset<1> RA;
    std::bitset<3> Z;
    std::bitset<4> RCODE;
    std::bitset<16> QDCOUNT;
    std::bitset<16> ANCOUNT;
    std::bitset<16> NSCOUNT;
    std::bitset<16> ARCOUNT;

    std::bitset<16> QTYPE;
    std::bitset<16> QCLASS;
    std::string qName;
    std::string questionSection;
    std::string idStr;
    std::string flagStr;
    std::string rcodeStr;
    std::string qdCountStr;
    std::string nsCountStr;
    std::string arCountStr;

    int dataSize;


};



#endif //DNS_RELAY_SERVER_DNS_RELAY_SERVER_H
