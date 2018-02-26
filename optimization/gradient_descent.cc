#include "optimization/gradient_descent.hh"

#include <iostream>

#include "util/numerical_diff.hh"

namespace optimization {
OptimizationResult GradientDescent::optimize(const OptimFunc &func, const Eigen::VectorXd &x0) {
    OptimizationResult result;
    result.result = x0;

    std::function<Eigen::VectorXd(Eigen::VectorXd)> f = [func](const Eigen::VectorXd &x) ->
        Eigen::VectorXd { return Eigen::Matrix<double, 1, 1>{func(x)}; };

    for (int i = 0; i < config_.max_iters; i++) {
        const double old_value = result.function_value;
        result.function_value = func(result.result);
        const double small_function_change = std::abs(old_value - result.function_value) < config_.f_tol;
        if (small_function_change) {
            // we've converged
            // Let's blow this popsicle stand
            result.num_iteration = i;
            result.converged = true;
            break;
        }

        // Compute gradient
        const Eigen::MatrixXd gradient = util::compute_jacobian(f, result.result).transpose();

        // Perform line search
        if (config_.line_search_enabled) {

        }
//        std::cout
//            << "Iter: " << i
//            << " Value: " << result.function_value
//            << " params: " << result.result.transpose()
//            << " gradient: " << gradient.transpose()
//            << std::endl;
        // update parameters
        result.result -= config_.alpha * gradient; 
    }
    return result;
};

}  // namespace optimization
