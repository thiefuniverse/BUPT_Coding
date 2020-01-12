#include "pokemonencrypter.h"

PokemonEncrypter::PokemonEncrypter()
{
}

PokemonEncrypter *PokemonEncrypter::getEncrypter()
{
    if(singleEncrypter==NULL)
    {
        singleEncrypter=new PokemonEncrypter();
        return singleEncrypter;
    }
    else
    {
        return singleEncrypter;
    }
}

PokemonEncrypter* PokemonEncrypter::singleEncrypter=NULL;
QString PokemonEncrypter::encryptItem(QByteArray item)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(item);
    return hash.result().toHex();
}
