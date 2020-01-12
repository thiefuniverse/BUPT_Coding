//
// Created by aymdr on 17-3-7.
//

#ifndef DNS_RELAY_SERVER_DATABASE_H
#define DNS_RELAY_SERVER_DATABASE_H

#include <iostream>
#include <cstring>
#include <fstream>
#include <hiredis/hiredis.h>

#define NUM 256
using namespace std;

class DNS_Database {
private:
    char buffer[NUM];
    string IP;
    string DomainName;
    redisContext* c;//保持连接状态
    const char* command;
public:
    DNS_Database();
    string getValueFromKey(string r);
    void loadFile(std::string dnsFileName);
};


#endif //REDIS_TEST_DATABASE_H
