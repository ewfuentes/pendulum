#pragma once

#include "optimization/optimizer.hh"

namespace optimization {
class GradientDescent : public Optimizer {
 public:
    struct Config {
        // The step size that we take
        double alpha = .001; 
        // Use line search to adjust step size
        bool line_search_enabled = false;
        // tolerance in function value
        double f_tol = 1e-9;
        // max number of iterations
        int max_iters = 100;
    };

    GradientDescent(const Config &config) : config_(config) {};

    OptimizationResult optimize(const OptimFunc &func, const Eigen::VectorXd &x0);

 private:
     const Config config_;

};
}  // namespace optimization
