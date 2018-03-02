#include "ilqr/naive_ilqr.hh"

#include "gtest/gtest.h"

#include "costs/pendulum_costs.hh"
#include "dynamics/pendulum.hh"
#include "util/numerical_diff.hh"


namespace ilqr {

template<class T>
double total_cost(const costs::CostFunction<T> &cost_func, const std::vector<typename T::State> &x, const std::vector<typename T::Control> &u) {
    double cost = 0;
    for (int i = 0; i < (int)x.size(); i++) {
        if (i < (int)u.size() - 1) {
            cost += cost_func(x[i], u[i], false);
        } else {
            const typename T::Control zero_control = T::Control::Zero();
            cost += cost_func(x[i], zero_control, true);
        }
    }
    return cost;
}

using dynamics::pendulum::Pendulum;
using costs::pendulum::PendulumCost;
TEST(NaiveIlqrTest, pendulum_test) {
    // Create dynamics
    const Pendulum::Config dyn_config = {
        .mass = 1.0,
        .length = 1.0,
        .gravity = 9.8,
        .dt = 0.1
    };  
    Pendulum pendulum(dyn_config);

    // create cost function
    const costs::pendulum::PendulumCost::Config cost_config {
        .energy_weight = 1.0,
        .position_weight = 1.0,
        .control_weight = 1.0,
    };

    PendulumCost pendulum_cost(dyn_config, cost_config);

    NaiveIlqr<Pendulum> ilqr(pendulum, &pendulum_cost);
    
    Pendulum::State x0 = Pendulum::State::Zero();
    const Pendulum::Control init{0.1};
    const std::vector<Pendulum::Control> init_u(50, init);
    for (int i = 0; i < 100; i++) {
        const NaiveIlqr<Pendulum>::Trajectory result = ilqr.solve(x0, init_u);
        const Pendulum::Control applied_control = result.u.front();
        std::cout << "Iter " << i << ": " << x0.transpose() << " " << applied_control << std::endl;    
        x0 = pendulum.simulate(x0, applied_control);
    }
}
}  // namespace ilqr
