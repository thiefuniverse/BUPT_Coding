#ifndef TRANSFERMESSAGE_H
#define TRANSFERMESSAGE_H

#include <QObject>
#include <QtNetwork>
#include <QDebug>
#include <QImage>
#include "chatwindow.h"
class chatWindow;

class transferMessage : public QObject
{
    Q_OBJECT
public:
    explicit transferMessage(QObject *parent = nullptr);
    ~transferMessage();

    bool initSendSocket(QString ip);
    const int CONNECT_PORT = 9999;    // 连接端远程主机端端口  ；注意此处都一样，如果在不同电脑上同时运行，可以正常工作，
    const int LOCAL_PORT = 9999;     // 本地监听端端口        ；但是如果在同一台电脑上测试，需要修改端口号
    // 发送文字
    void sendText(QString textStr);
    // 发送图片
    void sendImage(QString imagePath);
    bool getIsServer() const;

    void setChatwindow(chatWindow *value);

private slots:
    void handleConnection();
    void handleMessage();
signals:

public slots:

private:
    QTcpSocket* clientSocket;
    QTcpServer* receiveServer;
    bool isServer;  // 记录当前聊天窗口是否作为Server端存在
    chatWindow *chatwindow;

    qint32 transferSize;   // 当前正在接收到图片到大小
};

#endif // TRANSFERMESSAGE_H
