#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QTimer>
#include <QListWidget>
#include <QListWidgetItem>
#include "transfermessage.h"

class transferMessage;

namespace Ui {
class chatWindow;
}

class chatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatWindow(QWidget *parent = 0);
    ~chatWindow();

    enum WORD_DIRECTION{MYWORD, OTHERWORD};
    bool initSendSocket(QString ip);
    transferMessage *getTransferHelper() const;

public:
    void showText(QString& textStr, WORD_DIRECTION direct);
    void showImage(QImage imagePath, WORD_DIRECTION direct);
private slots:
    void on_send_clicked();

    void on_commandLinkButton_clicked();

private:
    Ui::chatWindow *ui;
    transferMessage* transferHelper;
};

#endif // CHATWINDOW_H
