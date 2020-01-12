#ifndef NEWCUSTOMER_H
#define NEWCUSTOMER_H
#include<QApplication>
#include <QMainWindow>

namespace Ui {
class newCustomer;
}

class newCustomer : public QMainWindow
{
    Q_OBJECT

public:
    explicit newCustomer(QWidget *parent = 0);
    ~newCustomer();

private:
    Ui::newCustomer *ui;
};

#endif // NEWCUSTOMER_H
