#include "pokemon.h"
//#include "PokemonServer.h"
#include <QApplication>
#include"window_signup.h"
//#include"test.h"
#include"playui.h"
#include"pokemonclient.h"
#include"fightwindow.h"

void testPokemon();
int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
   //Window_SignUp w(NULL);
   // w.show();
   // PlayUi testa;
  //  testa.show();
    //w.hide();
   // PlayUi r;
    PokemonClient client;
    client.showSignUp();
    //client.setMainProgram(&a);
  // FightWindow win;
   // win.show();



   // r.show();
   // PokemonServer server;
  //  server.connectAllPokemon();
 //   testPokemon();
    return a.exec();
}

void testPokemon()
{

    PokemonPower thief("thief",1,"Pika",99,15,8,500,25);
    PokemonPower huang("huang",1,"Qite",33,13,5,500,24);
    thief.printBasicInfo();
    huang.printBasicInfo();

   // thief.attack(&huang);
   // PokemonServer db;
   // db.connectAllPokemon();
}
