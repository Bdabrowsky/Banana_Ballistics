#include "tank.h"
#include "config.h"

#include<math.h>
#include<stdio.h>
#include<stdio.h>

void tank::init(){

    propellant_mass = propellant.density * volume;   
}

void tank::update(double downstream_pressure){

    mass_flow = Cd * reference_area * sqrt(2 * propellant.density * (pressure - downstream_pressure));
    propellant_mass -= mass_flow * dT;
}