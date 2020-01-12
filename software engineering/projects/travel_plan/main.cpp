//#include <QGuiApplication>
#include<QApplication>
#include<QtWidgets/QLabel>
#include<timetable.h>
#include<QDebug>
#include"createnewcustomer.h"
#include"mainwindow.h"
#include"strategy.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
   // QTimer systemTime=new QTimer;
    MainWindow user_main;
    user_main.show();

//    Traveller thief;
//    Strategy  generator_stra;
//    int fee=generator_stra.leastFee__strategy(thief);
//    vector<int> temp=thief.getTravelRoad();
//    for(auto m:temp)
//    {
//        qDebug()<<m<<"   ";
//    }
//    qDebug()<<endl<<fee<<endl;
    return app.exec();
}

