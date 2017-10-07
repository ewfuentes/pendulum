#pragma once
#include "pendulum.hh"

class PendulumPlanner {
 public:
    PendulumPlanner(Pendulum pen); 
    plan(const State &s, const int num_steps);
 private:

    Pendulum pendulum_;
}
