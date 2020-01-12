#include "windowsmanager.h"
#include "ui_windowsmanager.h"

WindowsManager::WindowsManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WindowsManager)
{
    ui->setupUi(this);
}

WindowsManager::~WindowsManager()
{
    delete ui;
}
