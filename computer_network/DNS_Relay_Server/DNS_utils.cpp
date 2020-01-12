//
// Created by thief on 17-3-7.
//

#include "DNS_utils.h"
#include <vector>
#include <bitset>
#include <iostream>
#include <sstream>


/* transfer address string (ip or domainname) to bitStr in dns*/
std::string addressToBitStr(std::string address)
{
    if (address.empty())
        return "";
    if (address[0]>='0'&&address[0]<='9')
        return ipToBitStr(address);
    else
        return domainNameToBitStr(address);
};

std::string domainNameToBitStr(std::string domainName)
{
    std::vector<std::string> domainPeices;
    std::string piece;
    for(auto c:domainName)
    {
        if(c!='.')
            piece+=c;
        else
        {
            domainPeices.push_back(piece);
            piece.clear();
        }
    }
    domainPeices.push_back(piece);

    std::string bitStr;
    for(auto piece:domainPeices)
    {
        std::bitset<8> leng(piece.size());
        bitStr+=(char)(leng.to_ulong());
        bitStr+=piece;
    }
  //  std::bitset<8> l(0);
  //  bitStr+=(char)(l.to_ulong());
    bitStr+=(char)NULL;
    std::cout<<">> response name : "<<bitStr<<"  <<";
    return bitStr;
};

std::string ipToBitStr(std::string ip)
{
    std::vector<std::string> domainPeices;
    std::string piece;
    for(auto c:ip)
    {
        if(c!='.')
            piece+=c;
        else
        {
            domainPeices.push_back(piece);
            piece.clear();
        }
    }
    domainPeices.push_back(piece);

    std::string bitStr;
    for(auto piece:domainPeices)
    {
        std::bitset<8> pie(std::stoi(piece));
        bitStr+=(char)(pie.to_ulong());
    }
    return bitStr;

};



void set16Bits(char one, char two, std::bitset<16> &bitsetDes)
{
    std::bitset<8> bits1(one);
    std::bitset<8> bits2(two);
    std::stringstream bitHelper;
    bitHelper<<bits1<<bits2;
    std::cout<<"id1: "<<bits1.to_string()<<" id2: "<<bits2.to_string()<<std::endl;
    bitHelper>>bitsetDes;
    std::cout<<bitsetDes.to_string();
}

std::string getStrFrom16Bits(std::bitset<16> &bitsetSour) {
    std::string bitStr=bitsetSour.to_string();
    std::bitset<8> first8(bitStr,0,8);
    std::bitset<8> second8(bitStr,8,8);
    std::stringstream bitH;
    bitH<<(char)(first8.to_ulong())<<(char)(second8.to_ulong());
    return bitH.str();
}

std::string getStrFrom32Bits(std::bitset<32> &bitsetSour) {
    std::string bitStr=bitsetSour.to_string();
    std::bitset<16> first16(bitStr,0,16);
    std::bitset<16> second16(bitStr,16,16);

    return getStrFrom16Bits(first16)+getStrFrom16Bits(second16);
}

void printStrByHex(const std::string str) {
    for(auto s:str)
    {
       // if(s<='9'&&s>='0')
        std::cout<<" "<<std::hex<<(unsigned int)s;
    //    else
    }
    std::cout<<std::endl;
}

void printCharArrayByHex(boost::array<char, RECE_BUFFER_SIZE> recBuf, int size) {
    int counter=0;
    std::cout<<std::endl;
    for (int i = 0; i < size; ++i) {
        counter++;
        unsigned  int val=(unsigned int)recBuf[i];
        if(val<10)
        std::cout<<" 0"<<std::hex<<val;
        else
        {
            std::cout<<" "<<std::hex<<val;
        }
        if (counter%16==0)
        {
            std::cout<<std::endl;
        }
    }
    std::cout<<std::endl;

}
