#include "pendulum_planner.hh"

namespace {
double state_cost(const State &s) {
    const double desired_energy = pendulum_.gravity_ * pendulum_.length_;

    const double max_energy = desired_energy;
    const double max_position = M_PI;
    const double max_velocity = 2 * std::sqrt(pendulum_.gravity_ / pendulum_.length_);

    const double kinetic_energy =
        .5 * pendulum_.length_ * pendulum_.length_ *s[StateId::VELOCITY];
    const double potential_energy =
        pendulum_.gravity_ * pendulum_.length_ * std::cos(s[StateId::POSITION]);
    const double energy = kinetic_energy + potential_energy;

    const double energy_diff = energy - desired_energy;
    const double energy_cost =
        .5 / (max_energy * max_energy) * energy_diff * energy_diff;
    
    const double position_cost =
        .25 / (M_PI * M_PI) * s[StateId::POSITION] * s[StateId::POSITION];

    const double velocity_cost = .25 / (max_velocity * max_velocity)
                                * s[StateId::VELOCITY] * s[StateId::VELOCITY];

    return energy_cost + position_cost + velocity_cost;
}

double control_cost(const double u) {
    const double max_control = .5;
    return u * u / (max_control * max_control);
}

Eigen::RowVector2d compute_state_cost_hessian(const State &s) {
    const double g = pendulum_.gravity_;
    const double l = pendulum_.length_;
    constexpr double Q_E = .5;
    const double E_D = pendulum_.gravity_ * pendulum_.length_;
    const double d2Energy_dTheta2 = -2.0 * g * l * Q_E
                            * (-g*l*std::sin(s(0)) * std::sin(s(0))
                               + (g * l * std::cos(s(0))
                                  + .5 * l * l * s(1) * s(1) - E_D)
                               * std::cos(s(0)));
    const double d2Energy_dThetaDot2 = Q_E * l * l
                                * (3 * l * l * s(1) * s(1)
                                   + g * l * std::cos(s(0))
                                   - E_D);

    const double d2Qpos_dTheta2 = 1 / (2.0 * M_PI * M_PI);
    const double d2Qspeed_dThetaDot2 = l / ( 8 * g);

    Eigen::RowVector2d H;
    H << d2Energy_dTheta2 + d2Energy_dTheta2, d2Energy_dThetaDot2 + d2Qspeed_dThetaDot2;
    return H;
}

double compute_control_cost_hessian(const double &u) {
    return .5;
}

}  // namespace

PendulumPlanner::PendulumPlanner(Pendulum pen) pendulum_(pen) {}

std::vector<double> plan(const State &s, const int num_steps) {
    // Proposed Trajectory 
    std::vector<double> u(num_steps, .00);
    std::vector<State> x = pendulum_.simulate(s, u);
    
    // Linearize dynamics about x, u
    
    
    // Compute 2nd order taylor expansion of cost
    // Solve Affine LQR for K, u
    // Forward simulate to get new X_t
    //

}
