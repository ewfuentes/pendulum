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

Eigen::Matrix2d Pendulum::compute_state_jacobian(const State &x) {
    Eigen::Matrix2d J = Eigen::Matrix2d::Identity();
    J << 1, dt_, dt_ * gravity_ / length_ * std::cos(x(0)), 1;
    return J;
}

std::vector<Eigen::Matrix2d> Pendulum::compute_state_jacobian(const std::vector<State> &x) {
    std::vector<State> A;
    for (const auto &x_t : x) {
        A.push_back(compute_state_jacobian(x_t));
    }
    return A;
}

Eigen::Vector2d Pendulum::compute_control_jacobian(const double u) {
    Eigen::Vector2d B = Eigen::Vector2d::Zero(); 
    B << 0, dt_ / (mass_ * length_ * length_);
    return B;
}

std::vector<Eigen::Vector2d> Pendulum::compute_control_jacobian(const std::vector<double> &u) {
    std::vector<Eigen::Vector2d> B;
    for (const auto &u_t : u) {
       B.push_back(compute_contol_jacobian(u_t); 
    }
    return B;
}

State Pendulum::simulate(const State &x_0, const double u) {
    State ret;
    double inertia = mass_ / (length_ * length_);
    // Symplectic Euler Integration
    ret[1] = x_0[1] + dt_ * gravity_ / length_ * sin(x_0[0]) + u / inertia * dt_;
    ret[0] = x_0[0] + dt_ * ret[1];
    return ret;
}

std::vector<State> Pendulum::simulate(const State &x_0,
                                      const std::vector<double> &u) {
    std::vector<State> states{x_0};
    for (auto &u_i : u) {
        states.push_back(simulate(states.back(), u_i));
    }
    return states;
}
