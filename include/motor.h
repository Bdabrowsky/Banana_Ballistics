#pragma once

#include<vector>

#include "grain.h"
#include "tank.h"
#include "nozzle.h"
#include "valve.h"

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

        valve oxidizer_valve;
        valve fuel_valve;

        propellant prop;

        double pressure;
        double volume;
        double pressure_exit;
        double thrust;
        double thrust_coeffcient;
        double last_valid_thrust;
        double characteristic_velocity;

        double impulse;
        double specific_impulse;
        double max_pressure;
        double max_thrust;

        double correction_coeff;
        double ignition_time;


        double previous_dP;
        
        void init();

        void update(double T);

        void update_transient(double T);

        void parse();

};