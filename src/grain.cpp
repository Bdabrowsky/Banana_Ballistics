#include "grain.h"
#include "config.h"

#include<math.h>
#include<stdio.h>
#include<stdio.h>



void grain::init(double len, double dia, double port_dia){
    length_initial = len;
    length = len;

    diameter = dia;
    port_diameter = port_dia;

    volume = PI * (diameter * diameter - port_diameter * port_diameter) * length / 4.0;
    volume_initial = volume;

    area = port_diameter * PI * length + 2.0 * PI * (diameter * diameter - port_diameter * port_diameter) / 4.0;

    web = (diameter - port_diameter) / 2.0;

}



void grain::update(double pressure){

    prop.change_coefficients(pressure);
    
    burn_rate = prop.a * pow(pressure, prop.n); //+ prop.alpha * pow(mass_flux, 0.8) * pow(length_initial, -0.2) * pow(e, ((-1.0) * prop.beta * prop.density * pressure) / mass_flux);

    port_diameter += 2.0 * burn_rate * dT;

    length -= 2.0 * burn_rate * dT;

    area = port_diameter * PI * length + 2.0 * PI * (diameter * diameter - port_diameter * port_diameter) / 4.0;

    volume = PI * (diameter * diameter - port_diameter * port_diameter) * length / 4.0;
    //printf("%lf", (port_diameter));
    web = (diameter - port_diameter) / 2.0;

    mass_flow += area * burn_rate * prop.density;
    mass_flux = mass_flow / (port_diameter * port_diameter * PI / 4.0);

    if(mass_flux >= max_mass_flux){
        max_mass_flux = mass_flux;
    }

  
}