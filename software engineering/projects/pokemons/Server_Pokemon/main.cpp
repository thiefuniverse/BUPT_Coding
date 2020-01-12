#include "pokemon.h"
#include "PokemonServer.h"
#include <QApplication>
#include"window_signup.h"
#include"mainwindow.h"
void testPokemon();
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
  // Window_SignUp w;
  //w.show();
    MainWindow mains;
    mains.show();
    PokemonServer server;
    server.connectAllPokemon();

    //testPokemon();
  //  w.show();
    return a.exec();
}

void testPokemon()
{

    PokemonPower thief("thief",1,"Pika",99,15,8,500,25);
    PokemonPower huang("huang",1,"Qite",33,13,5,500,24);
    thief.printBasicInfo();
    huang.printBasicInfo();

    thief.attack(&huang);
    PokemonServer db;
    db.connectAllPokemon();
}
