#ifndef WINDOW_SIGNUP_H
#define WINDOW_SIGNUP_H

#include <QDialog>
#include<QDebug>
#include<QVector>
#include<QLineEdit>
#include<QMessageBox>
#include"pokemonencrypter.h"
#include"pokemondataprocess.h"

class PokemonDataProcess;
class PokemonPlayer;

namespace Ui {
class Window_SignUp;
enum LOGIN_TYPE{SIGNIN,SIGNUP};
}

class Window_SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit Window_SignUp(QWidget *parent = 0);
    ~Window_SignUp();

signals:
    void signFeedBack(bool success);
private slots:

    void on_signIn_clicked();

    void on_signUp_clicked();

    void on_cancel_clicked();

    void on_ok_clicked();

    void on_showPassword_clicked();
public slots:
    void readPendingDatagram();
    void signFeedbackProcess(bool success);
private:
    void initLoginWindow();
    void initSocket();
    void switchState(Ui::LOGIN_TYPE type);
    void ok_signUp();
    void ok_signIn();
    void setPassEcho(QLineEdit::EchoMode show);
    void showInput();

    QUdpSocket* receiver;
    Ui::Window_SignUp *ui;
    Ui::LOGIN_TYPE loginState;
    PokemonDataProcess* dataProcessor;
    PokemonPlayer* currentPlayer;
};

#endif // WINDOW_SIGNUP_H
