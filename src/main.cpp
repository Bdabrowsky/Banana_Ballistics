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
#include "valve.h"




using json = nlohmann::json;


using namespace std;

double time_burnout = 0;
bool flag_burnout =0; 
biprop_engine mot;


int main(){
    ifstream temp("data/configuration.json");
    json config = json::parse(temp);
    temp.close();
   
    mot.init();

  
    for(double T=0;T<simulationLength;T+=dT){
        if(mot.oxidizer_tank.propellant_mass <= 0){
            if(!flag_burnout){
                time_burnout = T;
                flag_burnout = 1;
            }
        }

        if(flag_burnout && (T - time_burnout) > 0.9){
            break;
        }

        if(T == 0.0){
            mot.oxidizer_tank.propellant_valve.open(0);
            mot.oxidizer_tank.propellant_valve.actuate(1);
        }
        if(T >= 4.5 && T <= 4.51){
            mot.fuel_tank.propellant_valve.open(0);
            mot.fuel_tank.propellant_valve.actuate(1);
        }

        if(T >= 8.0 && T <=8.01){
            mot.fuel_tank.propellant_valve.close(0);
            mot.fuel_tank.propellant_valve.actuate(0.8);
            mot.oxidizer_tank.propellant_valve.close(0);
            mot.oxidizer_tank.propellant_valve.actuate(0.8);
        }

        if(T >= 15.0 && T <=15.01){
            mot.fuel_tank.propellant_valve.close(0);
            mot.fuel_tank.propellant_valve.actuate(0.6);
            mot.oxidizer_tank.propellant_valve.close(0);
            mot.oxidizer_tank.propellant_valve.actuate(0.6);
        }

        if(T >= 18.0 && T <=18.01){
            mot.fuel_tank.propellant_valve.close(0);
            mot.fuel_tank.propellant_valve.actuate(0.35);
            mot.oxidizer_tank.propellant_valve.close(0);
            mot.oxidizer_tank.propellant_valve.actuate(0.35);
        }

        if(T >= 23.0 && T <=23.01){
            mot.fuel_tank.propellant_valve.close(0);
            mot.fuel_tank.propellant_valve.actuate(0.15);
            mot.oxidizer_tank.propellant_valve.close(0);
            mot.oxidizer_tank.propellant_valve.actuate(0.15);
        }

        if(T >= 25.0 && T <=25.01){
            mot.fuel_tank.propellant_valve.close(0);
            mot.fuel_tank.propellant_valve.actuate(0.0);
            mot.oxidizer_tank.propellant_valve.close(0);
            mot.oxidizer_tank.propellant_valve.actuate(0.0);
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

        mot.update(T);
    }

    mot.parse();
   
  

}