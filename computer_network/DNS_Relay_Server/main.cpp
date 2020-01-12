#include <iostream>
#include <boost/asio.hpp>
#include <bitset>
#include "DNS_Relay_Server.h"
#include "DNS_utils.h"
#include "DNS_Database.h"
#include <sstream>

using boost::asio::ip::udp;
#define DNS_PORT 53

int getDebugClass(char* classStr)
{
    std::string debugFlag(classStr);
    if(debugFlag=="-d")
        return 1;
    else if(debugFlag=="-dd")
        return 2;
    else
    {
        std::cout<<"Debug class set error!\n";
    }

}

int main(int argc,char* argv[]) {
    std::string dnsFile="dnsrelay.txt";
    std::string remoteDNS="10.3.9.6";
    int defaultPort=53;
    int debugClass=0;
    if(argc==1)
    {
        DNS_Relay_Server server(defaultPort,dnsFile,remoteDNS,debugClass);
    }
    else if (argc==2)
    {
        debugClass=getDebugClass(argv[1]);
        DNS_Relay_Server server(defaultPort,dnsFile,remoteDNS,debugClass);
    }
    else if(argc==3)
    {
        debugClass=getDebugClass(argv[1]);
        remoteDNS=std::string(argv[2]);
        DNS_Relay_Server server(defaultPort,dnsFile,remoteDNS,debugClass);
    }
    else if(argc==4)
    {
        debugClass=getDebugClass(argv[1]);
        remoteDNS=std::string(argv[2]);
        dnsFile=std::string(argv[3]);
        DNS_Relay_Server server(defaultPort,dnsFile,remoteDNS,debugClass);
    }
    else
    {
        std::cout<<" arguments error!"<<std::endl;
    }

  //  server.start();
    return 0;
}