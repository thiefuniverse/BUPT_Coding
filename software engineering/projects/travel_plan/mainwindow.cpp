#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    //主界面启动
    ui->setupUi(this);

    //系统时间初始化、计时开始
    this->initSystemTime();

    systemTime=new QTimer(this);
    connect(systemTime,SIGNAL(timeout()),this,SLOT(timeUpDate()));
    systemTime->start(INTERNAL_TIME);


    //点击注册旅客后弹出注册界面
    connect(ui->signUp_Button,SIGNAL(clicked()),this,SLOT(create_NewCustomer()));


    //添加图片背景
    this->map_image=new QImage();
    map_image->load("D:\\study\\travel\\travel_plan\\welcome.jpg");
    *map_image=map_image->scaled(491,411,Qt::KeepAspectRatio);
    QGraphicsScene *map_scene=new QGraphicsScene();
    map_scene->addPixmap(QPixmap::fromImage(*map_image));
    ui->main_map->setScene(map_scene);
    ui->main_map->resize(map_image->width()+2,map_image->height()+2);
    ui->main_map->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//主窗口点击注册新旅客后弹出注册界面
void MainWindow::create_NewCustomer()
{
    Traveller newCus;
    Strategy  generator_stra;
    int fee=generator_stra.leastFee__strategy(newCus);
    vector<int> temp=newCus.getTravelRoad();

    for(auto m:temp)
    {
        qDebug()<<m;
    }
    qDebug()<<endl<<"leastfee"<<fee<<endl;

    //将点击后新建的旅客添加到主界面类中
    this->addCustomer(newCus);
}

//添加新旅客
void MainWindow::addCustomer(Traveller &newCustomer)
{
    allCustomers.push_back(newCustomer);

}

void MainWindow::timeUpDate()
{
    //更新时间系统时间
    timeDH.second=(++timeDH.second)%24;
    if(!timeDH.second)
    {
        ++timeDH.second;
    }
  //  qDebug()<<currentDay<<"  "<<currentHour;
    systemTime->start(INTERNAL_TIME);

    //更新旅客状态
}

void MainWindow::initSystemTime()
{
    timeDH=make_pair(0,0);
}
