#include "playui.h"
#include "ui_playui.h"

PlayUi::PlayUi(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayUi)
{
    ui->setupUi(this);
    connect(ui->personalPoke_Left,SIGNAL(clicked()),this,SLOT(updatePersonalPokeL()));
    connect(ui->personalPoke_Right,SIGNAL(clicked()),this,SLOT(updatePersonalPokeR()));
    //connect(ui->actionShow_others,SIGNAL(hovered());
    connect(this,SIGNAL(receiveSuccess(int)),this,SLOT(updateOthersPokemon(int)));
    connect(this,SIGNAL(receSystem(int)),this,SLOT(updateSystemPoke(int)));
  //  setOthersUi();
    initWindows();
    initBackMusic();
    showPersonal(true);

}

PlayUi::~PlayUi()
{
    delete backMusic;
    delete backList;
    delete ui;
}

void PlayUi::paintEvent(QPaintEvent *event)
{

}

void PlayUi::setPersonalUser()
{
    //ui->curplayerName_show->setText(QString::fromStdString(currentPlayer->getUsername()));
    //    ui->curpokemonNums_show->setText(currentPlayer->getUsername());
}

void PlayUi::updatePersonalUser(PokePlayer *poker)
{
    ui->curplayerName_show->setText(QString::fromStdString(poker->getName()));
    ui->curpokemonNums_show->setText(QString::fromStdString(to_string(poker->getPokemonsNum())));

   // ui->Personal_page->setStyleSheet("background-image: url(:/resource/back7.png)");
   // ui->Others_page->setStyleSheet("background-image: url(:/resource/back7.png)");
   // ui->System_page->setStyleSheet("background-image: url(:/resource/back7.png)");
    QPixmap examImg;

    if(poker->getPokemonsNum()>10)
    {
        examImg.load(QString::fromStdString(":/resource/honor2.png"));

    }
    else
    {
        examImg.load(QString::fromStdString(":/resource/honor1.png"));

    }
    examImg=examImg.scaled(50,50,Qt::KeepAspectRatio);
    ui->personalHonor->setPixmap(examImg);

    float winR=poker->winRate;
    ostringstream convert;
    convert<<setprecision(3)<<winR;
    ui->winRate_show->setText(QString::fromStdString(convert.str()));

}

void PlayUi::showOnePersonalPoke(int show)
{
    ui->level_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].level)));
    ui->ep_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].ep)));
    ui->attackPowers_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].attackP)));
    ui->defendPower_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].defendP)));
    ui->heartPower_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].blood)));
    ui->speedPower_label->setText(QString::fromStdString(to_string((*curPlayerPokes)[show].speed)));
    ui->personalPokemon_name->setText(QString::fromStdString((*curPlayerPokes)[show].roleName));
    ui->personalPokemon_description->setText(QString::fromStdString((*curPlayerPokes)[show].description));
    // todo set image
    QPixmap examImg;

    examImg.load(QString::fromStdString(":/resource/"+(*curPlayerPokes)[show].roleName+".png"));
    examImg=examImg.scaled(200,200,Qt::KeepAspectRatio);
    ui->personalPokemon_image->setPixmap(examImg);
}

void PlayUi::showOneOtherPoke(int show)
{
    ui->level_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].level)));
    ui->ep_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].ep)));
    ui->attackPowers_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].attackP)));
    ui->defendPower_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].defendP)));
    ui->heartPower_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].blood)));
    ui->speedPower_label_o->setText(QString::fromStdString(to_string((*otherPlayerPokes)[show].speed)));
    ui->otherPokemon_name->setText(QString::fromStdString((*otherPlayerPokes)[show].roleName));
    ui->otherPokemon_description->setText(QString::fromStdString((*otherPlayerPokes)[show].description));
    QPixmap examImg;

    examImg.load(QString::fromStdString(":/resource/"+(*otherPlayerPokes)[show].roleName+".png"));
    examImg=examImg.scaled(200,200,Qt::KeepAspectRatio);
    ui->otherPokemon_image->setPixmap(examImg);

}

void PlayUi::showOneSystemPoke(int show)
{
    ui->level_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].level)));
    ui->ep_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].ep)));
    ui->attackPowers_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].attackP)));
    ui->defendPower_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].defendP)));
    ui->heartPower_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].blood)));
    ui->speedPower_label_s->setText(QString::fromStdString(to_string((*systemPokes)[show].speed)));
    ui->systemPokemon_name->setText(QString::fromStdString((*systemPokes)[show].roleName));
    ui->systemPokemon_description->setText(QString::fromStdString((*systemPokes)[show].description));
    QPixmap examImg;

    examImg.load(QString::fromStdString(":/resource/"+(*systemPokes)[show].roleName+".png"));
    examImg=examImg.scaled(200,200,Qt::KeepAspectRatio);
    ui->systemPokemon_image->setPixmap(examImg);
}

void PlayUi::updatePersonalPokeL()
{
    curPokeFlag=(curPokeFlag-1+(*curPlayerPokes).size())%((*curPlayerPokes).size());
    showOnePersonalPoke(curPokeFlag);
}

void PlayUi::updateSystemPoke(int show)
{
    sysPokeFlag=0;
    showOneSystemPoke(sysPokeFlag);
}

void PlayUi::updatePersonalPokeR()
{
    curPokeFlag=(curPokeFlag+1)%((*curPlayerPokes).size());
    showOnePersonalPoke(curPokeFlag);
}

void PlayUi::updateOtherUsers(vector<PokePlayer> &allOthers)
{
    QStringListModel *model;
    QStringList currentList;
    for(auto one:allOthers)
    {
        QString onePlayer(QString::fromStdString(one.getName()));
        currentList.push_back(onePlayer);
    }
   // QString thief("thisuniverse");
   // currentList.push_back(thief);
    model=new QStringListModel(this);
    model->setStringList(currentList);
    ui->listView_otherName->setModel(model);
    //setOthersUi();
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void PlayUi::fightTwoRealPoke()
{
    PokemonBaby* attacker=getRealPoke(attackerPoke);
     PokemonBaby* enemy=getRealPoke(enemyPoke);
     attacker->setIsWinner(true);
     attacker->attack(enemy);

     attackerPoke->update(attacker);
     enemyPoke->update(enemy);
     updateCurPlayer(attacker->getIsWinner());
     isWinner=attacker->getIsWinner();

     // todo get winner and handle
     fightwindow=new FightWindow();
     fightwindow->setAttacker(attackerPoke);
     fightwindow->setEnemy(enemyPoke);
     fightwindow->setAttackerBaby(attacker);
     fightwindow->setAttackhurt(attacker->getAllHurts());
     fightwindow->setEnemyHurt(enemy->getAllHurts());

     fightwindow->initWindow();
     fightwindow->show();
     fightwindow->startFight();
     handleAttackerPoke();
}

void PlayUi::updateOtherState(PokemonData *user)
{
    if(user->isSuccess())
    {
        ui->online_info->setStyleSheet("background-color: rgb(0, 255, 127)");
    }
    else
    {
        ui->online_info->setStyleSheet("background-color: rgb(170, 170, 255)");
    }
}

void PlayUi::updateCurPlayer(bool isWin)
{
    if(isWin)
    {
        currentPlayer->setWinRate((currentPlayer->winRate*100+1)/(100+1));
        dataProcessor->updateRemoteUser(currentPlayer);
        updatePersonalUser(currentPlayer);
    }
    else
    {
        currentPlayer->setWinRate((currentPlayer->winRate*100)/(100+1));
        dataProcessor->updateRemoteUser(currentPlayer);
        updatePersonalUser(currentPlayer);
    }
}

void PlayUi::handleAttackerPoke()
{
    if(isLevelUpWar)
    {
        updateAttacker();
    }
    else
    {
        if(isWinner)
        {
          addAwardPoke();
        }
        else
        {
          showSelectLose();
          isLoseSelect=true;

        }
        updateAttacker();
    }
}

void PlayUi::addAwardPoke()
{
  (*curPlayerPokes).push_back(*enemyPoke);
   dataProcessor->requireSysAward(sysPokeFlag);
}

void PlayUi::updateAttacker()
{
    showOnePersonalPoke(curPokeFlag);
    dataProcessor->updateRemotePoke(attackerPoke);
}

void PlayUi::deleteAttacker()
{
    loseSelectFlag=0;
    geneSelectLose();
   dataProcessor->deleteRemotePoke(&((*curPlayerPokes)[loseSelections[loseSelectFlag]]));
  (*curPlayerPokes).erase((*curPlayerPokes).begin()+loseSelectFlag);

  if((*curPlayerPokes).empty())
  {
      dataProcessor->requireSysAward(0);
      (*curPlayerPokes).push_back((*systemPokes)[0]);
      showOnePersonalPoke(0);
  }
  else
  {
    updatePersonalPokeL();
  }

}

void PlayUi::geneSelectLose()
{
    int curSize=(*curPlayerPokes).size();
    if(curSize<=3)
    {
        for(int i=0;i<curSize;i++)
        {
            loseSelections.push_back(i);
        }
    }
    else
    {
        srand(NULL);
        int start=rand()%(curSize-2);
        for(int i=0;i<3;i++)
        {
            loseSelections.push_back(start+i);
        }
    }
}

void PlayUi::selectOneAttacker()
{
    ui->allPages->setCurrentIndex(0);
    //ui->Personal_page->setEnabled(false);
    //ui->personalPoke_Left->setEnabled(true);
    //ui->personalPoke_Right->setEnabled(true);
    //ui->selectIt->setEnabled(true);
    showSelector();
}

void PlayUi::showSelector()
{
    ui->selectIt->show();
    ui->selectIt_info->show();
    ui->challenge_others->hide();
    ui->challeng_system->hide();
    ui->sign_out->hide();
    ui->exit->hide();
}

void PlayUi::hideSelector()
{
    ui->selectIt_info->hide();
    ui->selectIt->hide();
    ui->challenge_others->show();
    ui->challeng_system->show();
    ui->sign_out->show();
    ui->exit->show();
}

void PlayUi::forbidOptions()
{
    ui->challenge_others->setEnabled(false);
    ui->challeng_system->setEnabled(false);
    ui->sign_out->setEnabled(false);
    ui->exit->setEnabled(false);
}

void PlayUi::resumeOptions()
{
    ui->challenge_others->setEnabled(true);
    ui->challeng_system->setEnabled(true);
    ui->sign_out->setEnabled(true);
    ui->exit->setEnabled(true);
}

void PlayUi::updateOthersPokemon(int row)
{
    otherPokeFlag=0;
    showOneOtherPoke(otherPokeFlag);
}


void PlayUi::showOthers(bool m)
{
   ui->allPages->setCurrentIndex(1);
}

void PlayUi::showPersonal(bool)
{
    ui->allPages->setCurrentIndex(0);

}

void PlayUi::showSettings(bool)
{
    ui->allPages->setCurrentIndex(3);
}

void PlayUi::emitReceiveSuccess()
{
    receiveSuccess(0);
}

void PlayUi::emitReceSystemSuce()
{
    requireSys=true;
    receSystem(0);

}

PokemonBaby *PlayUi::getRealPoke(RealPokemon *realPoke)
{
    PokemonBaby* real;
    if (realPoke->attribute=="power")
    {
        real=new PokemonPower(realPoke->name_for_user,realPoke->level,
                              realPoke->roleName,realPoke->ep,realPoke->attackP,realPoke->defendP,
                              realPoke->blood,realPoke->speed);
    }
    else if(realPoke->attribute=="fast")
    {
        real=new PokemonFast(realPoke->name_for_user,realPoke->level,
                              realPoke->roleName,realPoke->ep,realPoke->attackP,realPoke->defendP,
                              realPoke->blood,realPoke->speed);
    }
    else if(realPoke->attribute=="blood")
    {
        real=new PokemonBlood(realPoke->name_for_user,realPoke->level,
                              realPoke->roleName,realPoke->ep,realPoke->attackP,realPoke->defendP,
                              realPoke->blood,realPoke->speed);
    }
    else if(realPoke->attribute=="defend")
    {
        real=new PokemonDefend(realPoke->name_for_user,realPoke->level,
                              realPoke->roleName,realPoke->ep,realPoke->attackP,realPoke->defendP,
                              realPoke->blood,realPoke->speed);
    }
    return std::move(real);
}

void PlayUi::setOthersUi()
{
    QPixmap heart_image;
    heart_image.load(":/heartPowers.png");
    heart_image=heart_image.scaled(25,25,Qt::KeepAspectRatio);
    //ui->personalPokemon_gramphic->setPalette(heart_image);;
    ui->heartPower_image->setPixmap(heart_image);
    ui->heartPower_image_o->setPixmap(heart_image);
    ui->heartPower_image_s->setPixmap(heart_image);

    QPixmap attack_image;
    attack_image.load(":/attackPowers.png");
    attack_image=attack_image.scaled(25,25,Qt::KeepAspectRatio);
    ui->attckPowers_image->setPixmap(attack_image);
    ui->attckPowers_image_o->setPixmap(attack_image);
    ui->attckPowers_image_s->setPixmap(attack_image);

    QPixmap defend_image;
    defend_image.load(":/defendPowers.png");
    defend_image=defend_image.scaled(25,25,Qt::KeepAspectRatio);
    ui->defendPower_image->setPixmap(defend_image);
    ui->defendPower_image_o->setPixmap(defend_image);
    ui->defendPower_image_s->setPixmap(defend_image);

    QPixmap speed_image;
    speed_image.load(":/speedPowers.png");
    speed_image=speed_image.scaled(25,25,Qt::KeepAspectRatio);
    ui->speedPower_image->setPixmap(speed_image);
    ui->speedPower_image_o->setPixmap(speed_image);
    ui->speedPower_image_s->setPixmap(speed_image);



}

void PlayUi::initWindows()
{
    requireSys=false;
    selectItDel=false;
    isLoseSelect=false;
    ui->lose_It->hide();
 setOthersUi();
 hideSelector();


}

void PlayUi::initBackMusic()
{
    backMusic=new QMediaPlayer;
    backList=new QMediaPlaylist;
    backList->addMedia(QUrl::fromLocalFile("/home/thief/workspace/pokemon_thief/Pokemon_thief/resource/alone.mp3"));
    backMusic->setVolume(100);
    backMusic->play();
    backList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    backMusic->setPlaylist(backList);
    backMusic->play();
    isPlaying=true;


}


bool PlayUi::getRequireSys() const
{
    return requireSys;
}

void PlayUi::setSystemPokes(vector<RealPokemon> *value)
{
    systemPokes = value;
}

PokePlayer *PlayUi::getCurrentPlayer() const
{
    return currentPlayer;
}

void PlayUi::setCurrentPlayer(PokePlayer *value)
{
    currentPlayer = value;
}

void PlayUi::setMainController(PokemonClient *value)
{
    mainController = value;
}

void PlayUi::setReceOtherSuccess(bool value)
{
    receOtherSuccess = value;
}

void PlayUi::setDataProcessor(PokemonDataProcess *value)
{
    dataProcessor = value;

}

void PlayUi::setOtherPlayerPokes(vector<RealPokemon> *value)
{
    otherPlayerPokes = value;
}


void PlayUi::setCurPlayerPokes(vector<RealPokemon> *value)
{
    curPlayerPokes = value;
    curPokeFlag=0;
    showOnePersonalPoke(curPokeFlag);
}


void PlayUi::on_listView_otherName_clicked(const QModelIndex &index)
{
    receOtherSuccess=false;
    dataProcessor->requirePokemons(index.data().toString());
}

void PlayUi::on_otherPoke_Left_clicked()
{
    if(isLoseSelect)
    {
        loseSelectFlag=(loseSelectFlag-1+loseSelections.size())%(loseSelections.size());
        showOneOtherPoke(loseSelections[loseSelectFlag]);
    }
    else
    {
     otherPokeFlag=(otherPokeFlag-1+(*otherPlayerPokes).size())%((*otherPlayerPokes).size());
     showOneOtherPoke(otherPokeFlag);
    }
}

void PlayUi::on_otherPoke_Right_clicked()
{
    if(isLoseSelect)
    {
        loseSelectFlag=(loseSelectFlag+1+loseSelections.size())%(loseSelections.size());
        showOneOtherPoke(loseSelections[loseSelectFlag]);
    }
    else
    {
    otherPokeFlag=(otherPokeFlag+1+(*otherPlayerPokes).size())%((*otherPlayerPokes).size());
    showOneOtherPoke(otherPokeFlag);
    }
}

void PlayUi::on_challenge_others_clicked()
{
    ui->allPages->setCurrentIndex(1);

}

void PlayUi::on_challeng_system_clicked()
{
    ui->allPages->setCurrentIndex(2);
}

void PlayUi::on_exit_clicked()
{
    dataProcessor->sendSignoutInfo();
    mainController->exitProgram();
}

void PlayUi::on_sign_out_clicked()
{
    dataProcessor->sendSignoutInfo();
    mainController->backToSign();
}

void PlayUi::on_fight_withSystem_clicked()
{
    isLevelUpWar=true;

    // get enemyPokemon
   // enemyPoke=&((*systemPokes)[sysPokeFlag]);
    enemyPoke=&(*systemPokes)[sysPokeFlag];

    QMessageBox info;
    info.setWindowTitle("Really?");
    info.setText("Try to Fight with It?");
    info.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    switch(info.exec())
    {
        case QMessageBox::Yes:
            qDebug()<<"info      yes";
            selectOneAttacker();
            break;
        case QMessageBox::No:
            qDebug()<<"info      no";
            break;
    }
}

void PlayUi::on_capture_withSystem_clicked()
{
    isLevelUpWar=false;

    // get enemyPokemon
    enemyPoke=&((*systemPokes)[sysPokeFlag]);

    QMessageBox info;
    info.setWindowTitle("Really?");
    info.setText("Try to Capture It?");
    info.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    switch(info.exec())
    {
        case QMessageBox::Yes:
            qDebug()<<"info      yes";
            selectOneAttacker();
            break;
        case QMessageBox::No:
            qDebug()<<"info      no";
            break;
    }

}

void PlayUi::on_selectIt_clicked()
{
        hideSelector();
        attackerPoke=&((*curPlayerPokes)[curPokeFlag]);
        fightTwoRealPoke();
}

void PlayUi::on_systemPoke_Left_clicked()
{
    sysPokeFlag=(sysPokeFlag-1+(*systemPokes).size())%((*systemPokes).size());
    showOneSystemPoke(sysPokeFlag);
}

void PlayUi::on_systemPoke_Right_clicked()
{
    sysPokeFlag=(sysPokeFlag+1+(*systemPokes).size())%((*systemPokes).size());
    showOneSystemPoke(sysPokeFlag);
}

void PlayUi::on_lose_It_clicked()
{
    deleteAttacker();
    isLoseSelect=false;
    hideSelectLose();
}

void PlayUi::showSelectLose()
{
    ui->selectIt->hide();
    ui->lose_It->show();
    ui->selectIt_info->setText("Please select one for losing!");
    ui->selectIt_info->show();
    ui->challenge_others->hide();
    ui->challeng_system->hide();
    ui->sign_out->hide();
    ui->exit->hide();
}

void PlayUi::hideSelectLose()
{
    ui->lose_It->hide();
    ui->selectIt_info->hide();
    ui->challenge_others->show();
    ui->challeng_system->show();
    ui->sign_out->show();
    ui->exit->show();
  //   ui->selectIt_info->setText("Please select one for losing!");
}

void PlayUi::on_fight_withOthers_clicked()
{
    isLevelUpWar=true;

    // get enemyPokemon
   // enemyPoke=&((*systemPokes)[sysPokeFlag]);
    enemyPoke=&(*otherPlayerPokes)[otherPokeFlag];

    QMessageBox info;
    info.setWindowTitle("Really?");
    info.setText("Try to Fight with It?");
    info.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    switch(info.exec())
    {
        case QMessageBox::Yes:
            qDebug()<<"info      yes";
            selectOneAttacker();
            break;
        case QMessageBox::No:
            qDebug()<<"info      no";
            break;
    }
}

void PlayUi::on_music_clicked()
{
    if(isPlaying)
    {
        backMusic->stop();
        isPlaying=false;
    }
    else
    {
        backMusic->play();
        isPlaying=true;
    }
}
