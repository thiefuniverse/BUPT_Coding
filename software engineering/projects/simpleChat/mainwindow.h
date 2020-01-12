#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chatwindow.h"
#include <QDebug>
#include <QTimer>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void on_connectButton_clicked();
    void tryShowChatRoom();
private:
    bool networkRight(QString ip);

private:
    Ui::MainWindow *ui;
    QTimer monitor;
    chatWindow* chatwindow;
};

#endif // MAINWINDOW_H
