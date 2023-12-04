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



motor mot;


int main(){
    ifstream temp("data/configuration.json");
    json config = json::parse(temp);
    temp.close();
   
    mot.init();

    for(double T=0;T<simulationLength;T+=dT){
        if(T >= config.at("motor").at("ignition_time")){
            mot.update();
        }
        else{
            mot.update_transient(T);
        }
       
        if(mot.grains[0].web <= 0.001){
            break;
        }

        ofstream temp("output.csv", ios::out | std::ofstream::app);
        temp << T << ",";
        temp.close();
    }

    mot.parse();
   
  

}