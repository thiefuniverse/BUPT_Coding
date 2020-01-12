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

#include "orm_mysql.h"

#include<QUdpSocket>
#include<QByteArray>
#include<QDataStream>
#include<QDebug>
using namespace ORM_MYSQL_THIEF;
using namespace std;
using namespace PokeMon;

class PokemonDataProcess;

class PokemonServer:public QObject
{
    Q_OBJECT
public:
    static int DEFAULT_PORT;
    static QString DEFAULT_ADDRESS;
    static int SYSTEM_POKES_NUM;
    PokemonServer();
    ~PokemonServer();
    bool connectAllPokemon();
    PokemonBaby* generatePokemon();

    vector<PokePlayer> getAllPlayers();
    PokePlayer getOnePlayer(PokemonData* userInfo);
    void getNRealPokemon(PokemonData* userInfo,int num);
    vector<RealPokemon> getUserPokemon(PokemonData* userInfo);
    void addSysAwardToUser(PokemonData* userInfo);

    void updateUserPoke(RealPokemon* realPoke);
    void updateUser(PokePlayer* player);
    void deletePoke(RealPokemon* delPoke);
    void createSystemPokes();
    void sendAllUsers(QString senderHost,int port);
    bool signUpUser(PokemonData* userInfo);
    bool signOutUser(PokemonData* userInfo);
    bool isUserExisted(const QString& user);
    bool isOnline(PokemonData* userInfo);
    bool isSignInSuccess(PokemonData* userInfo);

    // help for create database
    void loadFileToDatabase(string fileName);
    void sendReply(PokemonData* reply);
    vector<RealPokemon *> getServerPokemons() const;

public slots:
    void readPendingDatagram();
private:
     int state;
    int ranNum(int moder);
    void initSocket();

    ORMapper *players_mapper;

    vector<RealPokemon*> serverPokemons;
    PokePlayer *mapperPlayerHelper;
    PokemonStandard *mapperStandardHelper;
    RealPokemon *mapperRealPokeHelper;

   // QSqlDatabase pokemonDatabase;
    QUdpSocket* receiveServer;
    PokemonDataProcess* dataProcesser;

};
#endif //POKEMON_OF_THIEF_POKEMONSERVER_H
