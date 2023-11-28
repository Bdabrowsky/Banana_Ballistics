#include "propellant.h"


#include <nlohmann/json.hpp>
#include <string.h>
#include <math.h>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using namespace std;

void propellant::init(){

    ifstream temp("data/Propellants.json");
    json propellants = json::parse(temp);

    //cout << setw(4) << propellants << '\n';
    
    density = propellants.at("KNSB").at("density");
    specific_heat_ratio = propellants.at("KNSB").at("specific_heat_ratio");
    combustion_temp = propellants.at("KNSB").at("combustion_temp");
    exhaust_molar_mass = propellants.at("KNSB").at("exhaust_molar_mass");
    a = propellants.at("KNSB").at("a");
    n = propellants.at("KNSB").at("n");
    alpha = propellants.at("KNSB").at("alpha");
    beta = propellants.at("KNSB").at("beta");
    max_pressure = propellants.at("KNSB").at("max_pressure");
    min_pressure = propellants.at("KNSB").at("min_pressure");

    temp.close();
    

    tau = sqrt(specific_heat_ratio) * pow( (2.0 / (specific_heat_ratio + 1.0)), (specific_heat_ratio + 1.0) / (2.0 * (specific_heat_ratio - 1.0)));
    
}
