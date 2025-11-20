#include "valve.h"

#include <stdio.h>
#include <math.h>
#include <string>
#include <iomanip>

#include "config.h"

double valve::update(double mass_flow){
    if(flag_open){
        if(ticks < 0 || ticks > setpoint * (actutation_time / dT)){
            return mass_flow * setpoint;
        }
    }
    else{
        return 0;
    }

    if(flag_direction == 0 && ticks <= 0){
        flag_open = 0;
    }

    if(flag_direction){
        ticks+=1;
    }
    else{
        ticks-=1;
    }

    //printf("Ticks %0.2lf kg/s \n", ticks);
    //printf("Initial mass flow %0.2lf kg/s \n", mass_flow);
    mass_flow = mass_flow * (ticks * dT) / actutation_time;
    //printf("Flow factor %0.2lf kg/s \n", (ticks * dT) / actutation_time);
    //printf("Corrected mass flow %0.2lf kg/s \n", mass_flow);

    return mass_flow;
    
}

void valve::open(double time){
    ticks = 0;
    flag_direction = 1;
    flag_open = 1;
}

void valve::close(double time){
    ticks = (actutation_time / dT);
    flag_direction = 0;
}

void valve::actuate(double i_setpoint){
    setpoint = i_setpoint;
}