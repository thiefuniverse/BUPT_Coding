//
// Created by thief on 16-10-4.
//
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <QtDebug>
#include <QSqlError>
#include "PokemonServer.h"

PokemonServer::PokemonServer() {
    initSocket();
}

bool PokemonServer::connectAllPokemon() {
    pokemonDatabase=QSqlDatabase::addDatabase("QMYSQL");
    pokemonDatabase.setHostName("localhost");
    pokemonDatabase.setDatabaseName("Pokemon");

    //todo  later let manager input password
    pokemonDatabase.setUserName("root");
    pokemonDatabase.setPassword("ssopq");

    if(!pokemonDatabase.open())
    {
        std::cout<<"fuck";
        std::cout<<pokemonDatabase.lastError().text().toStdString();
        return false;
    }
    return true;

}

bool PokemonServer::signUpUser(PokemonData* userInfo)
{
   state=0;
   if(!isUserExisted(userInfo->getUsername()))
   {
       QSqlQuery queryTool;
       QString command="insert into PokemonUsers(UserName,Password,State) Values (\""+
               userInfo->getUsername()+"\",\""+userInfo->getPassword()+"\","+QString::number(state)+")";
       return queryTool.exec(command);
   }
   else
   {
       qDebug()<<userInfo->getUsername()<<"is existed........";
       return false;
   }
}

bool PokemonServer::isUserExisted(const QString& user)
{
    QSqlQuery queryTool;
    queryTool.exec("SELECT UserName from PokemonUsers where UserName=\""+user+"\"");
    return queryTool.next();
}

bool PokemonServer::isSignInSuccess(PokemonData* userInfo)
{
    QSqlQuery queryTool;
    queryTool.exec("SELECT Password from PokemonUsers where UserName=\""+userInfo->getUsername()+"\"");
    queryTool.next();
    return queryTool.value(0).toString()==userInfo->getPassword();
}

void PokemonServer::sendReply(PokemonData *reply)
{
    dataProcesser->sendData(reply);
}

void PokemonServer::readPendingDatagram()
{
    dataProcesser->receiveDataServer(receiveServer,this);
}

void PokemonServer::initSocket()
{
    dataProcesser=new PokemonDataProcess();
    dataProcesser->listenData(&receiveServer,NULL,DEFAULT_PORT);  // bind a port for listening

    connect(receiveServer,SIGNAL(readyRead()),this,SLOT(readPendingDatagram()));
}

int PokemonServer::DEFAULT_PORT=8090;
QString PokemonServer::DEFAULT_ADDRESS="127.0.0.1";
