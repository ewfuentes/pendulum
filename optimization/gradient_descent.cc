#include "optimization/gradient_descent.hh"

#include <iostream>

#include "util/numerical_diff.hh"

namespace optimization {
namespace {
double perform_line_search(const OptimFunc &func, const Eigen::VectorXd &x,
        const Eigen::VectorXd gradient, const double initial_alpha, 
        const double expected_decrease_ratio) { 
    constexpr double SHRINK_FACTOR = 0.5;
    const double baseline = func(x);
    double alpha = initial_alpha * 2;
    for (int i = 0; i < 5; i++) { 
        alpha *= SHRINK_FACTOR;
        const Eigen::VectorXd test_point = x - alpha * gradient;
        const double test_val = func(test_point);
        const double expected_val = -alpha * gradient.transpose() * gradient + baseline;
        if (test_val * expected_val < expected_decrease_ratio) {
            break;
        }
    }
    return alpha;
}
}  // namespace


OptimizationResult GradientDescent::optimize(
        const OptimFunc &func, const Eigen::VectorXd &x0) {
    OptimizationResult result;
    result.result = x0;

    std::function<Eigen::VectorXd(Eigen::VectorXd)> f =
        [func](const Eigen::VectorXd &x) -> Eigen::VectorXd {
            return Eigen::Matrix<double, 1, 1>{func(x)};
    };

    for (int i = 0; i < config_.max_iters; i++) {
        const double old_value = result.function_value;
        result.function_value = func(result.result);
        const double small_function_change =
            std::abs(old_value - result.function_value) < config_.f_tol;
        if (small_function_change) {
            // we've converged
            // Let's blow this popsicle stand
            result.num_iteration = i;
            result.converged = true;
            break;
        }

        // Compute gradient
        const Eigen::MatrixXd gradient =
            util::compute_jacobian(f, result.result).transpose();

        // Perform line search
        const double alpha = config_.line_search_enabled ? 
            perform_line_search(func, result.result, gradient, config_.alpha, 0.9) : config_.alpha;

        std::cout
            << "iter: " << i
            << " f_val: " << result.function_value
            << " alpha: " << alpha
            << " x: " << result.result.transpose()
            << " grad: " << gradient.transpose()
            << std::endl;

        // update parameters
        result.result -= alpha * gradient; 
    }
    return result;
};

}  // namespace optimization
