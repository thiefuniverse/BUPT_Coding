#include "window_signup.h"
#include "ui_window_signup.h"

Window_SignUp::Window_SignUp(PokemonDataProcess *processor,QWidget *parent) :
    QDialog(parent),dataProcessor(processor),
    ui(new Ui::Window_SignUp)
{
    ui->setupUi(this);

    this->setWindowTitle("Welcome to Thief's World!");
    setPassEcho(QLineEdit::EchoMode::Password);


    initLoginWindow();
    initSocket();
}

Window_SignUp::~Window_SignUp()
{
    delete ui;
}

void Window_SignUp::showPlayerUi()
{
    //playui=new PlayUi();
   // playui->show();
}

void Window_SignUp::initLoginWindow()
{
    ui->userName_Label->setVisible(false);
    ui->userName_Text->setVisible(false);
    ui->password_Again_Label->setVisible(false);
    ui->password_Again_Text->setVisible(false);
    ui->password_Label->setVisible(false);
    ui->password_Text->setVisible(false);
    ui->ok->setVisible(false);
    ui->cancel->setVisible(false);

    ui->password_NotSame->setVisible(false);
    ui->errorInfo->setVisible(false);
    ui->showPassword->setVisible(false);

    ui->signIn->setVisible(true);
    ui->signUp->setVisible(true);

   // dataProcessor= new PokemonDataProcess();
}

void Window_SignUp::initSocket()
{

   // dataProcessor=new PokemonDataProcess();
    currentPlayer=new PokemonPlayer("");
    dataProcessor->listenData(&receiver,currentPlayer,0);  // bind a port for listening
    connect(&receiver,SIGNAL(readyRead()),this,SLOT(readPendingDatagram()));

    connect(this,SIGNAL(signFeedBack(bool)),this,SLOT(signFeedbackProcess(bool)));

}

void Window_SignUp::showInput()
{
    ui->userName_Label->setVisible(true);
    ui->userName_Text->setVisible(true);
    ui->password_Label->setVisible(true);
    ui->password_Text->setVisible(true);
    ui->ok->setVisible(true);
    ui->cancel->setVisible(true);

    ui->showPassword->setVisible(true);
    ui->signIn->setVisible(false);
    ui->signUp->setVisible(false);
    ui->errorInfo->setVisible(false);
    ui->password_NotSame->setVisible(false);
}

PokemonPlayer *Window_SignUp::getCurrentPlayer() const
{
    return currentPlayer;
}

void Window_SignUp::on_signIn_clicked()
{
    showInput();
    switchState(Ui::LOGIN_TYPE::SIGNIN);


}

void Window_SignUp::on_signUp_clicked()
{
    showInput();
    ui->password_Again_Label->setVisible(true);
    ui->password_Again_Text->setVisible(true);
    switchState(Ui::LOGIN_TYPE::SIGNUP);
}

void Window_SignUp::on_cancel_clicked()
{
    initLoginWindow();
}

void Window_SignUp::switchState(Ui::LOGIN_TYPE type)
{
    loginState=type;
}

void Window_SignUp::ok_signUp()
{
    if(ui->password_Text->text()==""||ui->userName_Text->text()=="")
    {
        ui->errorInfo->setVisible(true);
        QPalette temp;
        temp.setColor(QPalette::WindowText,Qt::red);
        ui->errorInfo->setPalette(temp);
    }
    else
    {
        ui->errorInfo->setVisible(false);
        if(ui->password_Text->text()==ui->password_Again_Text->text())
        {
            ui->password_NotSame->setVisible(false);

           // QString nameEncrypted=PokemonEncrypter::encryptItem(QByteArray(ui->userName_Text->text().toLatin1().data()));
            QString userName=ui->userName_Text->text();
            QString passwordEncrypted=PokemonEncrypter::encryptItem(QByteArray(ui->password_Text->text().toLatin1().data()));

            // should process
            PokemonData signUpInfo;
            signUpInfo.setType(PokemonData::DATA_TYPE::SIGN_UP);
            signUpInfo.setUsername(userName);
            signUpInfo.setPassword(passwordEncrypted);
            signUpInfo.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
            signUpInfo.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
            signUpInfo.setListenPort(currentPlayer->getListenPort());
            dataProcessor->setLastOp(PokemonDataProcess::SIGNUP);
            dataProcessor->sendData(&signUpInfo);
            currentPlayer->setName(userName.toStdString());
        //    currentPlayer=new PokemonPlayer(userName.toStdString());

        //    dataProcessor->sendData(signUpInfo,PokemonData::DATA_TYPE::SIGN_UP);

        }
        else
        {
            ui->password_NotSame->setVisible(true);
            QPalette temp;
            temp.setColor(QPalette::WindowText,Qt::red);
            ui->password_NotSame->setPalette(temp);
        }
    }
}

void Window_SignUp::ok_signIn()
{
    if(ui->password_Text->text()==""||ui->userName_Text->text()=="")
    {
        ui->errorInfo->setVisible(true);
        QPalette temp;
        temp.setColor(QPalette::WindowText,Qt::red);
        ui->errorInfo->setPalette(temp);

    }
    else
    {
        ui->errorInfo->setVisible(false);
       // QString nameEncrypted=PokemonEncrypter::encryptItem(QByteArray(ui->userName_Text->text().toLatin1().data()));
        QString userName=ui->userName_Text->text();
        QString passwordEncrypted=PokemonEncrypter::encryptItem(QByteArray(ui->password_Text->text().toLatin1().data()));

        // should process

//        QVector<QString> signInInfo;
//        signInInfo.push_back(userName);
//        signInInfo.push_back(passwordEncrypted);
        PokemonData signInfo;
        signInfo.setType(PokemonData::DATA_TYPE::SIGN_IN);
        signInfo.setUsername(userName);
        signInfo.setPassword(passwordEncrypted);
        signInfo.setAddress(PokemonDataProcess::DEFAULT_ADDRESS);
        signInfo.setPort(PokemonDataProcess::DEFAULT_HOST_PORT);
        signInfo.setListenPort(currentPlayer->getListenPort());
        dataProcessor->setLastOp(PokemonDataProcess::SIGNIN);
        dataProcessor->sendData(&signInfo);
        currentPlayer->setName(userName.toStdString());
      //  currentPlayer=new PokemonPlayer(userName.toStdString());
    }
}

void Window_SignUp::setPassEcho(QLineEdit::EchoMode show)
{
    ui->password_Text->setEchoMode(show);
    ui->password_Again_Text->setEchoMode(show);
}

void Window_SignUp::on_ok_clicked()
{
    if(loginState==Ui::LOGIN_TYPE::SIGNUP)
    {
        ok_signUp();
    }
    else
    {
        ok_signIn();
    }

}

void Window_SignUp::on_showPassword_clicked()
{
    if(ui->showPassword->isChecked())
    {
        setPassEcho(QLineEdit::EchoMode::Normal);
    }
    else
    {
        setPassEcho(QLineEdit::EchoMode::Password);
    }
}

void Window_SignUp::readPendingDatagram()
{
    //dataProcessor->setSignUpWindow(this);
    dataProcessor->receiveDataClient(&receiver);
}

void Window_SignUp::signFeedbackProcess(bool success)
{
    if(loginState==Ui::LOGIN_TYPE::SIGNIN)
    {
        if(success)
        {
           QMessageBox::information(NULL,tr("Congratulations！！"),tr("Sign in Successfully！！"));
        }
        else
        {
           QMessageBox::information(NULL,tr("Sorry！！"),tr("unvalid username or password ... "));
        }
    }
    else
    {
        if(success)
        {
           QMessageBox::information(NULL,tr("Congratulations！！"),tr("Sign up Successfully！！"));
        }
        else
        {
           QMessageBox::information(NULL,tr("Sorry！！"),tr("username has existed!"));
        }
    }

}
