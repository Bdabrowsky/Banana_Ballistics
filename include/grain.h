#pragma once

#include"propellant.h"

#include <string>


using namespace std;

class grain{
    public:
        double diameter;    //m
        double length;      //m
        double port_diameter; //m

        string propellant_type;

        double volume;      //m^3
        double area;        //m^2
        double web;         //m

        double volume_initial;
        double length_initial;

        double mass_flux;
        double mass_flow;
        double burn_rate;
        double max_mass_flux;

        propellant prop;

        
        void init(double len, double dia, double port_dia);
        void update(double pressure);
};