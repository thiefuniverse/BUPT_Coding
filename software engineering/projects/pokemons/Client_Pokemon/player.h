//
// Created by thief on 16-10-4.
//


#ifndef POKEMON_OF_THIEF_PLAYER_H
#define POKEMON_OF_THIEF_PLAYER_H

#include <string>
#include <QVector>
#include "pokemon.h"
//#include"pokemondataprocess.h"
#include"pokemondata.h"
using namespace PokeMon;
using namespace std;

class PokemonPlayer
{
public:
    PokemonPlayer(const string _name);
    ~PokemonPlayer();

    int getListenPort() const;
    void setListenPort(int value);

    string getName() const;
    void setName(const string &value);

private:
    string name;
    vector<PokemonBaby*> allPokemon;
    float winRate;
    vector<short> badges;

    int listenPort;
};
#endif //POKEMON_OF_THIEF_PLAYER_H
