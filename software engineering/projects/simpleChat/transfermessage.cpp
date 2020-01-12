#include "transfermessage.h"

transferMessage::transferMessage(QObject *parent) : QObject(parent)
{
    // 启动一个Server端，用于接受可能端连接
    receiveServer =  new QTcpServer();
    receiveServer->listen(QHostAddress::Any, LOCAL_PORT);

    qDebug() << "listening "<<LOCAL_PORT;
    // 如果有新的连接，则触发 handleConnetion() 函数
    connect(receiveServer,SIGNAL(newConnection()),this, SLOT(handleConnection()));

    isServer = false;
    transferSize = 0;

}

transferMessage::~transferMessage()
{
    delete receiveServer;
}

bool transferMessage::initSendSocket(QString ip)
{
    clientSocket = new QTcpSocket();

    // tcpsocket 连接对方主机
    clientSocket->connectToHost(QHostAddress(ip),CONNECT_PORT);

    // 3秒内连接成功则继续，否则失败
    if(clientSocket->waitForConnected(3000))
    {
        qDebug()<<"connected!";
        connect(clientSocket,SIGNAL(readyRead()),this,SLOT(handleMessage()));
        return true;
    }
    else
    {
        qDebug()<<"connection failed";
        return false;
    }
}

// 发送文字,在发送串头部追加一个字符 t
void transferMessage::sendText(QString textStr)
{
    QByteArray dataStr;
    QDataStream out( &dataStr,QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_5_8);
    out << QChar('t')<< textStr;
    clientSocket->write(dataStr);
}

// 发送图片,在发送串头部追加一个字符 i 和 图片字节大小(qint32存储)
void transferMessage::sendImage(QString imagePath)
{
    QImage img(imagePath);
    QBuffer buffer;
    QByteArray dataStr;
    QDataStream out( &dataStr,QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_5_8);
    buffer.open(QIODevice::ReadWrite);
    img.save(&buffer,"PNG");
    out << QChar('i')<<qint32(buffer.size())<< buffer.data();
    clientSocket->write(dataStr);
}

void transferMessage::handleConnection()
{
    // 有其他用户连接本机，新建一个socket用于通信
    clientSocket =receiveServer->nextPendingConnection();
    connect(clientSocket,SIGNAL(readyRead()),this,SLOT(handleMessage()));
    isServer = true;
}

void transferMessage::handleMessage()
{
    QDataStream in(clientSocket);
    in.setVersion(QDataStream::Qt_5_8);
    if (transferSize == 0){

        // 获取消息标志判断消息类型
        QChar flag;
        in >> flag;
        // 如果是文字消息
        if (flag == 't'){
            QString message;

            //获取消息
            in >> message;
            qDebug() << " first 4 char: "<<QString::fromStdString(message.toStdString().substr(0,4));
            qDebug() << "get message: "<<message;

            // 在主界面上显示接收到的消息
            chatwindow->showText(message, chatWindow::OTHERWORD);
            return;
        }
        // 如果是图片消息
        else if (flag == 'i')
        {
            //获取图片大小
            in >> transferSize;

            //如果未接收完，则暂时不读取，等待读完
            if (transferSize > qint32(clientSocket->bytesAvailable())){
                return;
            }

            // 从socket中读娶图片
            QByteArray rece;
            in >> rece;
            QImage img;

            // 加载为图片后添加到主界面
            img.loadFromData(rece);
            chatwindow->showImage(img, chatWindow::OTHERWORD);
            transferSize = 0;
            return;
        }
        else {
            qDebug() << "transfer error";
            transferSize = 0;
        }

    }

    // 之前已经接收了部分图片，现在判断是否接收完
    if (transferSize > qint32(clientSocket->bytesAvailable())){
        return;
    }
    QByteArray rece;
    in >> rece;
    QImage img;
    img.loadFromData(rece);

    chatwindow->showImage(img, chatWindow::OTHERWORD);

    transferSize = 0;

}

void transferMessage::setChatwindow(chatWindow *value)
{
    chatwindow = value;
}

bool transferMessage::getIsServer() const
{
    return isServer;
}
