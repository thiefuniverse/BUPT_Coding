//
// Created by aymdr on 17-3-5.
//

#include "DNS_Database.h"
#include <sstream>

DNS_Database::DNS_Database() {
    //redis port=6379
    c = redisConnect("127.0.0.1",6379);//连接redis数据库
    if(c->err){
        redisFree(c);
        printf("Connect to redisServer failed\n");
        return;
    }
    printf("Connect to redisServer Success\n");

}

void DNS_Database::loadFile(std::string dnsFileName) {
    ifstream DNS_data(dnsFileName);
    redisReply* msg;
    if(!DNS_data.is_open()){
        cout<<"Error opening file";
        return;
    }
    string ipDomainName;
    while( getline(DNS_data,ipDomainName)){
        stringstream readIp(ipDomainName);
        readIp>>IP>>DomainName;
        string s="set "+DomainName+' '+IP;
        cout<<s<<endl;
        command = s.c_str();
        //发送该命令到redis
        msg = (redisReply*)redisCommand(c,command);
        if(NULL==msg){
            cout<<"Execute command failure"<<endl;
    //        redisFree(c);
            return;
        }
        if(!(msg->type == REDIS_REPLY_STATUS&&strcasecmp(msg->str,"OK")==0)){
            cout<<"Failed to execute"<<endl;
            freeReplyObject(msg);
        //    redisFree(c);
            return;
        }
    }
    cout<<"success"<<endl;
    freeReplyObject(msg);
}

string DNS_Database::getValueFromKey(string r) {
    string s= "get " +r;
    command = s.c_str();
    redisReply* msg = (redisReply*)redisCommand(c,command);
    if(msg->type!=REDIS_REPLY_STRING){
        freeReplyObject(msg);
        return "";
    }
    string result = msg->str;
    freeReplyObject(msg);
    return result;
}


