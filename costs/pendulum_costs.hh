#pragma once

#include <cmath>

#include "dynamics/pendulum.hh"
#include "costs/cost_function.hh"
#include "util/angle_diff.hh"
#include "util/huber.hh"

namespace costs {
namespace pendulum {
using dynamics::pendulum::Pendulum;
class EnergyCost : public CostFunction<Pendulum> {
 public:
    EnergyCost(const Pendulum::Config &config) : p_config_{config} {};

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
        return util::huber(energy_delta);
    }
 private:
    const Pendulum::Config p_config_;
};

class PositionCost : public CostFunction<Pendulum> {
 public:
    PositionCost() {};

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        const double angle_diff = util::angle_diff(x[Pendulum::States::POSITION], M_PI);
        return util::huber(angle_diff);
    }
};

class ControlCost : public CostFunction<Pendulum> {
 public:
    ControlCost() {};

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        return u * u;
    }
};

class PendulumCost : public CostFunction<Pendulum> {
 public:
    struct Config {
        double energy_weight;
        double position_weight;
        double control_weight;
    };

    PendulumCost(const Pendulum::Config &p_config, const Config &config) :
        energy_cost_(EnergyCost(p_config)), config_(config) {};

    double operator()(const Pendulum::State &x,
                      const Pendulum::Control &u,
                      const bool is_terminal) const {
        return config_.energy_weight * energy_cost_(x, u, is_terminal) + 
            config_.position_weight * position_cost_(x, u, is_terminal) +
            config_.control_weight * control_cost_(x, u, is_terminal);
    }

 private:
    const EnergyCost energy_cost_;
    const PositionCost position_cost_;
    const ControlCost control_cost_;
    const Config config_;
};
}  // namespace pendulum
}  // namespace costs
