#pragma once

class heat_transfer{
    public:
        double gas_viscosity;
        double gas_specific_heat;
        double local_pradtl;
        double sigma;

        double hx;
    
        void calculate_hx(double throat_diameter, double mass_flow, double curvature_radius, double local_area, double);

};