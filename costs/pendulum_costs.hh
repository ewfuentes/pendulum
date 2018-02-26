#pragma once

#include <cmath>

#include "dynamics/pendulum.hh"
#include "costs/cost_function.hh"

namespace costs {
namespace pendulum {
using dynamics::Pendulum;
class EnergyCost : CostFunction<Pendulum> {
 public:
    EnergyCost(const Pendulum::Config &config) p_config_{config} {};

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        const double desired_energy = 2 * p_config_.length * p_config_.gravity * p_config_.mass;
        const double inertia = p_config_.mass * p_config_.length * p_config_.length;
        const double kinetic_energy =
            .5 * inertia * x[Pendulum::States::VELOCITY] * x[Pendulum::States::VELOCITY];
        const double height = p_config_.length * (1 - std::cos(x[Pendulum::States::POSITION]));
        const double potential_energy = p_config_.mass * p_config_.gravity * height;
        const double energy_delta = desired_energy - kinetic_energy - potential_energy;
        return energy_delta * energy_delta;
    }
 private:
    const Pendulum::Config p_config_;
};

class PositionCost : CostFunction<Pendulum> {
 public:
    PositionCost() {};

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        const double angle_diff = util::angle_diff(x[Pendulum::States::POSITION], M_PI);
        return angle_diff * angle_diff;
    }
};

class PendulumCost : CostFunction<Pendulum> {
 public:
    struct Config {
        double energy_weight;
        double position_weight;
    }

    PendulumCost(const Pendulum::Config &p_config, const Config &config) :
        energy_cost_(EnergyCost(p_config)), config_(config) {}

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        return config_.energy_weight * energy_cost(x, u, is_terminal) + 
            config_.position_weight * position_cost(x, u, is_terminal);
    }

 private:
    const EnergyCost energy_cost_;
    const PositionCost position_cost_;
    const Config config_;
};
}  // namespace pendulum
}  // namespace costs