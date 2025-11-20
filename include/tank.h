#pragma once

#include "propellant.h"
#include "valve.h"

#include <string>

using namespace std;

class tank{
    public:
        double volume;      //m^3
        double pressure;    //Pa
        double Cd;
        double reference_area; //m^2

        string propellant_type;

        double mass_flow;
        double propellant_mass;

        propellant propellant;
        valve propellant_valve;

        
        void init();
        void update(double downstream_pressure);
};

