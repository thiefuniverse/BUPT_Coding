#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chatwindow = new chatWindow();

    // 设置定时器，没30毫秒调用一次 tryShowChatRoom函数,判断本机是否作为server端，如果是，则接收另一人连接之后则立即显示聊天界面
    monitor.setInterval(30);
    monitor.start();
    connect(&monitor,SIGNAL(timeout()),this,SLOT(tryShowChatRoom()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

// 测试目标IP是否可达
bool MainWindow::networkRight(QString ip)
{
    int exitCode;
    qDebug() << "ping " + ip << endl;
    #ifdef Q_OS_WIN  // windows
    QString strArg = "ping " + ip + " -n 1 -i 2";
    exitCode = QProcess::execute(strArg);
    #else
    //其他平台(Linux或Mac)
    exitCode = QProcess::execute("ping",  QStringList() << "-c 1" << "-t 2" << ip); // removable
    #endif

    if (exitCode == 0){
        return true;
    }
    else {
        return false;
    }
    qDebug() << "exit code : "<< exitCode;
}

//点击连接之后， 开始与另一个窗口建立连接通信
void MainWindow::on_connectButton_clicked()
{
    // 如果IP窗口为空，则提示输入IP
   if(ui->ipField->toPlainText().toStdString().empty()) {
       ui->status->setText("请输入IP");
   }
   else {
       QString ip = ui->ipField->toPlainText();

       //测试可以 ping通对方
       if (networkRight(ip)){

           // 如果可以ping通， 则初始化通信socket
           if (chatwindow->initSendSocket(ip)){
               // 尝试连接另一个用户开始
               this->hide();
               chatwindow->show();
           }
           else{
               // 初始化失败，提示错误
               ui->status->setText("连接错误，请检查网络");
           }

       }
       else {
           // ping 不通
           ui->status->setText("对方IP不可达，请检查网络");
       }
   }
   ui->ipField->clear();
}

void MainWindow::tryShowChatRoom()
{
    // 如果当前聊天窗口作为server端存在并已建立连接，则直接显示聊天界面，并解除槽函数
    if (chatwindow->getTransferHelper()->getIsServer()){
        this->hide();
        chatwindow->show();
        disconnect(&monitor,SIGNAL(timeout()),this,SLOT(tryShowChatRoom()));
    }


}
