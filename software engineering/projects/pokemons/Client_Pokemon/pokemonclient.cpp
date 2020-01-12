#include "pokemonclient.h"

PokemonClient::PokemonClient()
{
    initEnvironment();
}

PokemonClient::~PokemonClient()
{
    delete playui;
}


void PokemonClient::initWindows()
{
    playui=new PlayUi();
    windows_signup=new Window_SignUp(dataProcessor);
    dataProcessor->setSignUpWindow(windows_signup);
    dataProcessor->setPlayUiWindow(playui);

    playui->setDataProcessor(dataProcessor);
    playui->setMainController(this);
}

void PokemonClient::initEnvironment()
{
    dataProcessor=new PokemonDataProcess();
    currentPlayer=new PokemonPlayer("");

   // receiver=new QUdpSocket();
   // dataProcessor->listenData(&receiver,currentPlayer,0);  // bind a port for listening
    //connect(receiver,SIGNAL(readyRead()),this,SLOT(readPendingDatagram()));

    initWindows();
}

void PokemonClient::showSignUp()
{
    windows_signup->show();
}

void PokemonClient::backToSign()
{
    delete playui;
    playui=new PlayUi();
    dataProcessor->setPlayUiWindow(playui);
    playui->setDataProcessor(dataProcessor);
    playui->setMainController(this);
    dataProcessor->clearData();
    windows_signup->show();
}

void PokemonClient::exitProgram()
{
    mainProgram->exit();
}

void PokemonClient::readPendingDatagram()
{
    //dataProcessor->receiveDataClient(receiver);
}

void PokemonClient::setMainProgram(QApplication *value)
{
    mainProgram = value;
}


