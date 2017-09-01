#include <math.h>

#include <functional>
#include <iostream>

#include "pendulum.hh"

Pendulum::Pendulum(double dt, double mass, double length, double g) : dt(dt),
                   mass(mass), length(length), gravity(g) {}

State Pendulum::simulate(State x_0, double u) {
    State ret;
    // Symplectic Euler Integration
    ret[1] = x_0[1] + dt * gravity / length * sin(x_0[0]) + u / mass / length / length * dt;
    ret[0] = x_0[0] + dt * ret[1];
    return ret;
}

std::vector<State> Pendulum::simulate(State x_0, std::vector<double> &u) {
    std::vector<State> states{x_0};
    for (auto &u_i : u) {
        states.push_back(simulate(states.back(), u_i));
    }
    return states;
}
