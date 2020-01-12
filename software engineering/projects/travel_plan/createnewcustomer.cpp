#include "createnewcustomer.h"
#include "ui_createnewcustomer.h"
using std::vector;
void create_Customer_Button(Traveller* customer);



/*旅客输入要求的窗口*/
CreateNewCustomer::CreateNewCustomer(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateNewCustomer)
{
    ui->setupUi(this);
   connect(ui->createCustomer_Button,SIGNAL(clicked()),this,SLOT(create_Sucess()));
   connect(ui->createCustomer_Button,SIGNAL(clicked()),this,SLOT(close()));
   this->setMaximumSize(542,358);
   this->setMinimumSize(542,358);
   // connect(ui,SIGNAL(timeToCreateCustomer(Traveller*)),this,SLOT(create_Customer_Button(Traveller*)));

}

CreateNewCustomer::~CreateNewCustomer()
{
    delete ui;
}

void CreateNewCustomer::create_Sucess()
{
    QMessageBox::information(NULL,tr("恭喜您！！"),tr("注册成功！！"));
}

void CreateNewCustomer::create_Customer(Traveller* customer)
{
     vector<QString> cities={"北京","天津","哈尔滨","乌鲁木齐","成都","六安","济南","西安","大连","银川","呼和浩特","威海","台北"};

     int start,end;

     for(int i=0;i!=cities.size();++i)
     {
         if(ui->start_City->currentText()==cities[i])
         {
             start=i;
         }
         if(ui->end_City->currentText()==cities[i])
         {
             end=i;
         }
     }
    //设置起止城市
    customer->setStartPlace((Traveller::CityName)start);
    customer->setEndPlace((Traveller::CityName)end);

    //设置当前城市
    customer->setCurrentCity((Traveller::CityName)start);

    //设置途径城市
    map<Traveller::CityName,int> passCities;


    //判断复选框如果勾选则将该城市加入顾客信息中

    //first line
    if(ui->Beijing_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::BEIJING,ui->Beijing_Time->value()));
    }
    if(ui->Tianjin__Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::TIANJIN,ui->Tianjin_Time->value()));
    }
    if(ui->HaerBin_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::HAERBIN,ui->HaerBin_Time->value()));
    }
    if(ui->WuluMuQi_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::WULUMUQI,ui->WuluMuQi_Time->value()));
    }

    //Second Line
    if(ui->Chendu_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::CHENDU,ui->Chendu_Time->value()));
    }
    if(ui->Luan_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::LUAN,ui->Luan_Time->value()));
    }
    if(ui->Jinan_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::JINAN,ui->Jinan_Time->value()));
    }
    if(ui->Xian_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::XIAN,ui->Xian_Time->value()));
    }

    //third Line
    if(ui->Dalian_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::DALIAN,ui->Dalian_Time->value()));
    }
    if(ui->Yinchuan_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::YINCHUAN,ui->Yinchuan_Time->value()));
    }
    if(ui->HuheHaoTe_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::HUHEHAOTE,ui->HuheHaoTe_Time->value()));
    }
    if(ui->WeiHai_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::WEIHAI,ui->WeiHai_Time->value()));
    }
    if(ui->Taibei_Button->isChecked())
    {
        passCities.insert(pair<Traveller::CityName,int>(Traveller::CityName::TAIBEI,ui->Taibei_Time->value()));
    }


    //设置途径城市
    customer->setPassCities(passCities);

    //设置策略选项
    if(ui->leastFee_Strategy->isChecked())
    {
        customer->setStrategy(LEAST_FEE_STRA);
    }
    else if(ui->leastTime_Fee_Strategy->isChecked())
    {
        customer->setStrategy(LEAST_TIME_AND_FEE);
    }
    else if(ui->leastTime_Strategy->isChecked())
    {
       customer->setStrategy(LEAST_TIME_STRA);
    }
    else
    {
        ;//提示需要选择策略！！！！！！！！！！！！！！！！！！！！！！
    }
}
