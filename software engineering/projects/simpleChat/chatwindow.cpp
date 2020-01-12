#include "chatwindow.h"
#include "ui_chatwindow.h"

chatWindow::chatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);

    transferHelper = new transferMessage();
    transferHelper->setChatwindow(this);
}

chatWindow::~chatWindow()
{
    delete ui;
}

// 初始化用于发送内容到socket
bool chatWindow::initSendSocket(QString ip)
{
    return transferHelper->initSendSocket(ip);
}

// 添加文字到主界面上
void chatWindow::showText(QString &textStr, WORD_DIRECTION direct)
{
    QListWidgetItem *newItem = new QListWidgetItem(textStr);

    // 自己发的右对齐，接收到消息右对齐
    if (direct == MYWORD){
        newItem->setTextAlignment(Qt::AlignRight);
    }
    else {
        newItem->setTextAlignment(Qt::AlignLeft);
    }

    //添加内容到主界面
    ui->showContent->addItem(newItem);
}

// 添加图片到聊天界面上
void chatWindow::showImage(QImage image, WORD_DIRECTION direct)
{

    // 图片过大，缩小
    if (image.width() > 400){
        image = image.scaled(400, 400, Qt::KeepAspectRatio);
    }

    // 由于图片不支持左右对齐，所以加上说明文字 :)
    QString flagInfo =  (direct == MYWORD ? "my image" : "received image");

    // 新建一个List项，包含图片
    QListWidgetItem *newItem = new QListWidgetItem(flagInfo, ui->showContent);
    newItem->setData(Qt::DecorationRole, QPixmap::fromImage(image));


    if (direct == MYWORD){
        newItem->setTextAlignment(Qt::AlignRight);
    }
    else {
        newItem->setTextAlignment(Qt::AlignLeft);
    }
    //添加内容到主界面
    ui->showContent->addItem(newItem);
}

// 发送消息
void chatWindow::on_send_clicked()
{
    // 获取文字框到内容
    QString sendText = ui->sendWindow->toPlainText();

    // 使用transferHelper 发送文字
    transferHelper->sendText(sendText);

    // 在主界面上显示自己发送到内容
    this->showText(sendText, MYWORD);
}

//发送图片
void chatWindow::on_commandLinkButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(this,tr("open file"),"", tr("Allfile(*.*)"));
    qDebug() << imagePath;
    // 如果选择了图片则传输，否则什么也不做
    if (!imagePath.toStdString().empty()){
        QImage image(imagePath);
        // 使用transferHelper 发送图片
        transferHelper->sendImage(imagePath);

        //在主界面上显示自己发送到图片
        this->showImage(image,MYWORD);
    }

}

transferMessage *chatWindow::getTransferHelper() const
{
    return transferHelper;
}
