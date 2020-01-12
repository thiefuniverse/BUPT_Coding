//
// Created by thief on 17-3-7.
//

#ifndef DNS_RELAY_SERVER_DNS_UTILS_H
#define DNS_RELAY_SERVER_DNS_UTILS_H
#include <string>
#include <bitset>
#include <boost/array.hpp>

#define  RECE_BUFFER_SIZE 1024

std::string addressToBitStr(std::string address);
std::string domainNameToBitStr(std::string domainName);
std::string ipToBitStr(std::string ip);


void set16Bits(char one,char two,std::bitset<16>& bitsetDes);

std::string getStrFrom16Bits(std::bitset<16>& bitsetSour);

std::string getStrFrom32Bits(std::bitset<32>& bitsetSour);

void printStrByHex(const std::string str);

void printCharArrayByHex(boost::array<char,RECE_BUFFER_SIZE> recBuf,int size);

#endif //DNS_RELAY_SERVER_DNS_UTILS_H
