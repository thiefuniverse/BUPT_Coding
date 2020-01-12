#include "timetable.h"


/*时间表构造函数*/
TimeTable::TimeTable()
{

}

/*连接时刻表数据库*/
bool TimeTable::connectTimeTable()
{
    db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("hell");

    db.setUserName("root");
    db.setPassword("ssopq");

    if(!db.open())
    {
        //输出数据库连接错误信息
        QMessageBox::critical(0,QObject::tr("Database Error"),db.lastError().text());
        return false;
    }
    return true;
}

/*获取以当前城市为起点可以到达的下一个城市的集合*/
vector<Traveller::CityName> TimeTable::getNextCity(Traveller::CityName currentCity)
{
    QSqlQuery queryTool;
    queryTool.exec("SELECT arrivecity from travel where departcity=\""+Traveller::cities.at(currentCity)+"\"");
    qDebug()<<Traveller::cities.at(currentCity)<<endl;

    QSqlRecord queryHis=queryTool.record();

    qDebug()<<"Now get next Cities"<<endl;
    vector<Traveller::CityName> nextCity;
    while(queryTool.next())
    {
        QString temp=queryTool.value(0).toString();
        Traveller::CityName temp_name=(Traveller::CityName)Traveller::cities_map.find(temp)->second;
        nextCity.push_back(temp_name);
        //qDebug()<<queryTool.value(0).toString()<<endl;
    }
    sort(nextCity.begin(),nextCity.end());
    nextCity.erase(unique(nextCity.begin(),nextCity.end()),nextCity.end());

    //打印途经城市
    qDebug()<<"print nextCities:"<<endl;
    for(auto i=nextCity.begin();i!=nextCity.end();++i)
    {
 //       qDebug()<<*i<<endl;
    }
    return nextCity;

//            QSqlQuery q("select * from employees");
//            QSqlRecord rec = q.record();

//            qDebug() << "Number of columns: " << rec.count();

//            int nameCol = rec.indexOf("name"); // index of the field "name"
//            while (q.next())
//                qDebug() << q.value(nameCol).toString(); // output all names
}

/*获取当前运输工具票价*/
int TimeTable::getPrice(int transportId)
{

}


//获取两个城市之间的最少交通费用,返回车ID和费用的键值对
 pair<int,int> TimeTable::getTwoCityFee(Traveller::CityName cityA,Traveller::CityName cityB)
{
   //   vector<QString> citiesTwo={"Beijing","Tianjin","Haerbin","Wulumuqi","Chendu","Luan","Jinan","Xian","Dalian","Yinchuan","Huhehaote","Weihai","Taibei"};

      QSqlQuery queryTool;
      queryTool.exec("SELECT id,price from travel where arrivecity=\""+Traveller::cities[cityA]+"\" and departcity=\""+Traveller::cities[cityB]+"\"");

      QSqlRecord queryHis=queryTool.record();

      //获取两城市之间的所有交通可能性
      map<int,int> allPrice;
      while(queryTool.next())
      {
          allPrice.insert(pair<int,int>(queryTool.value(0).toInt(),queryTool.value(1).toInt()));
      }


//      for(auto i:allPrice)
//      {
//          qDebug()<<i.first<<"    "<<i.second<<endl;
//          qDebug()<<"heeeeeeeeeeeeeee"<<endl;
//      }

      //得到最小费用
      pair<int,int> tempMin(0,BIGVALUE);
      for(auto& i:allPrice)
      {
          if(i.second<tempMin.second)
          {
              tempMin=i;
          }
      }
      if(tempMin.second==BIGVALUE)
      {

      //    qDebug()<<Traveller::cities[cityA]<<" and "<<Traveller::cities[cityB]<<" NO WAY"<<endl;
          return tempMin;
      }
      else
          return tempMin;


}
