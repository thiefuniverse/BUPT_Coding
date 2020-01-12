#include "test_thief.h"
#include "ui_test_thief.h"

test_thief::test_thief(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::test_thief)
{
    ui->setupUi(this);
}

test_thief::~test_thief()
{
    delete ui;
}
