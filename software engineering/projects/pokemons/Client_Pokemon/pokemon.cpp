//
// Created by thief on 16-10-4.
//

#include <iostream>
#include "pokemon.h"
#include <ctime>
//#include <stdio.h>
#include <stdlib.h>
//#include <cstdlib>



using namespace std;
namespace PokeMon
{

    //PokeMonBaby

    PokemonBaby::PokemonBaby(const string _name, unsigned int _level,const string _roleName, unsigned int _ep, unsigned int _attackPower,
                             unsigned int _defendPower, unsigned int _blood, unsigned int _speed)
            :name(_name),level(_level),roleName(_roleName), ep(_ep),attackPower(_attackPower),
             defendPower(_defendPower),blood(_blood),speed(_speed)
    {

    }

    PokemonBaby::~PokemonBaby() {

    }

    void PokemonBaby::attack(PokemonBaby *enemy) {
    }

    void PokemonBaby::changeName(const string newName) {
        this->name=newName;
    }

    void PokemonBaby::printBasicInfo() {
        std::cout<<"----------------------------------------\n";
        std::cout<<"Pokemon Name: "<<this->getName()<<"\n";
        std::cout<<"Pokemon Level: "<<this->getLevel()<<"\n";
        std::cout<<"Pokemon RoleName: "<<this->getRoleName()<<"\n";
        std::cout<<"Pokemon EP: "<<this->getEp()<<"\n";
        std::cout<<"Pokemon AP: "<<this->getAttackPower()<<"\n";
        std::cout<<"Pokemon DP: "<<this->getDefendPower()<<"\n";
        std::cout<<"Pokemon Blood: "<<this->getBlood()<<"\n";
        std::cout<<"----------------------------------------\n";
    }

    bool PokemonBaby::isUpgrade() {
        if (this->ep>20*level*(level-1))
        {
            return true;
        }
        return false;
    }

    void PokemonBaby::upgrade() {

        int changeLevels=0;
        for (int i=level;this->ep>20*i*(i-1);++i){
            changeLevels++;
        }

        if (level+changeLevels>=FULL_LEVEL)
        {
            changeLevels=FULL_LEVEL-level;
            level=FULL_LEVEL;
            //todo get full level
        }
        else
        {
            level+=changeLevels;
        }
        this->updateAttributes();
        this->addPower(changeLevels);

    }

    void PokemonBaby::addEp(EP_TYPE quantity) {
       switch (quantity)
       {
           case EP_TYPE ::SMALL:
               this->ep+=SMALL_EP;
               break;
           case EP_TYPE ::MEDIUM:
               this->ep+=MEDIUM_EP;
               break;
           case EP_TYPE ::BIG:
               this->ep+=BIG_EP;
               break;
           case EP_TYPE ::RANDOM:
               this->ep+=5;
               //todo
           case EP_TYPE ::SURPRISE:
               this->ep+=SURPRISE_EP;
               break;
           default:
               break;
       }
    }

    int PokemonBaby::normalHurt(PokemonBaby *enemy)
    {
        return (int)(attackPower*((float)enemy->getDefendPower()/(float)(attackPower+enemy->getDefendPower())));
    }



    // attributes functions
    const string &PokemonBaby::getName() const {
        return name;
    }

    void PokemonBaby::setName(const string &name) {
        this->name = name;
    }

    unsigned int PokemonBaby::getAttackPower() const {
        return attackPower;
    }

    void PokemonBaby::setAttackPower(unsigned int attackPower) {
        this->attackPower = attackPower;
    }

    unsigned int PokemonBaby::getEp() const {
        return ep;
    }

    void PokemonBaby::setEp(unsigned int ep) {
        this->ep = ep;
        this->upgrade();
    }

    unsigned short PokemonBaby::getLevel() const {
        return level;
    }

    void PokemonBaby::setLevel(unsigned short level) {
        this->level = level;
    }

    unsigned int PokemonBaby::getDefendPower() const {
        return defendPower;
    }

    void PokemonBaby::setDefendPower(unsigned int defendPower) {
        this->defendPower = defendPower;
    }

    int PokemonBaby::getBlood() const {
        return blood;
    }

    void PokemonBaby::setNewEp(PokemonBaby *enemy)
    {
        int addEp=attackPower*((float)getHurt()/(float)(getHurt()+enemy->getHurt()));
        ep+=addEp;
        PokemonBaby::upgrade();
    }

    void PokemonBaby::setBlood(int blood) {
        if (blood<=0)
            blood=0;
        this->blood = blood;
    }

    int PokemonBaby::getAllHurt() const
    {
        return allHurt;
    }

    unsigned int PokemonBaby::getSpeed() const
    {
        return speed;
    }

    vector<int> PokemonBaby::getAllHurts() const
    {
        return std::move(allHurts);
    }

    void PokemonBaby::appendHurts(int a)
    {
        allHurts.push_back(a);
    }

    int PokemonBaby::getHurt()
    {
        int sum=0;
        for(auto m:allHurts)
        {
            sum+=m;
        }
        return sum;
    }

    void PokemonBaby::setIsWinner(bool value)
    {
        isWinner = value;
    }

    bool PokemonBaby::getIsWinner() const
    {
        return isWinner;
    }

    const string &PokemonBaby::getRoleName() const {
        return this->roleName;
    }

    void PokemonBaby::setRoleName(const string &roleName) {
        this->roleName=roleName;
    }

    bool PokemonBaby::isDead() {
        return blood<=0;
    }

    void PokemonBaby::win(PokemonBaby *enemy) {
        std::cout<<enemy->getRoleName()+"  "+enemy->getName()+"  is dead!\n";
        std::cout<<this->getRoleName()+"  "+this->getName()+" win!\n";
    }

    void PokemonBaby::randomEventHurt(PokemonBaby *enemy) {

    }

    float PokemonBaby::randomGenerator() {

        return (float)(rand()%RANDOM_SMALL)/100;
    }

    int PokemonBaby::addtionalHurt(PokemonBaby *enemy) {
            int hurt=0;
            hurt+=this->specialAttrHurt(enemy);
            hurt+=this->mainModeHurt(enemy);
            return hurt;
    }


    //PokeMonPower
    PokemonPower::PokemonPower(const string _name, unsigned int _level,const string _roleName, unsigned int _ep, unsigned int _attackPower,
                               unsigned int _defendPower, unsigned int _blood, unsigned int _speed)
            :PokemonBaby(_name,_level,_roleName, _ep,_attackPower,_defendPower,_blood,_speed)
    {

    }


    void PokemonPower::attack(PokemonBaby* enemy) {
        int hurt=this->normalHurt(enemy);
        if (hurt>0)
        {
            if(enemy->getBlood()<hurt)
            {
                enemy->setBlood(0);
            }
            else
            {
                enemy->setBlood(enemy->getBlood()-hurt);
            }

        }
        else
        {
            enemy->setBlood(enemy->getBlood()-LEAST_HURT);
            hurt=LEAST_HURT;
        }
        hurt+=addtionalHurt(enemy);
        enemy->appendHurts(hurt);

        allHurt+=hurt;
        std::cout<<this->getName()+"  attack "+enemy->getName()+"  cause "<<hurt<<" point hurt ";
        std::cout<<enemy->getName()+"'s blood drop to ";
        std::cout<<enemy->getBlood()<<"\n";


        if (enemy->isDead())
        {
           this->win(enemy);
            allHurt=0;
            this->setNewEp(enemy);
            enemy->setNewEp(this);
            this->setIsWinner(true);
        }
        else
        {
            enemy->attack(this);
            allHurt=0;
            this->setIsWinner(false);
        }
    }

    PokemonPower::~PokemonPower() {

    }

    void PokemonPower::upgrade() {
        PokemonBaby::upgrade();
    }

    void PokemonPower::addPower(int changeLevels) {
        attackPower+=changeLevels*10;
        defendPower+=changeLevels*5;
        blood+=changeLevels*EVERY_LEVEL_BLOOD_UP;
        std::cout<<"attack and defend strongly and blood back to complete.\n";
    }

    POKEMON_TYPE PokemonPower::getType() {
        return POWER;
    }

    void PokemonPower::updateAttributes() {
        this->setBlood(EVERY_LEVEL_BLOOD_UP*this->getLevel());
    }

    int PokemonPower::specialAttrHurt(PokemonBaby *enemy) {
        bigAttackRate=(float)level/50;
        int addHurt=this->getAttackPower()*(randomGenerator()+bigAttackRate);
        return addHurt;
    }

    int PokemonPower::mainModeHurt(PokemonBaby *enemy) {
        //todo
        return 0;
    }



//PokemonBlood

    PokeMon::PokemonBlood::PokemonBlood(const string _name, unsigned int _level, const string _roleName, unsigned int _ep,                                        unsigned int _attackPower,
                                        unsigned int _defendPower, unsigned int _blood, unsigned int _speed)
            :PokemonBaby(_name,_level,_roleName, _ep,_attackPower,_defendPower,_blood,_speed)
    {

    }

    PokeMon::PokemonBlood::~PokemonBlood() {

    }

    void PokeMon::PokemonBlood::attack(PokemonBaby *enemy) {
        int hurt=this->normalHurt(enemy);
        if (hurt>0)
        {
            if(enemy->getBlood()<hurt)
            {
                enemy->setBlood(0);
            }
            else
            {
                enemy->setBlood(enemy->getBlood()-hurt);
            }

        }
        else
        {
            enemy->setBlood(enemy->getBlood()-LEAST_HURT);
            hurt=LEAST_HURT;
        }
        hurt+=this->specialAttrHurt(enemy);
        allHurt+=hurt;
        enemy->appendHurts(hurt);

        std::cout<<this->getName()+"  attack "+enemy->getName()+"  cause "<<hurt<<" point hurt ";
        std::cout<<enemy->getName()+"'s blood drop to ";
        std::cout<<enemy->getBlood()<<"\n";


        if (enemy->isDead())
        {
            this->win(enemy);
            this->setNewEp(enemy);
            enemy->setNewEp(this);
            allHurt=0;
            this->setIsWinner(true);

        }
        else
        {
            enemy->attack(this);
            allHurt=0;
            this->setIsWinner(false);
        }
    }

    void PokeMon::PokemonBlood::upgrade() {
        PokemonBaby::upgrade();
    }

    void PokeMon::PokemonBlood::addPower(int changeLevels) {
        attackPower+=changeLevels*5;
        defendPower+=changeLevels*5;
        blood+=changeLevels*90;
    }

    PokeMon:: POKEMON_TYPE PokeMon::PokemonBlood::getType() {
        return BLOOD;
    }

    void PokemonBlood::updateAttributes() {
        this->setBlood(this->getLevel()*90);
    }

    int PokemonBlood::specialAttrHurt(PokemonBaby *enemy) {
        drinkRate=(float)level/50;
        int addBlood=this->getAttackPower()*(randomGenerator()+drinkRate);
        return addBlood;
    }

    int PokemonBlood::mainModeHurt(PokemonBaby *enemy) {
        return 0;
    }


    PokeMon::PokemonDefend::PokemonDefend(const string _name, unsigned int _level, const string _roleName, unsigned int _ep,
                                          unsigned int _attackPower, unsigned int _defendPower, unsigned int _blood, unsigned int _speed)
            :PokemonBaby(_name,_level,_roleName, _ep,_attackPower,_defendPower,_blood,_speed)
    {

    }

    PokemonDefend::~PokemonDefend() {

    }

    void PokeMon::PokemonDefend::attack(PokemonBaby *enemy) {
        int hurt=this->normalHurt(enemy);
        if (hurt>0)
        {
            if(enemy->getBlood()<hurt)
            {
                enemy->setBlood(0);
            }
            else
            {
                enemy->setBlood(enemy->getBlood()-hurt);
            }

        }
        else
        {
            enemy->setBlood(enemy->getBlood()-LEAST_HURT);
            hurt=LEAST_HURT;
        }
         hurt+=this->specialAttrHurt(enemy);
        allHurt+=hurt;
         enemy->appendHurts(hurt);

        std::cout<<this->getName()+"  attack "+enemy->getName()+"  cause "<<hurt<<" point hurt ";
        std::cout<<enemy->getName()+"'s blood drop to ";
        std::cout<<enemy->getBlood()<<"\n";


        if (enemy->isDead())
        {
         this->win(enemy);
            this->setNewEp(enemy);
            enemy->setNewEp(this);
            allHurt=0;
            this->setIsWinner(true);
        }
        else
        {
            enemy->attack(this);
            allHurt=0;
            this->setIsWinner(false);
        }
    }

    void PokeMon::PokemonDefend::upgrade() {
        PokemonBaby::upgrade();
    }

    void PokeMon::PokemonDefend::addPower(int changeLevels) {
        attackPower+=changeLevels*5;
        defendPower+=changeLevels*10;
        blood+=changeLevels*EVERY_LEVEL_BLOOD_UP;
    }

    POKEMON_TYPE PokeMon::PokemonDefend::getType() {
        return DEFEND;
    }

    void PokemonDefend::updateAttributes() {
        this->setBlood(EVERY_LEVEL_BLOOD_UP*this->getLevel());
    }

    int PokemonDefend::specialAttrHurt(PokemonBaby *enemy) {
        antiDefendRate=(float)level/50;
        int addDefend=(float)this->getAttackPower()*(randomGenerator()+antiDefendRate);
        return addDefend;
    }

    int PokemonDefend::mainModeHurt(PokemonBaby *enemy) {
        return 0;
    }


    PokeMon::PokemonFast::PokemonFast(const string _name, unsigned int _level, const string _roleName, unsigned int _ep,
                                      unsigned int _attackPower, unsigned int _defendPower, unsigned int _blood, unsigned int _speed)
            :PokemonBaby(_name,_level,_roleName, _ep,_attackPower,_defendPower,_blood,_speed)
    {

    }

    PokeMon::PokemonFast::~PokemonFast() {

    }

    void PokeMon::PokemonFast::attack(PokemonBaby *enemy) {
        int hurt=this->normalHurt(enemy);
        if (hurt>0)
        {
            if(enemy->getBlood()<hurt)
            {
                enemy->setBlood(0);
            }
            else
            {
                enemy->setBlood(enemy->getBlood()-hurt);
            }

        }
        else
        {
            enemy->setBlood(enemy->getBlood()-LEAST_HURT);
             hurt=LEAST_HURT;
        }
         hurt+=this->specialAttrHurt(enemy);
        allHurt+=hurt;
         enemy->appendHurts(hurt);
        std::cout<<this->getName()+"  attack "+enemy->getName()+"  cause "<<hurt<<" point hurt ";
        std::cout<<enemy->getName()+"'s blood drop to ";
        std::cout<<enemy->getBlood()<<"\n";


        if (enemy->isDead())
        {
            this->win(enemy);
            this->setNewEp(enemy);
            enemy->setNewEp(this);
            allHurt=0;
            this->setIsWinner(true);
        }
        else
        {
            enemy->attack(this);
            allHurt=0;
            this->setIsWinner(false);
        }
    }

    void PokeMon::PokemonFast::upgrade() {
            PokemonBaby::upgrade();
    }

    void PokeMon::PokemonFast::addPower(int changeLevels) {
        attackPower+=changeLevels*5;
        defendPower+=changeLevels*5;
        blood+=changeLevels*EVERY_LEVEL_BLOOD_UP;
    }

    POKEMON_TYPE PokeMon::PokemonFast::getType() {
        return FAST;
    }

    void PokemonFast::updateAttributes() {
        this->setBlood(this->getLevel()*EVERY_LEVEL_BLOOD_UP);
    }

    int PokemonFast::specialAttrHurt(PokemonBaby *enemy) {
        mutiAttackRate=(float)level/50;
        int addHurt=this->getAttackPower()*(randomGenerator()+mutiAttackRate);
        return addHurt;
    }

    int PokemonFast::mainModeHurt(PokemonBaby *enemy) {
        return 0;
    }


}


