#include "ilqr/naive_ilqr.hh"

#include "gtest/gtest.h"

#include "costs/pendulum_costs.hh"
#include "dynamics/pendulum.hh"


namespace ilqr {
using dynamics::pendulum::Pendulum;
using costs::pendulum::PendulumCost;
TEST(NaiveIlqrTest, pendulum_test) {
    // Create dynamics
    const Pendulum::Config dyn_config = {
        .mass = 1.0,
        .length = 1.0,
        .gravity = 9.81,
        .dt = 0.1
    };  
    std::unique_ptr<Pendulum> pendulum = std::make_unique<Pendulum>(dyn_config);

    // create cost function
    const costs::pendulum::PendulumCost::Config cost_config {
        .energy_weight = 1.0,
        .position_weight = 1.0,
    };
    std::unique_ptr<costs::CostFunction<Pendulum>> pendulum_cost =
        std::make_unique<PendulumCost>(dyn_config, cost_config);

    NaiveIlqr<Pendulum> ilqr(std::move(pendulum), std::move(pendulum_cost));
    
    Pendulum::State x0 = {0,0};
    std::vector<Pendulum::Control> u(20, Pendulum::Control());
    ilqr.solve(x0, u);
}
}  // namespace ilqr
