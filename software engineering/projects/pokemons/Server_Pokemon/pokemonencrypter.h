#ifndef POKEMONENCRYPTER_H
#define POKEMONENCRYPTER_H
#include<QString>
#include<QByteArray>
#include<QCryptographicHash>



//give help for encryption
class PokemonEncrypter
{
public:
    static PokemonEncrypter* getEncrypter();
    //return encrypted QString for item
    static QString encryptItem(QByteArray item);
private:
    PokemonEncrypter();
    static PokemonEncrypter* singleEncrypter;
};

#endif // POKEMONENCRYPTER_H
