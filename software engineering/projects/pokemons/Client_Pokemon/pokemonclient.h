#ifndef POKEMONCLIENT_H
#define POKEMONCLIENT_H
#include"playui.h"
#include"window_signup.h"
#include"pokemondataprocess.h"
#include"player.h"
#include<QObject>
#include<QApplication>

class PlayUi;
class Window_SignUp;

class PokemonClient:public QObject
{
    //Q_OBJECT

public:

    PokemonClient();
    ~PokemonClient();
    void initWindows();
    void initEnvironment();
    void showSignUp();
    void backToSign();
    void exitProgram();
    void setMainProgram(QApplication *value);

public slots:
    void readPendingDatagram();
private:
    QUdpSocket* receiver;
    PlayUi *playui;
    Window_SignUp* windows_signup;
    PokemonDataProcess *dataProcessor;
    PokemonPlayer* currentPlayer;
    QApplication* mainProgram;
};

#endif // POKEMONCLIENT_H
