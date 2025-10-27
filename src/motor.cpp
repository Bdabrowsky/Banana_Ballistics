#include "motor.h"
#include "config.h"
#include "io.h"
 
#include <stdio.h>
#include <math.h>
#include <string>
#include <iomanip>
#include <nlohmann/json.hpp>
#include <fstream>


using namespace std;


void motor::init(){
    ifstream temp("data/configuration.json");
    json config = json::parse(temp);
   
    int cnt = config.at("motor").at("grain_count");
    printf("%d\n", cnt);
    for(auto i=0;i<cnt;i++){
        grain tempGrain;

        tempGrain.diameter = config.at("motor").at("grains").at(i).at("diameter");
        tempGrain.length = config.at("motor").at("grains").at(i).at("length");
        tempGrain.port_diameter = config.at("motor").at("grains").at(i).at("port_diameter");
        tempGrain.propellant_type = config.at("motor").at("grains").at(i).at("propellant_type");
        printf("%lf, %lf, %lf\n",tempGrain.diameter, tempGrain.length, tempGrain.port_diameter);
        tempGrain.init(tempGrain.length,  tempGrain.diameter,  tempGrain.port_diameter);
        tempGrain.prop.init(tempGrain.propellant_type);
        grains.push_back(tempGrain);   
    }
    
    pressure = ambient_pressure * 1.25;
    for(unsigned long i=0;i<grains.size();i++){
        total_volume += grains[i].volume_initial;
    }
    
    pressure_exit = ambient_pressure;


    correction_coeff = config.at("motor").at("correction_coeff");
    ignition_time = config.at("motor").at("ignition_time");

    nozz.throat_diameter = config.at("motor").at("nozzle").at("throat_diameter");
    nozz.expansion_ratio = config.at("motor").at("nozzle").at("expansion_ratio");
    nozz.angle_div = config.at("motor").at("nozzle").at("angle_div");
    nozz.angle_conv = config.at("motor").at("nozzle").at("angle_conv");
    nozz.erosion_coef = config.at("motor").at("nozzle").at("erosion_coef");
    nozz.buildup_coef = config.at("motor").at("nozzle").at("buildup_coef");

    for(unsigned long i=0;i<grains.size();i++){
        volume += grains[i].length_initial * grains[i].diameter * grains[i].diameter * PI / 4.0;
    }

    temp.close();
}

void motor::update_transient(double T){
    double temp_volume = 0.0;
    double temp_area = 0.0;

    
    for(auto i=0;i<grains.size();i++){
        grains[i].update(pressure);
        if(i>0){
            grains[i].mass_flow = grains[i-1].mass_flow;
        }
        else{
            grains[i].mass_flow = 0;
        }
       
        

        temp_volume += grains[i].volume;
        temp_area += grains[i].area;
    }
   
   temp_area *= T / ignition_time;

    double free_volume = volume - temp_volume;
    double throat_area = (nozz.throat_diameter * nozz.throat_diameter * PI) / 4.0;
   

    double delta = grains[0].prop.density * (gas_constant / grains[0].prop.exhaust_molar_mass) * grains[0].prop.combustion_temp * temp_area * grains[0].prop.a * pow(pressure, grains[0].prop.n);

    delta -= (grains[0].prop.tau * pressure * throat_area * sqrt((gas_constant/grains[0].prop.exhaust_molar_mass) * grains[0].prop.combustion_temp));

    delta *= dT/free_volume;
   
    pressure += delta;
    
    double a = (2.0 * grains[0].prop.specific_heat_ratio * grains[0].prop.specific_heat_ratio) /  (grains[0].prop.specific_heat_ratio - 1.0);
    double b = pow((2.0 / (grains[0].prop.specific_heat_ratio + 1.0)), (grains[0].prop.specific_heat_ratio + 1.0) / (grains[0].prop.specific_heat_ratio - 1.0) );
    double c = 1.0 - pow(pressure_exit / pressure, (grains[0].prop.specific_heat_ratio - 1.0) / grains[0].prop.specific_heat_ratio);

    thrust = correction_coeff * throat_area * pressure * sqrt(a * b * c) + (pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio;
    if(!isnan(thrust)){
        last_valid_thrust = thrust;
        impulse += thrust * dT;
    }
    else{
        thrust = last_valid_thrust;
    }
    thrust_coeffcient = sqrt(a * b * c) + ((pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio) / (pressure * throat_area);

    kn = temp_area / throat_area;

    if(pressure >= max_pressure){
        max_pressure = pressure;
    }
    
    ofstream temp("output.csv", ios::out | std::ofstream::app);
    temp << setprecision(5) << pressure/1000000.0 << ",";
    temp << setprecision(5) << thrust << ",";
    for(auto i=0;i<grains.size();i++){
        temp << setprecision(5) << grains[i].web << ",";
        temp << setprecision(5) << grains[i].mass_flow << ",";
        temp << setprecision(5) << grains[i].mass_flux << ",";
    }
    temp << setprecision(5) << kn << ",";
    temp << setprecision(5) << grains[0].burn_rate << "\n";
    
    temp.close();
}

void motor::update(){

    double temp_volume = 0.0;
    double temp_area = 0.0;

    
    for(auto i=0;i<grains.size();i++){
        if(i>0){
            grains[i].mass_flow = grains[i-1].mass_flow;
        }
        else{
            grains[i].mass_flow = 0;
        }
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
    if(!isnan(thrust)){
        last_valid_thrust = thrust;
        impulse += thrust * dT;
    }
    else{
        thrust = last_valid_thrust;
    }
    thrust_coeffcient = sqrt(a * b * c) + ((pressure_exit - ambient_pressure) * throat_area * nozz.expansion_ratio) / (pressure * throat_area);


    kn = temp_area / throat_area;

    //For steady state
    //pressure = temp * pow(kn, 1.0 / (1.0 - grains[0].prop.n));


    if(pressure >= max_pressure){
        max_pressure = pressure;
    }
    

    ofstream temp("output.csv", ios::out | std::ofstream::app);
    temp << setprecision(5) << pressure/1000000.0 << ",";
    temp << setprecision(5) << thrust << ",";
    for(auto i=0;i<grains.size();i++){
        temp << setprecision(5) << grains[i].web << ",";
        temp << setprecision(5) << grains[i].mass_flow << ",";
        temp << setprecision(5) << grains[i].mass_flux << ",";
    }
    temp << setprecision(5) << kn << ",";
    temp << setprecision(5) << grains[0].burn_rate << "\n";
    
    temp.close();

}

void motor::parse(){

    double total_propellant_mass = 0.0;
    for(auto i=0;i<grains.size();i++){
        total_propellant_mass += grains[i].volume_initial * grains[i].prop.density;
    }

    specific_impulse = impulse / (total_propellant_mass * gravity);
    printf("Impulse: %0.2lf Ns\n", impulse);
    printf("Peak pressure: %0.2lf MPa \n", max_pressure/1000000.0);
    double mx=0.0;
    for(auto i=0;i<grains.size();i++){
        if(grains[i].max_mass_flux > mx){
            mx = grains[i].max_mass_flux;
        }
    }
    printf("Peak mass flux: %0.2lf kg/m^s-s\n", mx);
    printf("Propellant mass: %lf\n", total_propellant_mass);
    printf("Isp: %0.1lf s \n", specific_impulse);

}


// 
// Liquid rocket engine
//


void biprop_engine::init(){
    ifstream temp("data/configuration.json");
    json config = json::parse(temp);

    pressure = ambient_pressure;
    pressure_exit = ambient_pressure;

    volume = config.at("biprop_engine").at("volume");
   
    oxidizer_tank.volume = config.at("biprop_engine").at("oxidizer_tank").at("volume");
    oxidizer_tank.pressure = config.at("biprop_engine").at("oxidizer_tank").at("pressure");
    oxidizer_tank.Cd = config.at("biprop_engine").at("oxidizer_tank").at("Cd");
    oxidizer_tank.reference_area = config.at("biprop_engine").at("oxidizer_tank").at("reference_area");
    oxidizer_tank.propellant.init(config.at("biprop_engine").at("oxidizer_tank").at("propellant_type"));

    fuel_tank.volume = config.at("biprop_engine").at("fuel_tank").at("volume");
    fuel_tank.pressure = config.at("biprop_engine").at("fuel_tank").at("pressure");
    fuel_tank.Cd = config.at("biprop_engine").at("fuel_tank").at("Cd");
    fuel_tank.reference_area = config.at("biprop_engine").at("fuel_tank").at("reference_area");
    fuel_tank.propellant.init(config.at("biprop_engine").at("fuel_tank").at("propellant_type"));

    prop.init(config.at("biprop_engine").at("propellant_type"));
    oxidizer_tank.init();
    fuel_tank.init();
    
    correction_coeff = config.at("motor").at("correction_coeff");
    ignition_time = config.at("motor").at("ignition_time");

    nozzle_d.throat_diameter = config.at("motor").at("nozzle").at("throat_diameter");
    nozzle_d.expansion_ratio = config.at("motor").at("nozzle").at("expansion_ratio");
    nozzle_d.angle_div = config.at("motor").at("nozzle").at("angle_div");
    nozzle_d.angle_conv = config.at("motor").at("nozzle").at("angle_conv");
    nozzle_d.erosion_coef = config.at("motor").at("nozzle").at("erosion_coef");
    nozzle_d.buildup_coef = config.at("motor").at("nozzle").at("buildup_coef");

    temp.close();
}

void biprop_engine::update_transient(double T){
    double temp_volume = 0.0;
    double temp_area = 0.0;

    
}

void biprop_engine::update(){

    double temp_volume = 0.0;
    double temp_area = 0.0;  
    double throat_area = (nozzle_d.throat_diameter * nozzle_d.throat_diameter * PI) / 4.0;

    //Termochemical equation for liquids
    if(isnan(pressure)){
        pressure = ambient_pressure;
    }

    oxidizer_tank.update(pressure);
    fuel_tank.update(pressure);

    double total_mass_flow = oxidizer_tank.mass_flow + fuel_tank.mass_flow;
    double OF = oxidizer_tank.mass_flow / fuel_tank.mass_flow;
    printf("Total mass flow %0.2lf \n", total_mass_flow);
    printf("OF %0.2lf \n", OF);

    double dP = gas_constant/1000.0 * prop.combustion_temp / volume * total_mass_flow - pressure * (throat_area/volume * sqrt(prop.specific_heat_ratio * gas_constant * pow(2.0/(prop.specific_heat_ratio + 1),(prop.specific_heat_ratio+1)/(prop.specific_heat_ratio-1))));
    
    
    //printf("dP/dT %0.2lf Pa/s \n", dP);
    //printf("Pressure %0.2lf Pa \n", pressure);



    pressure += dP * dT;
    thrust = pressure * throat_area * 1.38;

    if(!isnan(thrust)){
        last_valid_thrust = thrust;
    }
    else{
        thrust = last_valid_thrust;
    }
    impulse += thrust * dT;
    
   
    ofstream temp("biprop_output.csv", ios::out | std::ofstream::app);
    temp << setprecision(5) << pressure/1000000.0 << ",";
    temp << setprecision(5) << thrust << ",";
    temp << setprecision(5) << total_mass_flow << ",";
    temp << setprecision(5) << oxidizer_tank.propellant_mass << ",";
    temp << setprecision(5) << fuel_tank.propellant_mass << "\n";
    temp.close();

}

void biprop_engine::parse(){

    double total_propellant_mass = oxidizer_tank.volume * oxidizer_tank.propellant.density + fuel_tank.volume * fuel_tank.propellant.density;
    
    specific_impulse = impulse / (total_propellant_mass * gravity);
    printf("Impulse: %0.2lf Ns\n", impulse);
    printf("Peak pressure: %0.2lf MPa \n", max_pressure/1000000.0);
    double mx=0.0;
    printf("Peak mass flux: %0.2lf kg/m^s-s\n", mx);
    printf("Propellant mass: %lf\n", total_propellant_mass);
    printf("Isp: %0.1lf s \n", specific_impulse);

}