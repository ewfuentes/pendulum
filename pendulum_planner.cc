#include "pendulum_planner.hh"

namespace {
double state_cost(const State &s) {

    return 0;
}

double control_cost(const double u) {
    return 0;
}
}

PendulumPlanner::PendulumPlanner(Pendulum pen) pendulum_(pen) {}

std::vector<double> plan(const State &s, const int num_steps) {
    // Proposed Trajectory 
    std::vector<double> u(num_steps, .00);
}
