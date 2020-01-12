#ifndef FIGHTWINDOW_H
#define FIGHTWINDOW_H

#include <QWidget>
#include<QPaintEvent>
#include<QPainter>
#include<QPushButton>
#include<QPropertyAnimation>
#include"pokemondata.h"
#include<vector>
#include<QTimer>
#include<QMessageBox>
using namespace std;

namespace Ui {
class FightWindow;
}

class FightWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FightWindow(QWidget *parent = 0);
    ~FightWindow();

    void setAttacker(RealPokemon *value);
    void showAttack();
    void showEnemy();
    void showSkills();
    void initSkills();
    void animator();

    void startFight();

    void setEnemy(RealPokemon *value);
    void initWindow();
    void setAttackhurt(const vector<int> &value);

    void setEnemyHurt(const vector<int> &value);

    void setAttackerBaby(PokemonBaby *value);

private slots:
    void movieContinue();

private:
    void showResult();

    Ui::FightWindow *ui;
    RealPokemon* attacker;
    RealPokemon* enemy;
    PokemonBaby* attackerBaby;

    QPropertyAnimation* attackAni;
    QPropertyAnimation* enemyAni;
    QPropertyAnimation* skillAni;
    QPropertyAnimation* tempAni;
    QTimer *timer;
    vector<int> attackhurt;
    vector<int> enemyHurt;

    int attackHurtFlag;
    int enemyHurtFlag;

    bool attackRound;

};

#endif // FIGHTWINDOW_H
