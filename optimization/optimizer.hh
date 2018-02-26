#pragma once

#include <functional>
#include <limits>

#include "Eigen/Core"

namespace optimization {
using OptimFunc = std::function<double(const Eigen::VectorXd &)>;

struct OptimizationResult {
    bool converged = false;
    Eigen::VectorXd result;
    double function_value = std::numeric_limits<double>::max();
    int num_iteration = -1;
};

class Optimizer {
    virtual OptimizationResult optimize(
            const OptimFunc &func, const Eigen::VectorXd &x0) = 0;
};
}
