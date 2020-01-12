#include "traveller.h"

Traveller::Traveller()
{
    CreateNewCustomer newCustomer;
    newCustomer.exec();
    newCustomer.create_Customer(this);

    this->ID=travellerID++;

}
//Traveller::~Traveller()
//{

//}

void Traveller::inputDemmand()
{

}

//设置开始城市
bool Traveller::setStartPlace(Traveller::CityName startCity)
{
    if(startCity<0)
    {
        return false;
    }
    this->startPlace=startCity;
    return true;
}

//设置终点城市
bool Traveller::setEndPlace(Traveller::CityName endCity)
{
    if(endCity<0)
    {
        return false;
    }
    this->endPlace=endCity;
    return true;
}

//设置当前城市
bool Traveller::setCurrentCity(Traveller::CityName curCity)
{
    if(curCity<0)
    {
        return false;
    }
    this->currentCity=curCity;
    return true;
}

//设置途经城市
void Traveller::setPassCities(map<Traveller::CityName,int>& pass)
{
    this->passCities=pass;
}

//设置所选策略
void Traveller::setStrategy(STRATEGY_TYPE selectedStrategy)
{
    this->myStrategy=selectedStrategy;
}

//获取开始城市
Traveller::CityName Traveller::getStartPlace()
{
    return this->startPlace;
}

//获取终点城市
Traveller::CityName Traveller::getEndPlace()
{
    return this->endPlace;
}

//获取当前城市
Traveller::CityName Traveller::getCurrentCity()
{
    return this->currentCity;
}

//获取途经城市
map<Traveller::CityName,int>& Traveller::getpassCities()
{
    return passCities;
}

//获取所选策略
STRATEGY_TYPE Traveller::getStrategy()
{
    return this->myStrategy;
}


/*辅助打印函数 ---- 打印途经城市*/
void Traveller::printPassCities()
{
    for(auto i=passCities.begin();i!=passCities.end();++i)
    {
        qDebug()<<"Pass City：  "<<cities[i->first]<<"  Stay Time:  "<<i->second<<endl;
    }
}

/*设置旅游路线*/
void Traveller::setTravelRoad(vector<int> &myTravelRoad)
{
    myRoad=myTravelRoad;
}

//获取当前路线
vector<int>& Traveller::getTravelRoad()
{
    return this->myRoad;
}

void Traveller::initTimeForward()
{
   // currentPosition=make_pair(myRoad[0],passCities[myRoad[0]]);
}

void  Traveller::timeForward(int day,int hour)
{
    --currentPosition.second;
    if(!currentPosition.second)
    {

    }
}




const vector<QString> Traveller::cities={"Beijing","Tianjin","Haerbin","Wulumuqi","Chendu","Luan","Jinan","Xian","Dalian","Yinchuan","Huhehaote","Weihai","Taibei"};\
const map<QString,int> Traveller::cities_map={
    {"Beijing",0},{"Tianjin",1},{"Haerbin",2},{"Wulumuqi",3}
    ,{"Chendu",4},{"Luan",5},{"Jinan",6},{"Xian",7}
    ,{"Dalian",8},{"Yinchuan",9},{"Huhehaote",10},{"Weihai",11},{"Taibei",12}};

