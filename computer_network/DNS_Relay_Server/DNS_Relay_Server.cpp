//
// Created by thief on 17-3-4.
//
#include <iostream>
#include <boost/array.hpp>
#include "DNS_Relay_Server.h"
#include "DNS_utils.h"

#define REFUSED_IP "0.0.0.0"
#define SOCKET_TTL 255

void DNS_Relay_Server::start()
{
    try
    {
        udp::socket socket(ioService, udp::endpoint(udp::v4(), serverPort));

        while(true)
        {
            boost::array<char,RECE_BUFFER_SIZE> recvBuf;
            int bufferSize=0;
            udp::endpoint remoteEndpoint;
            boost::system::error_code error;
            bufferSize=socket.receive_from(boost::asio::buffer(recvBuf),
                                remoteEndpoint, 0, error);

            DNS_Data dnsData;

         //   socket.send_to(recvBuf,remoteEndpoint);
            resovleDNSData(recvBuf,dnsData);
            std::cout<<"receive DNS Data: \n";
            dnsData.debugPrintDNSData(debugClass);

            handleDNSRequest(socket,dnsData,remoteEndpoint);
            std::cout<<"response DNS Data: \n";
            dnsData.debugPrintDNSData(debugClass);
            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}


void DNS_Relay_Server::resovleDNSData(
        boost::array<char, RECE_BUFFER_SIZE> &receiveBuf,
        DNS_Data &dnsData)
{
    dnsData.resolveData(receiveBuf);
}


void DNS_Relay_Server::handleDNSRequest(udp::socket& sendSocket,DNS_Data &request, udp::endpoint &remoteEndPoint) {
    std::string queryName=request.getQName();

    std::string queryResult=queryDomainName(queryName);

    std::string responseStr;
    if(queryResult.empty())
    {
        responseStr+=request.getHeaderData();
        boost::system::error_code error = boost::asio::error::host_not_found;
        boost::asio::ip::address add;
        //add.from_string(REMOTE_DNS_SERVER);
        add.from_string(remoteDNS);
        boost::asio::ip::udp::endpoint endpoint(add, short(serverPort));

       // boost::asio::io_service tcpService;
        //boost::asio::ip::udp::socket serverSocket(tcpService);

      //  serverSocket.connect(endpoint, error);
      //  serverSocket.send(boost::asio::buffer(responseStr));
        sendSocket.send_to(boost::asio::buffer(responseStr),
                           boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(remoteDNS),serverPort));
        boost::array<char,RECE_BUFFER_SIZE> recvBuf;
       // int readSize=serverSocket.receive(boost::asio::buffer(recvBuf));

        int recvSize=sendSocket.receive_from(boost::asio::buffer(recvBuf),endpoint);
        printCharArrayByHex(recvBuf,recvSize);

        DNS_Data receiveData;
        resovleDNSData(recvBuf,receiveData);

        receiveData.debugPrintDNSData(debugClass);
        sendSocket.send_to(boost::asio::buffer(recvBuf),remoteEndPoint);
        return ;
    }
    else
    {
        // set refuse flag
        bool refused=false;
        if(queryResult==REFUSED_IP)
            refused=true;

        request.setQR(1);
        request.setRA(1);
        request.setRD(1);
        request.setANCOUNT(1);
        request.setQCLASS(1);

        std::string resultStr;
        int resultStrSize;
        if(request.getQTYPE().to_ulong()==1&&!refused)
        {
            request.setQTYPE(1);
            resultStr=queryResult;
            resultStrSize=4;

            responseStr+=request.getHeaderData();
            responseStr+=rrConstructor(queryName,request.getQTYPE().to_ulong(),1,SOCKET_TTL,resultStrSize,resultStr);

            std::cout<<">>"<<responseStr<<"  <<";

        } else if(request.getQTYPE().to_ulong()==28&&!refused)
        {
            request.setQTYPE(28);
      //      resultStrSize=queryResult.size()+2;

            request.setRCODE(1);
            request.setANCOUNT(0);
            responseStr+=request.getHeaderData();
            //    responseStr+=rrConstructor(queryName,request.getQTYPE().to_ulong(),1,69,resultStrSize,resultStr);
       //     std::cout<<">>"<<responseStr<<"  <<";
        }
        else
        {
            request.setQTYPE(1);
            //      resultStrSize=queryResult.size()+2;

            request.setRCODE(3);
            request.setANCOUNT(0);
            responseStr+=request.getHeaderData();
        }



        sendSocket.send_to(boost::asio::buffer(responseStr),remoteEndPoint);
    }



}

std::string
DNS_Relay_Server::rrConstructor(std::string name, int type, int class_in, int ttl,int rlength, std::string address) {
    std::stringstream bitHelper;

    bitHelper<<domainNameToBitStr(name);
    std::bitset<16> rType(type);
    std::bitset<16> rClass(class_in);
    std::bitset<32> rTtl(ttl);
    std::bitset<16> rLen(rlength);
    bitHelper<<getStrFrom16Bits(rType)<<getStrFrom16Bits(rClass)<<getStrFrom32Bits(rTtl)<<getStrFrom16Bits(rLen);
    bitHelper<<addressToBitStr(address);
    return bitHelper.str();
}

std::string DNS_Relay_Server::queryDomainName(std::string qr) {
    return dnsDatabase.getValueFromKey(qr);
}

DNS_Relay_Server::DNS_Relay_Server(int port,std::string dnsFile,std::string remoteDNS,int debugClass)
        :serverPort(port),remoteDNS(remoteDNS),debugClass(debugClass)
{
    dnsDatabase.loadFile(dnsFile);
}


DNS_Data::DNS_Data(boost::array<char, RECE_BUFFER_SIZE> &dataBuf) {
    this->resolveData(dataBuf);
}

/* resolve dns data*/
void DNS_Data::resolveData(boost::array<char, RECE_BUFFER_SIZE> &dataBuf)
{
    /* header section */
    // id
    set16Bits(dataBuf[0],dataBuf[1],ID);
    idStr+=dataBuf[0];
    idStr+=dataBuf[1];

    std::stringstream bitHelper;
    std::bitset<8> flagBits1(dataBuf[2]);
    std::bitset<8> flagBits2(dataBuf[3]);
    bitHelper<<flagBits1<<flagBits2;
    bitHelper>>QR>>OPCODE>>AA>>TC>>RD>>RA>>Z>>RCODE;

    set16Bits(dataBuf[4],dataBuf[5],QDCOUNT);
    set16Bits(dataBuf[6],dataBuf[7],ANCOUNT);
    set16Bits(dataBuf[8],dataBuf[9],NSCOUNT);
    set16Bits(dataBuf[10],dataBuf[11],ARCOUNT);

    //question section
    int currentFlag=12;
    currentFlag=resolveQuestionSec(dataBuf,currentFlag);
    set16Bits(dataBuf[currentFlag],dataBuf[currentFlag+1],QTYPE);
    set16Bits(dataBuf[currentFlag+2],dataBuf[currentFlag+3],QCLASS);

    //currentFlag+=4;
    //resource section

}

void DNS_Data::debugPrintDNSData(int debugClass) {
    if(debugClass==2)
    {
        std::cout<<"\n***********************************************\n";
        std::cout<<"ID: "<<ID<<"  "<<ID.to_ulong()<<"\n";
        std::cout<<"QR: "<<QR<<"\n";
        std::cout<<"OPCODE: "<<OPCODE<<"\n";
        std::cout<<"AA: "<<AA<<"\n";
        std::cout<<"TC: "<<TC<<"\n";
        std::cout<<"RD: "<<RD<<"\n";
        std::cout<<"RA: "<<RA<<"\n";
        std::cout<<"Z: "<<Z<<"\n";
        std::cout<<"RCODE: "<<RCODE<<"\n";
        std::cout<<"QDCOUNT: "<<QDCOUNT<<"\n";
        std::cout<<"ANCOUNT: "<<ANCOUNT<<"\n";
        std::cout<<"NSCOUNT: "<<NSCOUNT<<"\n";
        std::cout<<"ARCOUNT: "<<ARCOUNT<<"\n";
        std::cout<<"QNAME: "<<qName<<"\n";
        std::cout<<"QTYPE: "<<QTYPE<<"\n";
        std::cout<<"QCLASS: "<<QCLASS<<"\n";
        std::cout<<"***********************************************\n";
    }

}

std::string DNS_Data::getQName()
{
    return qName;
}


int DNS_Data::resolveQuestionSec(boost::array<char, RECE_BUFFER_SIZE> &dataBuf,
                                  int currentFlag)
{
    std::bitset<8> nameCounter(dataBuf[currentFlag]);
    questionSection+=dataBuf[currentFlag];
    currentFlag++;

    int counter=nameCounter.to_ulong();
    while(counter>0)
    {
        qName+=dataBuf[currentFlag];
        questionSection+=dataBuf[currentFlag];
        currentFlag++;

        counter--;
        if(counter==0)
        {
            std::bitset<8> nameCounter(dataBuf[currentFlag]);
            questionSection+=dataBuf[currentFlag];
            currentFlag++;

            counter=nameCounter.to_ulong();
            if(counter)
                qName+='.';
        }
    }
   // questionSection+=(char)NULL;


    return currentFlag;
}

DNS_Data::DNS_Data() {

}

std::string DNS_Data::getHeaderData() {
    std::stringstream bitHelper;

    bitHelper<<idStr<<getFlag()<<getStrFrom16Bits(QDCOUNT)<<getStrFrom16Bits(ANCOUNT)
             <<getStrFrom16Bits(NSCOUNT)<<getStrFrom16Bits(ARCOUNT);
    bitHelper<<questionSection<<getStrFrom16Bits(QTYPE)<<getStrFrom16Bits(QCLASS);

    printStrByHex(bitHelper.str());
    std::cout<<"\n>>questionSection :"<<questionSection<<" \n";
    std::cout<<"\n>>bitHelper :"<<bitHelper.str()<<" \n";
    return bitHelper.str();
}

void DNS_Data::setQR(const std::bitset<1> &QR) {
    DNS_Data::QR = QR;
}

void DNS_Data::setRD(const std::bitset<1> &RD) {
    DNS_Data::RD = RD;
}

void DNS_Data::setRA(const std::bitset<1> &RA) {
    DNS_Data::RA = RA;
}

std::string DNS_Data::getFlag() {
     std::bitset<16> flagBits;
    std::stringstream bitHelper;

    bitHelper<<QR<<OPCODE<<AA<<TC<<RD<<RA<<Z<<RCODE;
    bitHelper>>flagBits;
    printStrByHex(bitHelper.str());
    std::cout<<"flag bits : "<<flagBits<<" \n";
    return getStrFrom16Bits(flagBits);
}

void DNS_Data::setANCOUNT(const std::bitset<16> &ANCOUNT) {
    DNS_Data::ANCOUNT = ANCOUNT;
}

void DNS_Data::setQTYPE(const std::bitset<16> &QTYPE) {
    DNS_Data::QTYPE = QTYPE;
}

void DNS_Data::setQCLASS(const std::bitset<16> &QCLASS) {
    DNS_Data::QCLASS = QCLASS;
}

const std::bitset<16> &DNS_Data::getQTYPE() const {
    return QTYPE;
}

void DNS_Data::setRCODE(const std::bitset<4> &RCODE) {
    DNS_Data::RCODE = RCODE;
}
