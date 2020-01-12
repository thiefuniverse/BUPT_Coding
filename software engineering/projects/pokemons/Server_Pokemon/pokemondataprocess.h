#ifndef POKEMONDATAPROCESS_H
#define POKEMONDATAPROCESS_H

#include<QUdpSocket>
#include<QByteArray>
#include<QDataStream>
#include<QVector>
#include"PokemonServer.h"
#include"player.h"
#include"pokemon.h"
#include"pokemondata.h"
#include"window_signup.h"

class Window_SignUp;
class PokemonServer;
class PokemonPlayer;


class PokemonDataProcess:public QUdpSocket
{
 //   Q_OBJECT
public:
     const QString DEFAULT_ADDRESS="127.0.0.1";

    //symbol for type 1, 2, 3, 4, 5, 6

    /*  Define Data Content
     *  All data pack have a foward flag (which is a int, means 1 for ack,
     * 2 for sign_in and etc.)
     *
     *  ACK      int
     *  SIGN_IN  QVector<String> (username, password)
     *  SIGN_UP  QVector<String> (username, password)
     *  SIGN_OUT QVector<String> (username, password)
     *  POKEMON  QVector<int>
     *  USERS    ~
     * */

    PokemonDataProcess();
    void sendData(PokemonData* data);
    void receiveDataServer(QUdpSocket* receiveServer,PokemonServer* server);
   // void receiveDataClient(QUdpSocket *receiver, PokemonPlayer* player);
    void listenData(QUdpSocket** listener,PokemonPlayer*player,int port);

    void setWindow(Window_SignUp *value);

private:
    QHostAddress HOST_ADDRESS;
    const int HOST_PORT=8090;
    QUdpSocket* sendSocket;
    Window_SignUp* window;
    bool isRealPoke;
    bool canUpdate;
    bool isUser;

};

#endif // POKEMONDATAPROCESS_H
