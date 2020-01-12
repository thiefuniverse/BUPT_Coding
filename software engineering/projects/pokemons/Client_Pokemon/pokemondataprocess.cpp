#include "pokemondataprocess.h"


QString PokemonDataProcess::getDEFAULT_ADDRESS() const
{
    return DEFAULT_ADDRESS;
}

void PokemonDataProcess::setLastOp(const Operation &value)
{
    switch(value)
    {
    case SIGNIN:
        opProgress=3;
        break;
    case SIGNUP:
        opProgress=1;
        break;
    case OTHERS:
        opProgress=2;
        break;
    case OTHERS_SINGLE:
        opProgress=2;
        break;
    case SYSTEM_POKEMONS:
        opProgress=2;
        break;
    case SIGNOUT:
        opProgress=1;
        break;
    default:
        break;
    }
    lastOp = value;
}

void PokemonDataProcess::setCurPlayer(PokePlayer *value)
{
    curPlayer = value;
}

void PokemonDataProcess::clearData()
{
    otherPlayers.clear();
    curPlayerPokes.clear();
    otherSinglePokes.clear();
}

void PokemonDataProcess::changeToOthers(QModelIndex)
{

}


PokemonDataProcess::PokemonDataProcess()
{
    //pokemonNum=0;
    //  QListView lst=playWindow->getOtherList();
    //connect((playWindow->getUi())->listView_otherName,SIGNAL(clicked(QModelIndex)),this,SLOT(changeToOthers(QModelIndex)));
}

void PokemonDataProcess::receiveDataClient(QUdpSocket *receiver)
{
    // receive data
    while (receiver->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(receiver->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            receiver->readDatagram(datagram.data(), datagram.size(),
                                     &sender, &senderPort);

            PokemonData *receiveData;

            switch (lastOp) {
            case SIGNIN:
                if(opProgress==3)
                {
                    receiveData=new PokemonData(datagram);
                    if(receiveData->isSuccess())
                    {
                        qDebug()<<"sign in receive successfully!!!!";
                        signWindow->signFeedBack(true);
                        signWindow->hide();
                        opProgress--;
                        playWindow->show();
                        pokemonNum=receiveData->getAck_number();
                    }
                    else
                    {
                        signWindow->signFeedBack(false);
                    }

                }
                else if(opProgress==2)
                {
                   curPlayer=new PokePlayer(datagram);
                   playWindow->updatePersonalUser(curPlayer); // update personal info
                   playWindow->setCurrentPlayer(curPlayer);
                   receiveNum=curPlayer->getPokemonsNum();
                   opProgress--;
                }
                else if(opProgress==1)
                {
                    qDebug()<<"get one pokemon";
                    receiveData=new RealPokemon(datagram);
                    curPlayerPokes.push_back(std::move((*(static_cast<RealPokemon*>(receiveData)))));
                    receiveNum--;
                    if(!receiveNum)
                    {
                        opProgress--;
                        playWindow->setCurPlayerPokes(std::move(&curPlayerPokes));
                        PokemonData getOthers;
                        getOthers.setType(PokemonData::DATA_TYPE::USERS);
                        getOthers.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
                        getOthers.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
                        getOthers.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
                        this->setLastOp(PokemonDataProcess::OTHERS);
                        this->sendData(&getOthers);
                    }
                }
                break;
            case SIGNUP:
                 receiveData=new PokemonData(datagram);
                 if(receiveData->isSuccess())
                 {
                     qDebug()<<"sign up receive successfully!!!!";
                     signWindow->signFeedBack(true);
                 }
                 else
                 {
                     signWindow->signFeedBack(false);
                 }
                break;
            case OTHERS:
                if(opProgress==2)
                {
                   receiveData=new PokemonData(datagram);
                   receiveNum=receiveData->getAck_number();
                   // release resource
                   delete receiveData;
                   qDebug()<<" user num   "<<receiveNum;
                   opProgress--;
                }
                else if(opProgress==1)
                {
                    PokePlayer onePoker=PokePlayer(datagram);;
                    otherPlayers.push_back(onePoker);
                    receiveNum--;
                    if(receiveNum<=0)
                    {
                        opProgress--;
                        // get one other single
                        playWindow->updateOtherUsers(otherPlayers);
                       this->requirePokemons(QString::fromStdString(otherPlayers[0].getName()));
                    }
                }
                break;
            case OTHERS_SINGLE:
                if (opProgress==2)
                {
                    otherSinglePokes.clear();
                    receiveData=new PokemonData(datagram);
                    receiveNum=receiveData->getAck_number();
                    playWindow->updateOtherState(receiveData);

                    opProgress--;

                    delete receiveData;
                }
                else if(opProgress==1)
                {
                    receiveData=new RealPokemon(datagram);
                    otherSinglePokes.push_back(std::move((*(static_cast<RealPokemon*>(receiveData)))));
                    receiveNum--;
                    if(receiveNum<=0)
                    {
                        playWindow->setOtherPlayerPokes(&otherSinglePokes);
                        playWindow->emitReceiveSuccess();
                        opProgress--;

                        if(!playWindow->getRequireSys())
                            this->requireSystemPokes();
                    }
                }
                break;
            case SYSTEM_POKEMONS:
                if(opProgress==2)
                {
                    systemPokes.clear();
                    receiveData=new PokemonData(datagram);
                    receiveNum=receiveData->getAck_number();
                    opProgress--;

                    delete receiveData;
                }
                else if(opProgress==1)
                {
                    receiveData=new RealPokemon(datagram);
                    systemPokes.push_back(std::move((*(static_cast<RealPokemon*>(receiveData)))));
                    receiveNum--;
                    if(receiveNum<=0)
                    {
                        playWindow->setSystemPokes(&systemPokes);
                        playWindow->emitReceSystemSuce();
                        opProgress--;
                    }
                }
            case SIGNOUT:
                receiveData=new PokemonData(datagram);
                if(receiveData->isSuccess())
                {
                    //playWindow->close();
                    qDebug()<<"Sign out successfully!";
                }
                break;
            default:
                qDebug()<<"no match";
                break;
            }
    }

}

void PokemonDataProcess::sendData(PokemonData* data)
{
    sendSocket=new QUdpSocket(this);
    sendSocket->writeDatagram(data->getSendData(),data->getAddress(),data->getPort());
}

void PokemonDataProcess::listenData(QUdpSocket* listener,PokemonPlayer*player,int port)
{
   // (*listener)=new QUdpSocket();
    if(port!=PokemonDataProcess::DEFAULT_HOST_PORT)
    {
        int realPort=9000+rand()%100;
        while(!listener->bind(realPort))
        {
            realPort++;
        }
        if(player!=NULL)
            player->setListenPort(realPort);
        qDebug()<<"listen port realPort"<<realPort;
    }
    else
    {
        if(!(listener)->bind(port))
        {
            //error
            qDebug()<<"error in listener bind function";
           // return NULL;
        }

    }
}

void PokemonDataProcess::setSignUpWindow(Window_SignUp *value)
{
    signWindow = value;
}

void PokemonDataProcess::setPlayUiWindow(PlayUi *play)
{
    playWindow=play;
}

void PokemonDataProcess::requirePokemons(QString username)
{
    PokemonData getOthers;
    getOthers.setType(PokemonData::DATA_TYPE::POKEMON);
    getOthers.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    getOthers.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    getOthers.setUsername(username);
    getOthers.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    this->setLastOp(PokemonDataProcess::OTHERS_SINGLE);
    this->sendData(&getOthers);
}

void PokemonDataProcess::requireSystemPokes()
{
    PokemonData getOthers;
    getOthers.setType(PokemonData::DATA_TYPE::SYSTEM_POKES);
    getOthers.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    getOthers.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    getOthers.setUsername(QString::fromStdString(playWindow->getCurrentPlayer()->name));
    getOthers.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    this->setLastOp(PokemonDataProcess::SYSTEM_POKEMONS);
    this->sendData(&getOthers);
}

void PokemonDataProcess::requireSysAward(int flag)
{
    PokemonData getOthers;
    getOthers.setType(PokemonData::DATA_TYPE::ADD_SYSTEM);
    getOthers.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    getOthers.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    getOthers.setAck_number(flag);
    getOthers.setUsername(QString::fromStdString(playWindow->getCurrentPlayer()->name));
    getOthers.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    //this->setLastOp(PokemonDataProcess::SYSTEM_POKEMONS);
    this->sendData(&getOthers);
}

void PokemonDataProcess::updateRemotePoke(RealPokemon *realPoke)
{
    PokemonData updatePoke;
    updatePoke.setType(PokemonData::DATA_TYPE::UPDATE_POKE);
    updatePoke.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    updatePoke.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    updatePoke.setUsername(QString::fromStdString(playWindow->getCurrentPlayer()->name));
    updatePoke.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    //this->setLastOp(PokemonDataP\rocess::SYSTEM_POKEMONS);
    this->sendData(&updatePoke);

    realPoke->setType(PokemonData::DATA_TYPE::UPDATE_POKE);
    realPoke->setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    realPoke->setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    this->sendData(realPoke);
}

void PokemonDataProcess::deleteRemotePoke(RealPokemon *realPoke)
{
    PokemonData deletePoke;
    deletePoke.setType(PokemonData::DATA_TYPE::DELETE_POKE);
    deletePoke.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    deletePoke.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    deletePoke.setUsername(QString::fromStdString(playWindow->getCurrentPlayer()->name));
    deletePoke.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    deletePoke.setAck_number(realPoke->getId());
    //this->setLastOp(PokemonDataProcess::SYSTEM_POKEMONS);
    this->sendData(&deletePoke);
}

void PokemonDataProcess::updateRemoteUser(PokePlayer *player)
{
    PokemonData remotePoke;
    remotePoke.setType(PokemonData::DATA_TYPE::UPDATE_USER);
    remotePoke.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    remotePoke.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    remotePoke.setUsername(QString::fromStdString(playWindow->getCurrentPlayer()->name));
    remotePoke.setListenPort(signWindow->getCurrentPlayer()->getListenPort());

    //this->setLastOp(PokemonDataProcess::SYSTEM_POKEMONS);
    this->sendData(&remotePoke);
    player->setType(PokemonData::DATA_TYPE::UPDATE_USER);
    player->setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    player->setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    this->sendData(player);
}

void PokemonDataProcess::sendSignoutInfo()
{
    qDebug()<<"send sign out info";
    PokemonData signOutinfo;
    signOutinfo.setType(PokemonData::DATA_TYPE::SIGN_OUT);
    signOutinfo.setUsername(QString::fromStdString(signWindow->getCurrentPlayer()->getName()));
    signOutinfo.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
    signOutinfo.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
    signOutinfo.setListenPort(signWindow->getCurrentPlayer()->getListenPort());
    this->setLastOp(PokemonDataProcess::SIGNOUT);
    this->sendData(&signOutinfo);
}

int PokemonDataProcess::DEFAULT_HOST_PORT=8090;
QString PokemonDataProcess::DEFAULT_ADDRESS="127.0.0.1";
