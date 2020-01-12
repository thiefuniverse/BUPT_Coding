//
// Created by thief on 16-10-4.
//

#include "player.h"

PokemonPlayer::PokemonPlayer(const string _name)
:name(_name)
{

}

PokemonPlayer::~PokemonPlayer()
{

}

bool PokemonPlayer::loginIn()
{
    return false;
}

void PokemonPlayer::sendData(QVector<QString> item,PokemonData::DATA_TYPE type)
{
    PokemonDataProcess* sender= new PokemonDataProcess();
   // sender->sendData(item,type);
}

int PokemonPlayer::getListenPort() const
{
    return listenPort;
}

void PokemonPlayer::setListenPort(int value)
{
    listenPort = value;
}

string PokemonPlayer::getName() const
{
    return name;
}

void PokemonPlayer::setName(const string &value)
{
    name = value;
}





