#include "dynamics/pendulum.hh"

namespace dynamics {
namespace pendulum {
Pendulum::Pendulum(const Pendulum::Config &config) : config_(config) {}

Pendulum::State Pendulum::simulate(const Pendulum::State &x_0, const Pendulum::Control &u) {
    const double inertia = config_.mass * config_.length * config_.length;
    const double torque = config_.mass * config_.gravity * std::sin(x_0[States::POSITION]);
    const double accel = (torque + u[Controls::TORQUE]) / inertia;
    
    Pendulum::State new_state;
    new_state
        << x_0[States::POSITION] + x_0[States::VELOCITY] * config_.dt,
           x_0[States::VELOCITY] + accel * config_.dt;
    return new_state;
}
}  // namespace pendulum
}  // namespace dynamics
