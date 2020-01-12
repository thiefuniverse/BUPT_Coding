#ifndef TIMETABLE_H
#define TIMETABLE_H

#include<QtSql/QSql>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include<QSqlError>
#include<QMessageBox>
#include<QSqlRecord>
#include<QDebug>
#include<algorithm>
#include<vector>

#include"traveller.h"
//class Traveller;
#define BIGVALUE 999999

using std::vector;

/*时刻表*/
class TimeTable
{
public:
    TimeTable();

    /*连接数据库时间表*/
    bool connectTimeTable();
    //return value :表示是否ch成功连接数据库

    /*获取以当前城市为起点可以到达的下一个城市的集合*/
    vector<Traveller::CityName> getNextCity(Traveller::CityName currentCity);

    /*获取当前运输工具票价*/
    int getPrice(int transportId);
    //transportId :表示当前运输工具在时刻表中的id

    //获取两个城市之间的最少交通费用
    std::pair<int,int> getTwoCityFee(Traveller::CityName cityA,Traveller::CityName cityB);
    //返回车次ID和最少费用

private:
    QSqlDatabase db;
};

#endif // TIMETABLE_H
