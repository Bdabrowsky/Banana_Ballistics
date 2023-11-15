#include<cinttypes>
#include<math.h>
#include<stdio.h>

#include "config.h"
#include "motor.h"
#include "grain.h"
#include "propellant.h"

using namespace std;



motor mot;


int main(){

    
    mot.init();

    for(double T=0;T<simulationLength;T+=dT){
        mot.update();
        if(mot.grains[0].web <= 0.001){
            break;
        }
        printf("Time: %0.3lf s   ", T);
    }

    mot.parse();

}