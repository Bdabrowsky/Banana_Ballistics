#include "temperature.h"

#include <math.h>

void heat_transfer::calculate_hx(double throat_diameter, double mass_flow, double curvature_radius, double local_area, double){

    hx = 0.026 * pow(throat_diameter, -0.2) * pow(gas_viscosity, 0.2);
}
