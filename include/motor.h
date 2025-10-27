#pragma once

#include<vector>

#include "grain.h"
#include "tank.h"
#include "nozzle.h"

using namespace std;

class motor{

    public:
        nozzle nozz;

        vector< grain > grains;

        double pressure;
        double volume;
        double total_volume;
        double pressure_exit;
        double thrust;
        double thrust_coeffcient;
        double last_valid_thrust;

        double impulse;
        double specific_impulse;
        double max_pressure;

        double kn;

        double correction_coeff;
        double ignition_time;

        void init();

        void update();

        void update_transient(double T);

        void parse();
    
};

class biprop_engine{

    public:
        nozzle nozzle_d;

        tank oxidizer_tank;
        tank fuel_tank;

        propellant prop;

        double pressure;
        double volume;
        double pressure_exit;
        double thrust;
        double thrust_coeffcient;
        double last_valid_thrust;

        double impulse;
        double specific_impulse;
        double max_pressure;

        double correction_coeff;
        double ignition_time;

        void init();

        void update();

        void update_transient(double T);

        void parse();

};