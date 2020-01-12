#ifndef STRATEGY_H
#define STRATEGY_H

#include"traveller.h"
#include"timetable.h"

#include<QtSql/QSql>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QSqlError>
#include<QMessageBox>
#include<QSqlRecord>
#include<QDebug>
#include<algorithm>
#include<vector>


#define LEAST_TIME_STRA 1 //最少时间
#define LEAST_FEE_STRA 2  //最少费用
#define LEAST_TIME_AND_FEE 3  //限时最少费用
#define ALLCITY 13
#define INFINITY 9999

typedef  int STRATEGY_TYPE;
class Traveller;
class TimeTable;

/*三种策略*/
class Strategy
{
public:
    Strategy();
    ~Strategy();
    //friend class Traveller;
    //最少时间策略
    void leastTime_strategy(Traveller& customer);

    //最少费用策略
    int leastFee__strategy(Traveller& customer);
    //返回最少费用

    //限时最少费用策略
    void leastFeeTime_strategy(Traveller& customer);

    void initStrategy();
private:
    int shortPathTable[ALLCITY][ALLCITY];//记录路径
    int Path[ALLCITY][ALLCITY];//记录最短的费用


};

#endif // STRATEGY_H
