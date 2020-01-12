#include "pokemondata.h"

PokemonData::PokemonData()
{

}

PokemonData::PokemonData(QByteArray& initData)
{
    QDataStream seprateData(&initData, QIODevice::ReadOnly);

    seprateData.setVersion(QDataStream::Qt_5_7);
    seprateData >> ack_number>>type>>username>>password>>success>>listenPort;
}

QByteArray PokemonData::getSendData()
{
    QByteArray dataCollection;
    QDataStream dataCollectStream(&dataCollection,QIODevice::WriteOnly);

    dataCollectStream<< ack_number<<type<<username<<password<<success<<listenPort;

    // add some other info

    return dataCollection;
}

QHostAddress PokemonData::getAddress() const
{
    return address;
}

void PokemonData::setAddress(const QString &value)
{
    address=value;
}

int PokemonData::getPort() const
{
    return port;
}

void PokemonData::setPort(int value)
{
    port = value;
}

PokemonData::DATA_TYPE PokemonData::getType() const
{
    return typeCast[type];
}

void PokemonData::setType(PokemonData::DATA_TYPE value)
{
    type = int(value);
}

bool PokemonData::isSuccess() const
{
    return success;
}

void PokemonData::setSuccess(bool value)
{
    success = value;
}

int PokemonData::getListenPort() const
{
    return listenPort;
}

void PokemonData::setListenPort(int value)
{
    listenPort = value;
}

short PokemonData::getAck_number() const
{
    return ack_number;
}

void PokemonData::setAck_number(short value)
{
    ack_number = value;
}


QString PokemonData::getUsername() const
{
    return username;
}

void PokemonData::setUsername(const QString &value)
{
    username = value;
}

QString PokemonData::getPassword() const
{
    return password;
}

void PokemonData::setPassword(const QString &value)
{
    password = value;
}
PokePlayer::PokePlayer(QByteArray &initData)
{
    QDataStream seprateData(&initData, QIODevice::ReadOnly);

    seprateData.setVersion(QDataStream::Qt_5_7);

    QString _name;
    QString _ip;
    seprateData>>type>>_ip>>winRate>> _name>>state>>pokemonsNum;
    ip=_ip.toStdString();
    name=_name.toStdString();
}

void PokePlayer::setWinRate(float value)
{
    winRate = value;
}

string PokePlayer::getName() const
{
    return name;
}

int PokePlayer::getPokemonsNum() const
{
    return pokemonsNum;
}

QByteArray PokePlayer::getSendData()
{

    QByteArray dataCollection;
    QDataStream dataCollectStream(&dataCollection,QIODevice::WriteOnly);

    dataCollectStream<<type<<QString::fromStdString(ip)<<winRate<<QString::fromStdString(name)<<state<<pokemonsNum;
    return dataCollection;
}

RealPokemon::RealPokemon(QByteArray &initData)
{
    QDataStream seprateData(&initData, QIODevice::ReadOnly);

    seprateData.setVersion(QDataStream::Qt_5_7);
    QString _name_for_user;
    QString _owner;
    QString _attribute;
    QString _skill;
    QString _roleName;
    QString _description;

    seprateData>>id>>type>>_name_for_user>>_owner>>level
            >>_attribute>>_skill>>_roleName>>_description
             >>ep>>attackP>>defendP>>blood>>speed;
    name_for_user=_name_for_user.toStdString();
    owner=_owner.toStdString();
    attribute=_attribute.toStdString();
    skill=_skill.toStdString();
    roleName=_roleName.toStdString();
    description=_description.toStdString();

}

void RealPokemon::update(PokemonBaby *baby)
{
    name_for_user="";
    level=baby->getLevel();
    ep=baby->getEp();
    attackP=baby->getAttackPower();
    defendP=baby->getDefendPower();
    blood=baby->getBlood();
    speed=baby->getSpeed();
}

string RealPokemon::getRoleName() const
{
    return roleName;
}

int RealPokemon::getId() const
{
    return id;
}

void RealPokemon::setId(int value)
{
    id = value;
}

QByteArray RealPokemon::getSendData()
{

    QByteArray dataCollection;
    QDataStream dataCollectStream(&dataCollection,QIODevice::WriteOnly);

    dataCollectStream<<id<<type<<QString::fromStdString(name_for_user)<<QString::fromStdString(owner)
                    <<level<<QString::fromStdString(attribute)<<QString::fromStdString(skill)
                   <<QString::fromStdString(roleName)<<QString::fromStdString(description)
                  <<ep<<attackP<<defendP<<blood<<speed;


    return dataCollection;
}




QMap<int,PokemonData::DATA_TYPE> PokemonData:: typeCast={{1,PokemonData::DATA_TYPE::ACK},
                                                         {2,PokemonData::DATA_TYPE::SIGN_IN},
                                                         {3,PokemonData::DATA_TYPE::SIGN_UP},
                                                         {4,PokemonData::DATA_TYPE::SIGN_OUT},
                                                         {5,PokemonData::DATA_TYPE::POKEMON},
                                                         {6,PokemonData::DATA_TYPE::USERS},
                                                         {7,PokemonData::DATA_TYPE::SYSTEM_POKES},
                                                         {8,PokemonData::DATA_TYPE::ADD_SYSTEM},
                                                         {9,PokemonData::DATA_TYPE::UPDATE_POKE},
                                                         {10,PokemonData::DATA_TYPE::DELETE_POKE},
                                                         {11,PokemonData::DATA_TYPE::UPDATE_USER}};
