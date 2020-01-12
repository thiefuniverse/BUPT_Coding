#ifndef PLANCHANGE_H
#define PLANCHANGE_H

#include<traveller.h>
class PlanChange
{
public:
    /*计划改变构造函数*/
    PlanChange();

    /*改变途经城市*/
    bool changePassCities(Traveller customer);


    /*改变终点城市*/
    bool changeDestination(Traveller& customer);

private:



};

#endif // PLANCHANGE_H
