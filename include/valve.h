#pragma once

class valve{
    public:
        double actutation_time;
        double Cd;
        double area;
        double setpoint;
        double position;

        double update(double mass_flow);
        void open(double time);
        void close(double time);
        void actuate(double i_setpoint);

    private:
        double ticks;
        bool flag_direction;
        bool flag_open;
};