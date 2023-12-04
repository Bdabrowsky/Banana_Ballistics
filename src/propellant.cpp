#include "propellant.h"


#include <nlohmann/json.hpp>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using namespace std;

void propellant::init(string str){

    ifstream temp("data/Propellants.json");
    json propellants = json::parse(temp);

    //cout << setw(4) << propellants << '\n';
    type = str;
    density = propellants.at(str).at("density");
    specific_heat_ratio = propellants.at(str).at("specific_heat_ratio");
    combustion_temp = propellants.at(str).at("combustion_temp");
    exhaust_molar_mass = propellants.at(str).at("exhaust_molar_mass");
    a = propellants.at(str).at("a");
    n = propellants.at(str).at("n");
    alpha = propellants.at(str).at("alpha");
    beta = propellants.at(str).at("beta");
    max_pressure = propellants.at(str).at("max_pressure");
    min_pressure = propellants.at(str).at("min_pressure");

    temp.close();
    

    tau = sqrt(specific_heat_ratio) * pow( (2.0 / (specific_heat_ratio + 1.0)), (specific_heat_ratio + 1.0) / (2.0 * (specific_heat_ratio - 1.0)));
    
}

void propellant::change_coefficients(double pressure){
    ifstream temp("data/Propellants.json");
    json propellants = json::parse(temp);

    int range_count = propellants.at(type).at("range_count");
    for(auto i=0;i < range_count;i++){
        double low = propellants.at(type).at("ranges").at(i).at("low");
        double high = propellants.at(type).at("ranges").at(i).at("high");
        if(high < 0){
            high = 1e9;
        }
        if(pressure / 1000000.0 >= low && pressure / 1000000.0 < high){
            a = propellants.at(type).at("ranges").at(i).at("a");
            n = propellants.at(type).at("ranges").at(i).at("n");
            break;
        }
    }

    temp.close();
}
