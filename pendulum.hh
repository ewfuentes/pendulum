#pragma once
#include <vector>
#include <array>

using State = std::array<double,2>;

class Pendulum {
 public:
    Pendulum(double dt, double mass, double length, double g);     
    State simulate(State x_0, double u);
    std::vector<State> simulate(State x_0, std::vector<double> &u); 
 private:
    const double dt;
    const double mass;
    const double length;
    const double gravity;
};
