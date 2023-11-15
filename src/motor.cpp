#include "motor.h"
#include "config.h"


#include<stdio.h>
#include<math.h>

void motor::init(){
    //temp init
    grain temp;
    temp.init(0.1,0.05,0.015);
   
    grains.push_back(temp);

    grains[0].prop.a = 0.000101;
    grains[0].prop.n = 0.319000;
    grains[0].prop.alpha = 0.0;
    grains[0].prop.beta = 0.0;
    grains[0].prop.combustion_temp = 1720.0;
    grains[0].prop.density = 1820.23;
    grains[0].prop.exhaust_molar_mass = 41.98;
    grains[0].prop.specific_heat_ratio = 1.133000;
    grains[0].prop.init();

    
    pressure = 200000;
    total_volume = grains[0].volume_initial;
    pressure_exit = ambient_pressure;

    nozz.expansion_ratio = 4.0;
    nozz.throat_diameter = 0.008;

    for(auto i=0;i<grains.size();i++){
        volume += grains[i].length_initial * grains[i].diameter * grains[i].diameter * PI / 4.0;
    }

}

void motor::update(){

    double temp_volume = 0.0;
    double temp_area = 0.0;

    
    for(auto i=0;i<grains.size();i++){
        grains[i].update(pressure);

        temp_volume += grains[i].volume;
        temp_area += grains[i].area;

        //printf("temp_volume: %lf, temp_area: %lf\n",grains[i].volume,temp_area);
    }
   

    double free_volume = volume - temp_volume;
    double throat_area = (nozz.throat_diameter * nozz.throat_diameter * PI) / 4.0;
   

    double delta = grains[0].prop.density * (gas_constant / grains[0].prop.exhaust_molar_mass) * grains[0].prop.combustion_temp * temp_area * grains[0].prop.a * pow(pressure, grains[0].prop.n);

    delta -= (grains[0].prop.tau * pressure * throat_area * sqrt((gas_constant/grains[0].prop.exhaust_molar_mass) * grains[0].prop.combustion_temp));

    delta *= dT/free_volume;
   
    //For steady state
    //double temp = pow((grains[0].prop.a * grains[0].prop.density) / sqrt(grains[0].prop.specific_heat_ratio / (gas_constant/grains[0].prop.exhaust_molar_mass * grains[0].prop.combustion_temp) * pow(2.0 / (grains[0].prop.specific_heat_ratio + 1.0), (grains[0].prop.specific_heat_ratio + 1.0) / (grains[0].prop.specific_heat_ratio - 1.0))), 1.0 / (1.0 - grains[0].prop.n));

    //printf("delta: %lf\n", delta);

    pressure += delta;
    //pressure = 4000000;
    
    double a = (2.0 * grains[0].prop.specific_heat_ratio * grains[0].prop.specific_heat_ratio) /  (grains[0].prop.specific_heat_ratio - 1.0);
    double b = pow((2.0 / (grains[0].prop.specific_heat_ratio + 1.0)), (grains[0].prop.specific_heat_ratio + 1.0) / (grains[0].prop.specific_heat_ratio - 1.0) );
    double c = 1.0 - pow(pressure_exit / pressure, (grains[0].prop.specific_heat_ratio - 1.0) / grains[0].prop.specific_heat_ratio);

    thrust = throat_area * pressure * sqrt(a * b * c) + (pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio;
    thrust_coeffcient = sqrt(a * b * c) + ((pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio) / (pressure * throat_area);

    impulse += thrust * dT;

    kn = temp_area / throat_area;

    //For steady state
    //pressure = temp * pow(kn, 1.0 / (1.0 - grains[0].prop.n));

    printf("Pressure: %0.2lf MPa    ", pressure/1000000.0);
    printf("Thrust: %0.2lf N    ", thrust);
    printf("Web left: %0.4lf m    ", grains[0].web);
    printf("Kn: %0.2lf ", kn);
    printf("Regression rate: %lf m/s \n", grains[0].burn_rate);


}

void motor::parse(){

    double total_propellant_mass = 0.0;
    for(auto i=0;i<grains.size();i++){
        total_propellant_mass += grains[i].volume_initial * grains[i].prop.density;
    }

    specific_impulse = impulse / (total_propellant_mass * gravity);
    printf("Impulse: %0.2lf \n", impulse);
    printf("Isp: %0.1lf \n", specific_impulse);

}