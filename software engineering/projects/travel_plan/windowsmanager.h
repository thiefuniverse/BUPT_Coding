#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include <QDialog>

namespace Ui {
class WindowsManager;
}

class WindowsManager : public QDialog
{
    Q_OBJECT

public:
    explicit WindowsManager(QWidget *parent = 0);
    ~WindowsManager();

private:
    Ui::WindowsManager *ui;
};

#endif // WINDOWSMANAGER_H
