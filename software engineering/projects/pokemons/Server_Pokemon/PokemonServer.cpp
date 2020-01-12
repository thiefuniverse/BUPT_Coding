//
// Created by thief on 16-10-4.
//
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <QtDebug>
#include <QSqlError>
#include "PokemonServer.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include<ctime>

PokemonServer::PokemonServer() {
    initSocket();

}

PokemonServer::~PokemonServer()
{
    players_mapper->query(*mapperPlayerHelper).update(Exp("state")=0);
}

bool PokemonServer::connectAllPokemon() {
    players_mapper=new ORMapper("localhost","thief","ssopq","Pokemon",0);
    //initialize helpers
    mapperPlayerHelper=new PokePlayer("help","help");
    mapperStandardHelper=new PokemonStandard("help","help","help","help","help");
    mapperRealPokeHelper=new RealPokemon(1,"help","help",1,"help","help","help","help",1,1,1,1,1);

    srand(NULL);
    createSystemPokes();
    //players_mapper->createTbl(*mapperPlayerHelper);

   // players_mapper->insert(*mapperRealPokeHelper);

 //players_mapper->dropTbl(*mapperRealPokeHelper);
 // players_mapper->createTbl(*mapperRealPokeHelper,1,"id");
    //players_mapper->dropTbl(*mapperStandardHelper);
    //players_mapper->createTbl(*mapperStandardHelper);
   // this->loadFileToDatabase("standardPokemons_config.txt");
// players_mapper->dropTbl(*mapperPlayerHelper);

   // players_mapper->insert(helper);
    //players_mapper->insert(saver);
    //auto result=players_mapper->select(*mapperPlayerHelper).toVector();
    //for(auto a:result)
    {
     //   string name=a[0];
     //   float winRate=a[1];
       // qDebug()<<QString::fromStdString(name)<<" "<<winRate;
    }
    return true;

}

vector<PokePlayer> PokemonServer::getAllPlayers()
{
    auto allPlayers=players_mapper->select(*mapperPlayerHelper,Exp("name,state,pokemonsNum,winRate,ip")).toVector();

    vector<PokePlayer> allPokePlayers;

    for(auto onePoke:allPlayers)
    {
       PokePlayer tempPlayer(onePoke[0],onePoke[1],onePoke[2],onePoke[3],onePoke[4]);
       allPokePlayers.push_back(tempPlayer);
    }
    return std::move(allPokePlayers);

}

PokePlayer PokemonServer::getOnePlayer(PokemonData *userInfo)
{
    auto curPlayer=players_mapper->select(*mapperPlayerHelper,Exp("name,state,pokemonsNum,winRate,ip")).query()
            .where(Exp("name")==userInfo->getUsername().toStdString())
            .toVector();

    if (curPlayer.empty())
    {
        //return NULL;
    }
    else
    {
        PokePlayer tempPlayer(curPlayer[0][0],curPlayer[0][1],curPlayer[0][2],curPlayer[0][3],curPlayer[0][4]);
        return std::move(tempPlayer);
    }
}

// get n(3) realPokemon for user when sign up
void PokemonServer::getNRealPokemon(PokemonData* userInfo,int num=3)
{

    int allNum=players_mapper->query(*mapperStandardHelper).count();
    for(int i=0;i<3;++i)
    {
         auto realPokes=players_mapper->query(*mapperStandardHelper).limit(ranNum(allNum),1).toVector();
         RealPokemon tempPokemon(realPokes[0][0],userInfo->getUsername().toStdString(),1,realPokes[0][2],
                 realPokes[0][1],realPokes[0][0],realPokes[0][3],
                 1,10+ranNum(20),10+ranNum(20),
                100+ranNum(20),5+ranNum(20));
        players_mapper->insert(tempPokemon);

    }
}


vector<RealPokemon> PokemonServer::getUserPokemon(PokemonData *userInfo)
{
     auto realPokes=players_mapper->query(*mapperRealPokeHelper).where(Exp("owner")==userInfo->getUsername().toStdString())
             .toVector();
     // need id to ORMYSQL
     auto allIds=players_mapper->select(*mapperRealPokeHelper,Exp("id")).query().where(Exp("owner")==userInfo->getUsername().toStdString())
             .toVector();
     vector<RealPokemon> nRealPokemon;
     int flag=0;
     for(auto poke:realPokes)
     {
         RealPokemon tempPokemon(allIds[flag][0],poke[0],poke[1],poke[2],poke[3],poke[4],poke[5],poke[6],poke[7],poke[8],poke[9],poke[10],poke[11]);
       // players_mapper->insert(tempPokemon);
         nRealPokemon.push_back(tempPokemon);
         flag++;
     }
     return std::move(nRealPokemon);
}

void PokemonServer::addSysAwardToUser(PokemonData *userInfo)
{
    RealPokemon *real=serverPokemons[userInfo->getAck_number()];
    real->setUsername(userInfo->getUsername());
    players_mapper->insert(*real);
}

void PokemonServer::updateUserPoke(RealPokemon *realPoke)
{
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("level")==realPoke->level);
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("ep")==realPoke->ep);
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("attackP")==realPoke->attackP);
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("defendP")==realPoke->defendP);
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("blood")==realPoke->blood);
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==realPoke->id).update(Exp("speed")==realPoke->speed);
}

void PokemonServer::updateUser(PokePlayer *player)
{
    players_mapper->update(*player);
}

void PokemonServer::deletePoke(RealPokemon *delPoke)
{
    players_mapper->query(*mapperRealPokeHelper).where(Exp("id")==delPoke->getAck_number()).del();
    auto nums=players_mapper->select(*mapperPlayerHelper,Exp("pokemonsNum")).query().where(Exp("name")==delPoke->getUsername().toStdString()).toVector();
    int realNum=nums[0][0];
    realNum--;
    players_mapper->query(*mapperPlayerHelper).where(Exp("name")==delPoke->getUsername().toStdString()).update(Exp("pokemonsNum")=realNum);

}

void PokemonServer::createSystemPokes()
{
    int allNum=players_mapper->query(*mapperStandardHelper).count();
    int range=20;
    int levelChange=0;
    int staticChange=50;
    for(int i=0;i<SYSTEM_POKES_NUM;++i)
    {
        levelChange+=3;

        auto realPokes=players_mapper->query(*mapperStandardHelper).limit(ranNum(allNum),1).toVector();
         RealPokemon* tempPokemon=new RealPokemon(1,realPokes[0][0],"system",levelChange,realPokes[0][2]
                 ,realPokes[0][1],realPokes[0][0],realPokes[0][3]
                 ,levelChange*40+ranNum(range)+staticChange,10+ranNum(range)+staticChange,10+ranNum(range)+staticChange,
                100+ranNum(range)+staticChange,5+ranNum(range)+staticChange);
        staticChange+=50;
        serverPokemons.push_back(tempPokemon);

    }
}

void PokemonServer::sendAllUsers(QString senderHost,int port)
{
    auto allPokeplayers=getAllPlayers();
    for(auto one:allPokeplayers)
    {
        one.setType(PokemonData::USERS);
        one.setAddress(senderHost);
        one.setPort(port);
        this->sendReply(&one);
    }
}



bool PokemonServer::signUpUser(PokemonData* userInfo)
{
   state=0;
   if(!isUserExisted(userInfo->getUsername()))
   {
       PokePlayer newPlayer(userInfo->getUsername().toStdString(),
                                                userInfo->getPassword().toStdString());
       newPlayer.pokemonsNum=3;
       newPlayer.winRate=1.0;
       players_mapper->insert(newPlayer);
       this->getNRealPokemon(userInfo);
       return true;
   }
   else
   {
       qDebug()<<userInfo->getUsername()<<"is existed........";
       return false;
   }
}

bool PokemonServer::signOutUser(PokemonData *userInfo)
{
    players_mapper->query(*mapperPlayerHelper).where(Exp("name")==userInfo->getUsername().toStdString()).update(Exp("state")=0);
    return true;
}

bool PokemonServer::isUserExisted(const QString& user)
{
    return players_mapper->select(*mapperPlayerHelper,Exp("name")).query().where(Exp("name")==user.toStdString()).count()!=0;
}

bool PokemonServer::isOnline(PokemonData *userInfo)
{
     return players_mapper->select(*mapperPlayerHelper,Exp("name")).query().
             where(Exp("name")==userInfo->getUsername().
                   toStdString() && Exp("state")==1).count()!=0;
}

bool PokemonServer::isSignInSuccess(PokemonData* userInfo)
{
        //judge current state
        auto currentState=players_mapper->select(*mapperPlayerHelper,Exp("state")).query()
                .where(Exp("name")==userInfo->getUsername()
                .toStdString()).toVector();
        int state;
        if(currentState.empty())
        {
            return false;
        }
        else
        {
           state=currentState[0][0];
        }

        // if this player offline, judge password is right or not
        if (state==0)
        {
            auto queryResult=players_mapper->select(*mapperPlayerHelper,Exp("password")).query().where(Exp("name")==userInfo->getUsername().toStdString())
                    .toVector();
            if (queryResult.empty())
            {
                return false;
            }

            string password=queryResult[0][0];

            if (password==userInfo->getPassword().toStdString())
            {
                players_mapper->query(*mapperPlayerHelper)
                        .where(Exp("name")==userInfo->getUsername().toStdString())
                        .update(Exp("state")=1);
                return true;
            }
        }
        else
        {
            return false;
        }
        // return queryTool.value(0).toString()==userInfo->getPassword();
}

void PokemonServer::loadFileToDatabase(string dataFile)
{
    fstream reader;
    reader.open(dataFile.c_str());
    if(!reader.is_open())
    {
        qDebug()<<"can't open file ";
        exit(1);
    }
    string buffer;
    getline(reader,buffer);
    while (getline(reader,buffer))
    {
        vector<string> infos;
        string tempString;
        int flag=0;
        while(flag<buffer.size())
        {
            if(buffer[flag]!=';')
            {
                tempString+=buffer[flag];
            }
            else
            {
                infos.push_back(tempString);
                tempString.clear();
            }
            flag++;
        }
        PokemonStandard stand(infos[0],infos[1],infos[2],infos[3],infos[4]);
        players_mapper->insert(stand);
    }
}

void PokemonServer::sendReply(PokemonData *reply)
{
    dataProcesser->sendData(reply);
}

void PokemonServer::readPendingDatagram()
{
    dataProcesser->receiveDataServer(receiveServer,this);
}

int PokemonServer::ranNum(int moder)
{
    return rand()%moder;
}

void PokemonServer::initSocket()
{
    dataProcesser=new PokemonDataProcess();
    dataProcesser->listenData(&receiveServer,NULL,DEFAULT_PORT);  // bind a port for listening

    connect(receiveServer,SIGNAL(readyRead()),this,SLOT(readPendingDatagram()));
}

vector<RealPokemon *> PokemonServer::getServerPokemons() const
{
    return serverPokemons;
}

int PokemonServer::DEFAULT_PORT=8090;
QString PokemonServer::DEFAULT_ADDRESS="127.0.0.1";
int PokemonServer::SYSTEM_POKES_NUM=4;
