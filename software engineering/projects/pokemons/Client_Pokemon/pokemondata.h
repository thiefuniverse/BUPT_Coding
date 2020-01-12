#ifndef POKEMONDATA_H
#define POKEMONDATA_H
#include<QByteArray>
#include<QString>
#include<QHostAddress>
#include<QDataStream>
#include"pokemon.h"
//#include "orm_mysql.h"
using namespace std;
using namespace PokeMon;

class PokemonData
{

public:
    PokemonData();
    PokemonData(QByteArray& initData);

    //define different data type sent
    enum DATA_TYPE{ACK=1,SIGN_IN,SIGN_UP,SIGN_OUT,POKEMON,USERS,SYSTEM_POKES,
                  ADD_SYSTEM,UPDATE_POKE,DELETE_POKE,UPDATE_USER};
    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    virtual QByteArray getSendData();


    QHostAddress getAddress() const;
    void setAddress(const QString &value);

    int getPort() const;
    void setPort(int value);

    PokemonData::DATA_TYPE getType() const;
    void setType(PokemonData::DATA_TYPE value);

    bool isSuccess() const;
    void setSuccess(bool value);

    int getListenPort() const;
    void setListenPort(int value);

    short getAck_number() const;

    void setAck_number(short value);

protected:
    int type;
private:
    QString username;
    QString password;
    QHostAddress address;
    int port;
    int listenPort;
    bool success;

    /* ack_number info
     * 1    sign up success
     * 2    sign up failed
     * 3    sign in success
     * 4    sign in failed
     * 5    sign in
    */
    short ack_number;
    static QMap<int,DATA_TYPE> typeCast;

};

class PokePlayer:public PokemonData
{
   // ORMAP_MYSQL(PokemonPlayers,name,password,state)
public:
    PokePlayer(string _name,string _password)
    :name(_name),password(_password),state(0)
    {}
    PokePlayer(string _name,int state,int pokemonsNum,float winRate,string ip)
    :name(_name),state(state),pokemonsNum(pokemonsNum),winRate(winRate),ip(ip)
    {}

    PokePlayer(QByteArray& initData);
    string name;
    float winRate;
    string password;
    int state;
    int pokemonsNum;
    string ip;
    QByteArray getSendData();

    int getPokemonsNum() const;
    string getName() const;
    void setWinRate(float value);
};
class PokemonStandard
{
    //  ORMAP_MYSQL(AllPokemons,roleName,skill,attribute,description,imagePath)
    public:
    PokemonStandard(string roleName,string skill,string attribute,string description,string imagePath)
    :roleName(roleName),skill(skill),attribute(attribute),description(description),imagePath(imagePath)
    {}
    string roleName;
    string skill;
    string attribute;
    string description;
    string imagePath;
};

class RealPokemon:public PokemonData
{
   // ORMAP_MYSQL(RealPokemons,name_for_user,owner,level,attribute,skill,roleName,description,ep,attackP,defendP,blood,speed)
    public:
        RealPokemon( int _id,string _name_for_user,string _owner,int _level,string _attribute,string _skill,string _roleName,
                      string _description,int _ep,int _attackP,int _defendP,int _blood,int _speed)
      :id(_id),name_for_user(_name_for_user),owner(_owner),level(_level),attribute(_attribute),skill(_skill),
        roleName(_roleName),description(_description),ep(_ep),attackP(_attackP),defendP(_defendP),blood(_blood),speed(_speed)
        {}

       RealPokemon(QByteArray& initData);
       void update(PokemonBaby* baby);
       int id;
       string name_for_user;
       string owner;
       int level;
       string attribute;
       string skill;
       string roleName;
       string description;
       int ep;
       int attackP;
       int defendP;
       int blood;
       int speed;
       QByteArray getSendData();
       int getId() const;
       void setId(int value);
       string getRoleName() const;
};

#endif // POKEMONDATA_H
