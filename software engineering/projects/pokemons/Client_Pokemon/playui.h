#ifndef PLAYUI_H
#define PLAYUI_H

#include <QMainWindow>
#include<QStringListModel>
#include<QDebug>
#include<QPainter>
#include<QPaintDevice>
#include"fightwindow.h"
#include<iomanip>
#include<QMediaPlayer>
#include<QMediaPlaylist>

#include "pokemondataprocess.h"
//#include "pokemondata.h"
//#include "player.h"
#include"pokemonclient.h"
#include"pokemon.h"

#include<QListView>

using namespace std;


class PokemonDataProcess;
class PokemonClient;
namespace Ui {
class PlayUi;

}

class PlayUi : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayUi(QWidget *parent = 0);
    ~PlayUi();
    void paintEvent(QPaintEvent* event);
    void updatePersonalUser(PokePlayer* poker);
    void showOnePersonalPoke(int show);
    void showOneOtherPoke(int show);
    void showOneSystemPoke(int show);

    void updateOtherUsers(vector<PokePlayer>& allOthers);   
    void fightTwoRealPoke(); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void updateOtherState(PokemonData* user);
    void updateCurPlayer(bool isWin);

    void handleAttackerPoke();
    void addAwardPoke();
    void updateAttacker();
    void deleteAttacker();
    void geneSelectLose();

    void selectOneAttacker();
    void showSelector();
    void hideSelector();
    void forbidOptions();
    void resumeOptions();
    void emitReceiveSuccess();
    void emitReceSystemSuce();
    PokemonBaby* getRealPoke(RealPokemon* realPoke);
    //set functions
    void setPersonalUser();
    void setPersonalPokemon();
    void setCurPlayerPokes(vector<RealPokemon> *value);
    void setOtherPlayerPokes(vector<RealPokemon> *value);
    void setDataProcessor(PokemonDataProcess *value);
    void setReceOtherSuccess(bool value);
    void setMainController(PokemonClient *value);

    void setCurrentPlayer(PokePlayer *value);

    PokePlayer *getCurrentPlayer() const;

    void setSystemPokes(vector<RealPokemon> *value);

    bool getRequireSys() const;

public slots:
    void updateOthersPokemon(int row);
    void updatePersonalPokeR();
    void updatePersonalPokeL();
    void updateSystemPoke(int show);
    void showOthers(bool);
    void showPersonal(bool);
    void showSettings(bool);


private slots:
   // void on_listView_otherName_clicked(const QModelIndex &index);

    void on_listView_otherName_clicked(const QModelIndex &index);

    void on_otherPoke_Left_clicked();

    void on_otherPoke_Right_clicked();

    void on_challenge_others_clicked();

    void on_challeng_system_clicked();

    void on_exit_clicked();

    void on_sign_out_clicked();

    void on_fight_withSystem_clicked();

    void on_capture_withSystem_clicked();

    void on_selectIt_clicked();

    void on_systemPoke_Left_clicked();

    void on_systemPoke_Right_clicked();


    void on_lose_It_clicked();

    void on_fight_withOthers_clicked();

    void on_music_clicked();

signals:
    bool receiveSuccess(int);
    bool receSystem(int);
private:
    void showSelectLose();
    void hideSelectLose();
    void setOthersUi();
    void initWindows();
    void initBackMusic();
    Ui::PlayUi *ui;
    PokemonDataProcess *dataProcessor;

    vector<RealPokemon> *curPlayerPokes;
    vector<RealPokemon> *otherPlayerPokes;
    vector<RealPokemon> *systemPokes;
    vector<int> loseSelections;
    bool isLoseSelect;
    int loseSelectFlag;
    int curPokeFlag;
    int otherPokeFlag;
    int sysPokeFlag;
    bool receOtherSuccess;
    bool isLevelUpWar;
    bool requireSys;
    bool isWinner;
    bool selectItDel;

    map<string,vector<RealPokemon>> *otherPokes;

    PokePlayer* currentPlayer;
    PokemonClient *mainController;
    RealPokemon *attackerPoke;
    RealPokemon *enemyPoke;
    FightWindow* fightwindow;

    QMediaPlayer *backMusic;
    QMediaPlaylist *backList;
    bool isPlaying;
};

#endif // PLAYUI_H
