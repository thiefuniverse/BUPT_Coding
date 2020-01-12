//
// Created by thief on 16-10-4.
//

#include "player.h"

PokemonPlayer::PokemonPlayer(const string _name)
:name(_name)
{

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




