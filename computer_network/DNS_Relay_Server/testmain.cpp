#include <iostream>
#include "database.h"
//#include <hiredis>
//#include "DNS_Relay_Server.h"
//using boost::asio::ip::udp;

int main() {
    //DNS_Relay_Server server(53);
    //server.start();
    database test;
    test.writeFile();
    //string a="baidu.com";
    string b=test.readFile(a);
    cout<<b<<endl;
    cout<<"end"<<endl;
    return 0;
}