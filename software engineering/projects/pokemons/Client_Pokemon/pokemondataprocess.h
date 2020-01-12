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
#include"playui.h"

class Window_SignUp;
class PokemonServer;
class PokemonPlayer;
class PlayUi;

class PokemonDataProcess:public QUdpSocket
{
 //   Q_OBJECT
public:
     static QString DEFAULT_ADDRESS;
     static int DEFAULT_HOST_PORT;
     enum Operation{SIGNOUT,SIGNIN,SIGNUP,OTHERS,OTHERS_SINGLE,SYSTEM_POKEMONS};
    //symbol for type 1, 2, 3, 4, 5, 6

    PokemonDataProcess();
    void sendData(PokemonData* data);
  //  void receiveDataServer(QUdpSocket* receiveServer,PokemonServer* server);
    void receiveDataClient(QUdpSocket *receiver);
    void listenData(QUdpSocket* listener,PokemonPlayer*player,int port);

    void setSignUpWindow(Window_SignUp *value);
    void setPlayUiWindow(PlayUi* play);
    void requirePokemons(QString username);
    void requireSystemPokes();
    void requireSysAward(int flag);
    void updateRemotePoke(RealPokemon* realPoke);
    void deleteRemotePoke(RealPokemon* realPoke);
    void updateRemoteUser(PokePlayer* player);


    void sendSignoutInfo();
    QString getDEFAULT_ADDRESS() const;

    void setLastOp(const Operation &value);

    void setCurPlayer(PokePlayer *value);
    void clearData();
public slots:
    void changeToOthers(QModelIndex);
private:
    QHostAddress HOST_ADDRESS;
    const int HOST_PORT=8090;
    short  pokemonNum;
    QUdpSocket* sendSocket;
    vector<RealPokemon> curPlayerPokes;
    vector<RealPokemon> otherSinglePokes;
    vector<RealPokemon> systemPokes;
    vector<PokePlayer> otherPlayers;
    map<string,vector<RealPokemon>> otherPokes;
    PokePlayer* curPlayer;
    int opProgress;
    int receiveNum;


    Window_SignUp* signWindow;
    PlayUi* playWindow;
    Operation lastOp;

};

#endif // POKEMONDATAPROCESS_H
