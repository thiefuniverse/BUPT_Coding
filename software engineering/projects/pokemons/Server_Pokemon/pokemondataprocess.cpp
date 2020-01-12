#include "pokemondataprocess.h"


PokemonDataProcess::PokemonDataProcess()
{
    isRealPoke=false;
    canUpdate=false;
    isUser=false;
}

void PokemonDataProcess::receiveDataServer(QUdpSocket *receiveServer, PokemonServer *server)
{
    while (receiveServer->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(receiveServer->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            receiveServer->readDatagram(datagram.data(), datagram.size(),
                                     &sender, &senderPort);
            PokemonData *receiveData;
            if(isRealPoke)
            {
                receiveData=new RealPokemon(datagram);

            }
            else if(isUser)
            {
                 receiveData=new PokePlayer(datagram);
            }
            else
            {
                 receiveData=new PokemonData(datagram);
            }
            PokemonData* replyData;

            PokemonData::DATA_TYPE type=receiveData->getType();
          // switch (type)
          //  {
          //  case PokemonData::USERS:
            if (type==PokemonData::USERS){
               // qDebug()<<"i will offer all users";
                replyData=new PokemonData();
                replyData->setAck_number(server->getAllPlayers().size());
                replyData->setAddress(sender.toString());
                replyData->setPort(receiveData->getListenPort());
                replyData->setType(PokemonData::ACK);
                server->sendReply(replyData);
                server->sendAllUsers(sender.toString(),receiveData->getListenPort());
                delete replyData;
                //break;
            }
          //  case PokemonData::SIGN_IN:
            else if(type==PokemonData::SIGN_IN){
                replyData=new PokemonData();
                replyData->setType(PokemonData::ACK);
                bool signInSuccess=server->isSignInSuccess(receiveData);
                if(signInSuccess)
                {
                    qDebug()<<receiveData->getUsername()+" sign in successfully.......";
                    replyData->setSuccess(true);
                }
                else
                {
                    qDebug()<<receiveData->getUsername()+" sign in failed ........";
                    replyData->setSuccess(false);

                }
                //replyData->setAck_number(3);
                replyData->setAddress(sender.toString());
                replyData->setPort(receiveData->getListenPort());

                // send ack
                server->sendReply(replyData);

                if(signInSuccess)
                {
                    qDebug()<<"send cur player";
                    // send curPlayer
                    PokePlayer curPokeplayer=server->getOnePlayer(receiveData);
                    curPokeplayer.setType(PokemonData::USERS);
                    curPokeplayer.setAddress(sender.toString());
                    curPokeplayer.setPort(receiveData->getListenPort());
                    server->sendReply(&curPokeplayer);

                     // send current player pokemons
                    auto allPokemons=server->getUserPokemon(receiveData);
                    for(auto poke:allPokemons)
                    {
                        qDebug()<<"send cur player pokemon";
                        poke.setAddress(sender.toString());
                        poke.setPort(receiveData->getListenPort());
                        poke.setType(PokemonData::POKEMON);
                        server->sendReply(&poke);
                    }
                   // server->sendReply(replyData);
                }
                delete replyData;
            }
               // break;
            else if(type==PokemonData::SIGN_UP){
           // case PokemonData::ACK:

          //      break;
           // case PokemonData::SIGN_UP:
                replyData=new PokemonData();
                replyData->setType(PokemonData::ACK);
                if(server->signUpUser(receiveData))
                {
                    qDebug()<<receiveData->getUsername()+" sign up successfully";
                    replyData->setSuccess(true);
                }
                else
                {
                    qDebug()<<receiveData->getUsername()+" sign up failed .......";
                     replyData->setSuccess(false);
                }
                replyData->setAddress(sender.toString());
                replyData->setPort(receiveData->getListenPort());
                server->sendReply(replyData);
             //   break;
                delete replyData;
}
            else if(type==PokemonData::POKEMON){
                // send current player pokemons
               auto allPokes=server->getUserPokemon(receiveData);
               replyData=new PokemonData();
               replyData->setType(PokemonData::ACK);
               replyData->setAck_number(allPokes.size());
               replyData->setAddress(sender.toString());
               replyData->setPort(receiveData->getListenPort());
               if(server->isOnline(receiveData))
               {
                   replyData->setSuccess(true);
               }
               else
               {
                   replyData->setSuccess(false);
               }

               server->sendReply(replyData);
               qDebug()<<"send ing allPokes";
               for(auto poke:allPokes)
               {
                   qDebug()<<"send cur player pokemon";
                   poke.setAddress(sender.toString());
                   poke.setPort(receiveData->getListenPort());
                   poke.setType(PokemonData::POKEMON);
                   server->sendReply(&poke);
               }
        }
            else if(type==PokemonData::SIGN_OUT)
            {
                server->signOutUser(receiveData);
                replyData=new PokemonData();
                replyData->setAddress(sender.toString());
                replyData->setPort(receiveData->getListenPort());
                replyData->setType(PokemonData::ACK);
                server->sendReply(replyData);
                delete replyData;
            }
            else if(type==PokemonData::SYSTEM_POKES)
            {
                // send current system pokemons
               replyData=new PokemonData();
               replyData->setType(PokemonData::ACK);
               replyData->setAck_number(PokemonServer::SYSTEM_POKES_NUM);
               replyData->setAddress(sender.toString());
               replyData->setPort(receiveData->getListenPort());
               server->sendReply(replyData);
               qDebug()<<"send ing system  allPokes";
               auto systemPokes=server->getServerPokemons();
               for(auto poke:systemPokes)
               {
                   qDebug()<<"send one system pokemon";
                   poke->setAddress(sender.toString());
                   poke->setPort(receiveData->getListenPort());
                   poke->setType(PokemonData::POKEMON);
                   server->sendReply(poke);
               }
            }
            else if(type==PokemonData::ADD_SYSTEM)
            {
                qDebug()<<"add one pokemon of "<<receiveData->getUsername();
                server->addSysAwardToUser(receiveData);
            }
            else if(type==PokemonData::UPDATE_POKE)
            {
                isRealPoke=true;
                if(canUpdate)
                {
                    qDebug()<<"update one pokemon of "<<receiveData->getUsername();
                    server->updateUserPoke(receiveData);
                    canUpdate=false;
                    isRealPoke=false;
                }

                if(isRealPoke)
                canUpdate=true;
            }
            else if(type==PokemonData::DELETE_POKE)
            {
                qDebug()<<"delete poke";
                server->deletePoke(receiveData);
            }
            else if(type==PokemonData::UPDATE_USER)
            {

                isUser=true;
                if(canUpdate)
                {
                    qDebug()<<"update user";
                    server->updateUser(receiveData);
                    canUpdate=false;
                    isUser=false;
                }
                if (isUser)
                    canUpdate=true;
            }

    }
}

void PokemonDataProcess::sendData(PokemonData* data)
{
    sendSocket=new QUdpSocket(this);
   // sendSocket->writeDatagram(data->getSendData(),data->getAddress(),data->getPort());
    sendSocket->writeDatagram(data->getSendData(),data->getAddress(),data->getPort());
}

void PokemonDataProcess::listenData(QUdpSocket** listener,PokemonPlayer*player,int port)
{
    (*listener)=new QUdpSocket();
    if(port!=PokemonServer::DEFAULT_PORT)
    {
        int realPort=9000+rand()%100;
        while(!(*listener)->bind(realPort))
        {
            realPort++;
        }
        if(player!=NULL)
            player->setListenPort(realPort);
        qDebug()<<"listen port realPort"<<realPort;
    }
    else
    {
        if(!(*listener)->bind(port))
        {
            //error
            qDebug()<<"error in listener bind function";
           // return NULL;
        }

    }
}

void PokemonDataProcess::setWindow(Window_SignUp *value)
{
    window = value;
}

