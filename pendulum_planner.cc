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


Eigen::Matrix2d compute_state_cost_jacobian(const State &s) {
    const double g = pendulum_.gravity_;
    const double l = pendulum_.length_;
    constexpr double Q_E = .5;  
    const double E_D = g * l;
    const double dEnergy_dTheta = -2.0 * g * l * Q_E * (.5 * l * l * s(1)*s(1)
                                                        + g * l *std::cos(s(0))
                                                        - E_D)
                                  * std::sin(s(0));
    const double dEnergy_dThetaDot = Q_E * l * l * s(1) * (l * l * s(1) * s(1)
                                                           + g * l * std::cos(s(0))
                                                           - E_D);

    const double dQpos_dTheta = 1 / (2 * M_PI * M_PI) * s(0);
    const double dQSpeed_dThetaDot = l / (8 * g);

    
    Eigen::RowVector2d J;
    J << dEnergy_dTheta + dQpos_dTheta, dEnergy_dThetaDot + dQSpeed_dThetaDot;
    return J;

}

Eigen::Matrix2d compute_state_cost_hessian(const State &s) {
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

    const double d2Energy_dTheta_dThetaDot = -2 * g * l * l * l * Q_E * std::sin(s(0)) * s(1);

    const double d2Qpos_dTheta2 = 1 / (2.0 * M_PI * M_PI);
    const double d2Qspeed_dThetaDot2 = l / ( 8 * g);
    

    Eigen::Matrix2d H;
    H << d2Energy_dTheta2 + d2Energy_dTheta2, d2Energy_dTheta_dThetaDot,
         d2Energy_dTheta_dThetaDot, d2Energy_dThetaDot2 + d2Qspeed_dThetaDot2;
    return H;
}

double control_cost(const double u) {
    const double max_control = .5;
    return u * u / (max_control * max_control);
}

double compute_control_cost_jacobian(const double &u) {
    return .5 * u;    
}

double compute_control_cost_hessian(const double &u) {
    return .5;
}

struct TaylorExpansion {
    double control_linearization_point;
    double control_constant_term;
    double control_linear_term;
    double control_quadratic_term;

    Eigen::Vector2d state_linearization_point;
    double state_constant_term;
    Eigen::Vector2d state_linear_term;
    Eigen::Matrix2d state_quadratic_term;

    double evaulate_control(const double u) {
        double u_prime = u - control_linearization_point;
        return control_constant_term + control_linear_term * u_prime
               + .5 * control_quadratic_term * u_prime * u_prime;
    }

    double evaluate_state(const State &s) {
        State s_prime = s - state_linearization_point;
        return state_constant_term + state_linear_term.transpose() * s_prime
            + s_prime.transpose() * state_quadratic_term * s_prime;
    }
}

TaylorExpansion quadraticize_cost(const State &s, const double u) {
    TaylorExpansion t = {
        .control_linearization_point = u;
        .control_constant_term = control_cost(u);
        .control_linear_term = compute_control_jacobian(u);
        .control_quadratic_term = compute_control_cost_hessian(u);

        .state_linearization_point = s,
        .state_constant_term = state_cost(s);
        .state_linear_term = compute_state_jacobian(s);
        .state_quadratic_term = compute_state_cost_hessian(s);
    };
    return t;
}

std::vector<TaylorExpansion> quadraticize_cost(const std::vector<State> &s,
                                               const std::vector<double> &u) {
    std::vector<TaylorExpansion> exp;
    for (int i = 0; i < s.size(); i++) {
        if (i == s.size() - 1) {
            exp.push_back(quadraticize_cost(s[i], 0));
        } else {
            exp.push_back(quadraticize_cost(s[i], u[i])); 
        }
    }
    return exp;
}

}  // namespace

PendulumPlanner::PendulumPlanner(Pendulum pen) pendulum_(pen) {}

std::vector<double> plan(const State &s, const int num_steps) {
    // Proposed Trajectory 
    std::vector<double> u(num_steps, .00);
    std::vector<State> x = pendulum_.simulate(s, u);
    
    // Linearize dynamics about x, u
    std::vector<State> A = pendulum_.compute_state_jacobian(x);
    std::vector<Eigen::Vector2d> B = pendulum_.compute_contol_jacobian(u);    
    
    // Compute 2nd order taylor expansion of cost
    std::vector<TaylorExpansion> cost = quadraticize_cost(x, u);
    
    // Solve Affine LQR for K, u
    // Forward simulate to get new X_t
    //

}
