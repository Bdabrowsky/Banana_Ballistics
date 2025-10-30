#include<cinttypes>
#include<math.h>
#include<stdio.h>
#include <nlohmann/json.hpp>
#include<string.h>

#include "config.h"
#include "motor.h"
#include "grain.h"
#include "propellant.h"
#include "io.h"




using json = nlohmann::json;


using namespace std;



biprop_engine mot;


int main(){
    ifstream temp("data/configuration.json");
    json config = json::parse(temp);
    temp.close();
   
    mot.init();

    for(double T=0;T<simulationLength;T+=dT){
        mot.update();
       
        if(mot.oxidizer_tank.propellant_mass <= 0 && mot.fuel_tank.propellant_mass <= 0){
            break;
        }

        #ifdef bipropellant_engine
            ofstream temp("biprop_output.csv", ios::out | std::ofstream::app);
        #endif
        #ifdef solid_motor
            ofstream temp("solid_output.csv", ios::out | std::ofstream::app);
        #endif
        #ifdef hybrid_motor
            ofstream temp("hybrid_output.csv", ios::out | std::ofstream::app);
        #endif
        temp << T << ",";
        temp.close();
    }

    mot.parse();
   
  

}