#include "strategy.h"

Strategy::Strategy()
{
    initStrategy();
}

void Strategy::leastTime_strategy(Traveller& customer)
{
    //int allPath[ALLCITY][ALLCITY]=0;

}

int Strategy::leastFee__strategy(Traveller& customer)
{
    //获取途经城市和起始终止城市
    int startCity=(int)customer.getStartPlace();
    int endCity=(int)customer.getEndPlace();

    map<Traveller::CityName,int>& pass=customer.getpassCities();
    vector<int> passCity;
    for(auto i:pass)
    {
        passCity.push_back((int)i.first);
    }


    //排序以为生成全排列做好准备
    sort(passCity.begin(),passCity.end());

    //添加所有可能到allPassPro中
    vector<vector<int>> allPassPro;
    allPassPro.push_back(passCity);
    while(next_permutation(passCity.begin(),passCity.end()))
    {
        allPassPro.push_back(passCity);
    }


    //挑选出最短路径
    int least_fee=INFINITY;
    vector<int> least_fee_road;

    for(auto& mid:allPassPro)
    {
        int temp_least_fee=0;
        temp_least_fee+=Path[startCity][mid[0]];
        for(auto i=mid.begin();i!=mid.end()-1;++i)
        {
            temp_least_fee+=Path[*i][*(i+1)];
        }
        temp_least_fee+=Path[*(mid.end()-1)][endCity];
        if(temp_least_fee<least_fee)
        {
            least_fee=temp_least_fee;
            least_fee_road=mid;
        }

    }
    least_fee_road.insert(least_fee_road.begin(),startCity);
    least_fee_road.push_back(endCity);

    for(auto i=least_fee_road.begin();i!=least_fee_road.end()-1;++i)
        qDebug()<<Path[*i][*(i+1)]<<"   fee";

    customer.setTravelRoad(least_fee_road);
    return least_fee;

}

void Strategy::leastFeeTime_strategy(Traveller& customer)
{

}

Strategy::~Strategy()
{

}

void Strategy::initStrategy()
{

    //弗洛伊德算法
    for(int i=0;i!=ALLCITY;++i)
    {
        for(int j=0;j!=ALLCITY;++j)
        {
            if(i==j)
            {
                Path[i][j]=0;
            }
            else
            {
                Path[i][j]=INFINITY;
            }
        }
    }
    TimeTable timeTable_stra;
    timeTable_stra.connectTimeTable();

    for(int depart=0;depart!=ALLCITY;++depart)
    {
        //获取depart出发城市的可到达城市
        vector<Traveller::CityName> temp_nextCity=timeTable_stra.getNextCity((Traveller::CityName)depart);

        //把城市图添加到弗洛伊德图中
        for(auto arrive:temp_nextCity)
        {
            pair<int,int> transport=timeTable_stra.getTwoCityFee((Traveller::CityName)depart,arrive);
            Path[depart][(int)arrive]=transport.second;
        }
    }

    //让弗洛伊德的图对称
    for(int i=0;i!=ALLCITY;++i)
    {
        for(int j=i;j!=ALLCITY;++j)
        {
            Path[j][i]=Path[i][j];
        }
    }

    //初始化路径记录图
    //int shortPathTable[ALLCITY][ALLCITY];//记录路径
    for(int i=0;i!=ALLCITY;++i)
    {
        for(int j=0;j!=ALLCITY;++j)
        {
            shortPathTable[i][j]=j;
        }
    }

    //算法主体
    for(int k=0;k<ALLCITY;++k)
    {
        for(int v=0;v<ALLCITY;++v)
        {
            for(int w=0;w<ALLCITY;++w)
            {
                if(!(Path[v][k]==INFINITY||Path[k][w]==INFINITY)&&Path[v][w]>Path[v][k]+Path[k][w])
                {
                    Path[v][w]=Path[v][k]+Path[k][w];
                    shortPathTable[v][w]=Path[v][k];
                }
            }
        }
    }
}
