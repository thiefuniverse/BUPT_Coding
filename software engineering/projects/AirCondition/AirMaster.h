//
// Created by thief on 17-5-21.
//

#ifndef AIR_CONDITION_AIRMASTER_H
#define AIR_CONDITION_AIRMASTER_H

#include "json.hpp"

using json= nlohmann::json;
using TemperatureType= float;


class AirMaster {
public:
    //空调工作模式
    enum AIR_WORK_MODE{COLD,HOT};

    void start();
    void stop();

    // getter and setter
    void setWorkMode(AIR_WORK_MODE mode);

private:

    AIR_WORK_MODE currentMode;
    TemperatureType defaTemperature;

};


#endif //AIR_CONDITION_AIRMASTER_H
