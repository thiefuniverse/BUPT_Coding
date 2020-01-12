#ifndef CREATENEWCUSTOMER_H
#define CREATENEWCUSTOMER_H

#include<QApplication>
#include <QDialog>
#include<QWidget>
#include<QMessageBox>
#include<map>

#include"traveller.h"
class Traveller;


namespace Ui {
class CreateNewCustomer;
}

class CreateNewCustomer : public QDialog
{
    Q_OBJECT

public:
    explicit CreateNewCustomer(QWidget *parent = 0);
    ~CreateNewCustomer();
public slots:
    void create_Customer(Traveller* customer);
    void create_Sucess();
signals:
    //void timeToCreateCustomer(Traveller* customer);
private:
    Ui::CreateNewCustomer *ui;
};

#endif // CREATENEWCUSTOMER_H
