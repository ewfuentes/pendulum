#include <math.h>

#include <functional>
#include <iostream>

#include "pendulum.hh"

Pendulum::Pendulum() : dt(.05),
                   mass(1), length(1), gravity(9.81) {
    Pendulum::render_init();                   
};
Pendulum::Pendulum(const double dt,
                   const double mass,
                   const double length,
                   const double g) :
                   dt(dt), mass(mass), length(length), gravity(g) {
    Pendulum::render_init();                   
};

State Pendulum::simulate(const State x_0, const double u) {
    State ret;
    double inertia = mass / (length * length);
    // Symplectic Euler Integration
    ret[1] = x_0[1] + dt * gravity / length * sin(x_0[0]) + u / inertia * dt;
    ret[0] = x_0[0] + dt * ret[1];
    return ret;
}

std::vector<State> Pendulum::simulate(const State x_0,
                                      const std::vector<double> &u) {
    std::vector<State> states{x_0};
    for (auto &u_i : u) {
        states.push_back(simulate(states.back(), u_i));
    }
    return states;
}
