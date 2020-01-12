//
// Created by thief on 16-10-4.
//


#ifndef POKEMON_OF_THIEF_POKEMONSERVER_H
#define POKEMON_OF_THIEF_POKEMONSERVER_H

#include <vector>
#include "pokemon.h"
#include "player.h"
#include"pokemondataprocess.h"
#include"pokemondata.h"
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include<QUdpSocket>
#include<QByteArray>
#include<QDataStream>
#include<QDebug>

using namespace std;
using namespace PokeMon;

class PokemonDataProcess;

class PokemonServer:public QObject
{
    Q_OBJECT
public:
    static int DEFAULT_PORT;
    static QString DEFAULT_ADDRESS;
    PokemonServer();
    bool connectAllPokemon();
    PokemonBaby* generatePokemon();

    bool signUpUser(PokemonData* userInfo);
    bool isUserExisted(const QString& user);
    bool isSignInSuccess(PokemonData* userInfo);

    void sendReply(PokemonData* reply);
public slots:
    void readPendingDatagram();
private:
     int state;
    void initSocket();

    vector<PokemonBaby* > serverPokemons;
    QSqlDatabase pokemonDatabase;
    QUdpSocket* receiveServer;
    PokemonDataProcess* dataProcesser;

};
#endif //POKEMON_OF_THIEF_POKEMONSERVER_H
