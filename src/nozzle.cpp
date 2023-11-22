#include "nozzle.h"
#include "config.h"

void nozzle::update(){

    throat_diameter += buildup_coef * dT - erosion_coef * dT;
}