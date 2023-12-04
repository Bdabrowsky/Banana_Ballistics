#pragma once

#include <string>

using namespace std;

class propellant{
    public:
        double density;
        double specific_heat_ratio;
        double combustion_temp;
        double exhaust_molar_mass;
        double tau;

        double a;
        double n;
        double alpha;
        double beta;

        double max_pressure;
        double min_pressure;

        string type;

        void init(string str);
        void change_coefficients(double pressure);
};