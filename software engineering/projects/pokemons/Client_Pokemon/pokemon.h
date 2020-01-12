//
// Created by thief on 16-10-4.
//
#ifndef POKEMON_OF_THIEF_POKEMON_H
#define POKEMON_OF_THIEF_POKEMON_H


#include <string>
//#include <iostream>
#include<vector>

using std::string;

namespace PokeMon {


    //decide how much to add ep
    enum EP_TYPE{SMALL,MEDIUM,BIG,RANDOM,SURPRISE};
    const unsigned int SMALL_EP=10;
    const unsigned int MEDIUM_EP=40;
    const unsigned int BIG_EP=80;
    const unsigned int SURPRISE_EP=1000;
    const unsigned short FULL_LEVEL=15;
    const unsigned int EVERY_LEVEL_BLOOD_UP=50;
    //decide pokemon four main type
    enum POKEMON_TYPE{POWER,BLOOD,FAST,DEFEND};


    const unsigned short LEAST_HURT=1; //least hurt constant
    const unsigned short RANDOM_SMALL=10; //random range

    class PokemonBaby {
    public:
        //todo change args to vector
        PokemonBaby(const string _name, unsigned int _level,const string _roleName,
                     unsigned int _ep, unsigned int _attackPower,
                     unsigned int _defendPower, unsigned int _blood, unsigned int _speed);

        ~PokemonBaby();
        virtual void attack(PokemonBaby * enemy)=0;
        virtual void addPower(int changeLevels)=0;
        virtual void upgrade();
        virtual void changeName(const string newName);
        virtual POKEMON_TYPE getType()=0;
        void addEp(EP_TYPE quantity);
        int normalHurt(PokemonBaby* enemy);
        bool isDead();
        void win(PokemonBaby* enemy);

        //add extra hurt
        virtual int addtionalHurt(PokemonBaby* enemy);
        virtual int specialAttrHurt(PokemonBaby* enemy) =0 ;
        virtual int mainModeHurt(PokemonBaby* enemy) =0 ;
        virtual void randomEventHurt(PokemonBaby* enemy);

        //random generator
        float randomGenerator();

        /*judge upgrade for changeing level
         * experience point compute by
         * when current EP with level n > 20*n*(n-1)
         * n will change to n+1   */
        bool isUpgrade();
        void printBasicInfo();


        const string &getName() const ;
        void setName(const string &name) ;
        const string &getRoleName() const;
        void setRoleName(const string &roleName);
        unsigned int getAttackPower() const;
        void setAttackPower(unsigned int attackPower) ;
        unsigned int getEp() const ;
        void setEp(unsigned int ep);
        unsigned short getLevel() const ;
        void setLevel(unsigned short level);
        unsigned int getDefendPower() const;
        void setDefendPower(unsigned int defendPower) ;
        int getBlood() const ;

        void setNewEp(PokemonBaby* enemy);
        void setBlood(int blood);
        int getAllHurt() const;

        unsigned int getSpeed() const;

        std::vector<int> getAllHurts() const;
        void appendHurts(int a);
        int getHurt();

        void setIsWinner(bool value);

        bool getIsWinner() const;

    protected:
        virtual void updateAttributes()=0;

        std::vector<int> allHurts;
        string name;
        string roleName;
        unsigned short level;   //current level
        unsigned int ep;  //experience point
        unsigned int attackPower; //attack Power
        unsigned int defendPower;  //defensive Power
        int blood;   //health value
        unsigned int speed;   //attack speed
        int allHurt;    // allHurt to enemy at once war
        bool isWinner;

    };


    class PokemonPower : public PokemonBaby {
    public:
        PokemonPower(const string _name, unsigned int _level,const string _roleName,
                     unsigned int _ep, unsigned int _attackPower,
                     unsigned int _defendPower, unsigned int _blood, unsigned int _speed);

        ~PokemonPower();

        void attack(PokemonBaby *enemy);
        void upgrade();
        void addPower(int changeLevels);
        void updateAttributes();
        POKEMON_TYPE getType();

        int specialAttrHurt(PokemonBaby* enemy);
        int mainModeHurt(PokemonBaby* enemy);

    private:
        bool isUpgrade();


        float bigAttackRate;  // a rate for big attacking
    };


    class PokemonBlood:public PokemonBaby
    {
    public:
        PokemonBlood(const string _name, unsigned int _level,const string _roleName,
                     unsigned int _ep, unsigned int _attackPower,
                     unsigned int _defendPower, unsigned int _blood, unsigned int _speed);
        ~PokemonBlood();
        void attack(PokemonBaby* enemy);
        void upgrade();
        void addPower(int changeLevels);
        void updateAttributes();
        POKEMON_TYPE getType();

        int specialAttrHurt(PokemonBaby* enemy);
        int mainModeHurt(PokemonBaby* enemy);


    private:
        bool isUpgrade();
        float drinkRate; //rob blood from enemy
    };

    class PokemonDefend:public PokemonBaby
    {
    public:
        PokemonDefend(const string _name, unsigned int _level,const string _roleName,
                     unsigned int _ep, unsigned int _attackPower,
                     unsigned int _defendPower, unsigned int _blood, unsigned int _speed);
        ~PokemonDefend();
        void attack(PokemonBaby* enemy);
        void upgrade();
        void addPower(int changeLevels);
        void updateAttributes();
        POKEMON_TYPE getType();

        int specialAttrHurt(PokemonBaby* enemy);
        int mainModeHurt(PokemonBaby* enemy);

    private:
        bool isUpgrade();
        float antiDefendRate;  // for attacking enemy when defend
    };

    class PokemonFast:public PokemonBaby
    {
    public:
        PokemonFast(const string _name, unsigned int _level,const string _roleName,
                     unsigned int _ep, unsigned int _attackPower,
                     unsigned int _defendPower, unsigned int _blood, unsigned int _speed);
        ~PokemonFast();
        void attack(PokemonBaby* enemy);
        void upgrade();
        void addPower(int changeLevels);
        void updateAttributes();
        POKEMON_TYPE getType();

        int specialAttrHurt(PokemonBaby* enemy);
        int mainModeHurt(PokemonBaby* enemy);

    private:
        bool isUpgrade();
        float mutiAttackRate;   //for multiple attack
    };
}




#endif //POKEMON_OF_THIEF_POKEMON_H
