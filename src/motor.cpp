#include "motor.h"
#include "config.h"


#include<stdio.h>
#include<math.h>

void motor::init(){
    //temp init
    grain temp;
    
   
    
    temp.init(0.125,0.068,0.03);
    temp.prop.a = 0.000101;
    temp.prop.n = 0.319000;
    temp.prop.alpha = 0.0;
    temp.prop.beta = 0.0;
    temp.prop.combustion_temp = 1720.0;
    temp.prop.density = 1820.23;
    temp.prop.exhaust_molar_mass = 41.98;
    temp.prop.specific_heat_ratio = 1.133000;
    temp.prop.init();

    for(auto i=0;i<6;i++){
        grains.push_back(temp);   
    }
    
    pressure = 200000;
    for(auto i=0;i<grains.size();i++){
        total_volume += grains[i].volume_initial;
    }
    
    pressure_exit = ambient_pressure;
    correction_coeff = 1.0;

    nozz.expansion_ratio = 5.0;
    nozz.throat_diameter = 0.024;

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


    pressure += delta;
    
    double a = (2.0 * grains[0].prop.specific_heat_ratio * grains[0].prop.specific_heat_ratio) /  (grains[0].prop.specific_heat_ratio - 1.0);
    double b = pow((2.0 / (grains[0].prop.specific_heat_ratio + 1.0)), (grains[0].prop.specific_heat_ratio + 1.0) / (grains[0].prop.specific_heat_ratio - 1.0) );
    double c = 1.0 - pow(pressure_exit / pressure, (grains[0].prop.specific_heat_ratio - 1.0) / grains[0].prop.specific_heat_ratio);

    thrust = correction_coeff * throat_area * pressure * sqrt(a * b * c) + (pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio;
    thrust_coeffcient = sqrt(a * b * c) + ((pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio) / (pressure * throat_area);

    impulse += thrust * dT;

    kn = temp_area / throat_area;

    //For steady state
    //pressure = temp * pow(kn, 1.0 / (1.0 - grains[0].prop.n));


    if(pressure >= max_pressure){
        max_pressure = pressure;
    }

    printf("Pressure: %0.2lf MPa    ", pressure/1000000.0);
    printf("Thrust: %0.2lf N    ", thrust);
    printf("Web left: %0.4lf m    ", grains[0].web);
    printf("Mass flux: %0.4lf kg/m^2-s    ", grains[0].mass_flux);
    printf("Kn: %0.2lf ", kn);
    printf("Regression rate: %lf m/s \n", grains[0].burn_rate);


}

void motor::parse(){

    double total_propellant_mass = 0.0;
    for(auto i=0;i<grains.size();i++){
        total_propellant_mass += grains[i].volume_initial * grains[i].prop.density;
    }

    specific_impulse = impulse / (total_propellant_mass * gravity);
    printf("Impulse: %0.2lf Ns\n", impulse);
    printf("Peak pressure: %0.2lf MPa \n", max_pressure/1000000.0);
    printf("Peak mass flux: %0.2lf kg/m^s-s\n", grains[0].max_mass_flux);
    printf("Isp: %0.1lf s \n", specific_impulse);

}