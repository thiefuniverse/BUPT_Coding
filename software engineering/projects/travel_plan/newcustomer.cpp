#include "newcustomer.h"
#include "ui_newcustomer.h"

newCustomer::newCustomer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::newCustomer)
{
    ui->setupUi(this);

}

newCustomer::~newCustomer()
{
    delete ui;
}
