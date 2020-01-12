#ifndef TRAVELLER_H
#define TRAVELLER_H

#include<QDebug>
#include<vector>
#include<map>

#include"createnewcustomer.h"
class Strategy;


typedef  int STRATEGY_TYPE;
#define LEAST_TIME_STRA 1 //最少时间
#define LEAST_FEE_STRA 2  //最少费用
#define LEAST_TIME_AND_FEE 3  //限时最少费用

using namespace std;
using std::map;
using std::vector;

//简化城市类型操作为int型变量操作

/*旅客信息*/
class Traveller
{
    int travellerID=0;
public:
    /*构造函数运行时弹出新窗口要求用户输入*/
    Traveller();



    enum CityName{BEIJING,TIANJIN,HAERBIN,\
                  WULUMUQI,CHENDU,LUAN,JINAN,XIAN,\
                  DALIAN,YINCHUAN,HUHEHAOTE,WEIHAI,\
                  TAIBEI};

    static const vector<QString> cities;
    static const map<QString,int> cities_map;

    /*读取旅客需求*/
    void inputDemmand();


    // ~Traveller();

    /*设置和获取起点城市*/
    bool setStartPlace(CityName startCity);
    CityName getStartPlace();

    /*设置和获取终点城市*/
    bool setEndPlace(CityName endCity);
    CityName getEndPlace();

    /*设置和获取途经城市*/
    void setPassCities(map<CityName,int>& yourPassCities);
    map<CityName,int>& getpassCities();

    /*设置和获取当前城市*/
    bool setCurrentCity(CityName curCity);
    CityName getCurrentCity();

    /*辅助打印函数 ---- 打印途经城市*/
    void printPassCities();

    /*设置和获取当前旅客路线*/
    void setTravelRoad(vector<int>& myTravelRoad);
    vector<int>& getTravelRoad();

    /*设置和获取当前策略*/
    void setStrategy(STRATEGY_TYPE selectedStrategy);
    STRATEGY_TYPE getStrategy();

    /*时间推移初始化*/
    void initTimeForward();

    /*随时间片推移函数*/
    void timeForward(int day,int hour);


private:
    int ID;                     //旅客编号
    CityName startPlace;         //起点城市
    CityName endPlace;           //终点城市
    map<CityName,int> passCities;//改成字典,添加停留时间
    vector<int>  myRoad;     //获取当前旅客路线
    CityName currentCity;        //当前城市
    STRATEGY_TYPE myStrategy;   //策略类型

    pair<CityName,int> currentPosition; //当前城市和到达下一城市所需小时数
};

#endif // TRAVELLER_H
