#include "fightwindow.h"
#include "ui_fightwindow.h"

FightWindow::FightWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FightWindow)
{
    ui->setupUi(this);

    timer=new QTimer();
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(movieContinue()));
    attackRound=true;
}

FightWindow::~FightWindow()
{
    delete attackAni;
    delete enemyAni;
    delete skillAni;
    delete tempAni;
    delete ui;
}

void FightWindow::initWindow()
{
    QPixmap examImg1;
    examImg1.load(QString::fromStdString(":/resource/"+attacker->roleName+".png"));
    examImg1=examImg1.scaled(200,200,Qt::KeepAspectRatio);
    ui->attacker_image->setPixmap(examImg1);

    QPixmap examImg2;
    examImg2.load(QString::fromStdString(":/resource/"+enemy->roleName+".png"));
    examImg2=examImg2.scaled(200,200,Qt::KeepAspectRatio);
    ui->enemy_image->setPixmap(examImg2);


    attackAni=new QPropertyAnimation(ui->attacker_image,"geometry");
    attackAni->setDuration(300);
    attackAni->setStartValue(QRect(1,1,200,200));
//    for(int i=0;i<4;++i)
//    {
//         attackAni->setKeyValueAt(0.1+i*0.1,QRect(410,1,200,200));
//         attackAni->setKeyValueAt(0.1+(i+1)*0.1,QRect(1,1,200,200));
//    }
    attackAni->setKeyValueAt(0.5,QRect(410,1,200,200));
    attackAni->setEndValue(QRect(1,1,200,200));

    enemyAni=new QPropertyAnimation(ui->enemy_image,"geometry");
    enemyAni->setDuration(300);
    enemyAni->setStartValue(QRect(410,1,200,200));
//    for(int i=0;i<4;++i)
//    {
//         enemyAni->setKeyValueAt(0.1+i*0.1,QRect(410,1,200,200));
//         enemyAni->setKeyValueAt(0.1+(i+1)*0.1,QRect(1,1,200,200));
//    }

    enemyAni->setKeyValueAt(0.5,QRect(1,1,200,200));
    enemyAni->setEndValue(QRect(410,1,200,200));

    initSkills();
    attackHurtFlag=0;
    enemyHurtFlag=0;
}

void FightWindow::movieContinue()
{
    if (attackRound)
    {
        if(attackHurtFlag==attackhurt.size())
        {
            timer->stop();
            showResult();
            this->hide();
            return;
        }
        ui->enemy_Hurt->setText("-"+QString::fromStdString(
                                       std::to_string(attackhurt[attackHurtFlag])));
        showAttack();
        attackHurtFlag++;
    }
    else
    {
        if(enemyHurtFlag==enemyHurt.size())
        {
            timer->stop();
            showResult();
            this->hide();
            return;
        }
        ui->attacker_Hurt->setText("-"+QString::fromStdString(
                                       std::to_string(enemyHurt[enemyHurtFlag])));
        showEnemy();
        enemyHurtFlag++;
    }
    attackRound=!attackRound;
}

void FightWindow::showResult()
{
    if(attackerBaby->getIsWinner())
    {
        QMessageBox::information(NULL,tr("Note！！"),
                                 tr("Congratulations!You win!\nYou can get one ")+
                                 QString::fromStdString(enemy->getRoleName()));
    }
    else
    {
        QMessageBox::information(NULL,tr("Note！！"),tr("You lose!"));
    }

}

void FightWindow::setAttackerBaby(PokemonBaby *value)
{
    attackerBaby = value;
}

void FightWindow::setEnemyHurt(const vector<int> &value)
{
    enemyHurt = value;
}

void FightWindow::setAttackhurt(const vector<int> &value)
{
    attackhurt = value;
}

void FightWindow::startFight()
{
    tempAni=new QPropertyAnimation(ui->fight_start,"geometry");
    tempAni->setDuration(2000);

    tempAni->setStartValue(QRect(210,-50,221,41));
    tempAni->setKeyValueAt(0.2,QRect(210,140,221,41));
    tempAni->setKeyValueAt(0.8,QRect(210,140,221,41));
    tempAni->setEndValue(QRect(210,-50,221,41));
    tempAni->start();
    timer->start();
}

void FightWindow::setEnemy(RealPokemon *value)
{
    enemy = value;
}

void FightWindow::setAttacker(RealPokemon *value)
{
    attacker = value;
}

void FightWindow::showAttack()
{
    attackAni->start();
    ui->skill->setText(QString::fromStdString(attacker->skill));

    showSkills();
}

void FightWindow::showEnemy()
{
    enemyAni->start();
    ui->skill->setText(QString::fromStdString(enemy->skill));
    showSkills();
}

void FightWindow::showSkills()
{
    skillAni->start();
}

void FightWindow::initSkills()
{
    skillAni=new QPropertyAnimation(ui->skill,"geometry");
    skillAni->setDuration(300);

    skillAni->setStartValue(QRect(240,30,221,41));
    skillAni->setKeyValueAt(0.3,QRect(110,150,221,41));
    skillAni->setKeyValueAt(0.7,QRect(330,150,221,41));
    skillAni->setEndValue(QRect(240,30,221,41));
}

void FightWindow::animator()
{
    showAttack();
}
