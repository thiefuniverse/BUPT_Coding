#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include<QTimer>
#include <QDialog>
#include<QImage>
#include<QGraphicsView>
#include<QGraphicsScene>
#include<QDebug>
#include"traveller.h"
#include"createnewcustomer.h"
#include"strategy.h"

#define INTERNAL_TIME 1000

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addCustomer(Traveller& newCustomer);

    //初始化系统时间
    void initSystemTime();

    //获取系统时间
    pair<int,int>& getSystemTime();
public slots:
    void create_NewCustomer();
    void timeUpDate();

private:
    Ui::MainWindow *ui;
    QImage *map_image;

    vector<Traveller> allCustomers;
    QTimer* systemTime;

    //当前时间的天数和小时表示
    pair<unsigned int,unsigned int> timeDH;
};

#endif // MAINWINDOW_H
