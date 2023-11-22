#include "propellant.h"

#include<math.h>

void propellant::init(){

    tau = sqrt(specific_heat_ratio) * pow( (2.0 / (specific_heat_ratio + 1.0)), (specific_heat_ratio + 1.0) / (2.0 * (specific_heat_ratio - 1.0)));
    
}