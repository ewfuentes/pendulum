#include <math.h>

#include <functional>
#include <iostream>

#include "pendulum.hh"

Pendulum::Pendulum() : dt_(.05),
                   mass_(1), length_(1), gravity_(9.81) {
    Pendulum::render_init();                   
};
Pendulum::Pendulum(const double dt,
                   const double mass,
                   const double length,
                   const double g) :
                   dt_(dt), mass_(mass), length_(length), gravity_(g) {
    Pendulum::render_init();                   
};

State Pendulum::simulate(const State x_0, const double u) {
    State ret;
    double inertia = mass_ / (length_ * length_);
    // Symplectic Euler Integration
    ret[1] = x_0[1] + dt_ * gravity_ / length_ * sin(x_0[0]) + u / inertia * dt_;
    ret[0] = x_0[0] + dt_ * ret[1];
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
