#include "gradient_descent.hh"

#include "gtest/gtest.h"

#include "optimization/test_functions.hh"

namespace optimization {

TEST(GradientDescentTest, quadratic_test) {
    // Setup
    GradientDescent::Config config = {
        .alpha = .1,    
        .line_search_enabled = false,
        .f_tol = 1e-9,
        .max_iters = 1000,
    };
    GradientDescent opt = GradientDescent(config);
    Eigen::Vector3d x0{1, 2, 3};

    // Action
    OptimizationResult result = opt.optimize(test_functions::sphere, x0);

    // Verification
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(result.result(0), 0, 1e-3);
    EXPECT_NEAR(result.result(1), 0, 1e-3);
    EXPECT_NEAR(result.result(2), 0, 1e-3);
}

TEST(GradientDescentTest, rosenbrock_test) {
    // Setup
    GradientDescent::Config config = {
        .alpha = .1,    
        .line_search_enabled = false,
        .f_tol = 1e-9,
        .max_iters = 1000,
    };
    GradientDescent opt = GradientDescent(config);
    Eigen::Vector3d x0{0, 0, 0};

    // Action
    OptimizationResult result = opt.optimize(test_functions::rosenbrock, x0);

    // Verification
    EXPECT_TRUE(result.converged);
    EXPECT_NEAR(result.result(0), 1.0, 1e-3);
    EXPECT_NEAR(result.result(1), 1.0, 1e-3);
    EXPECT_NEAR(result.result(2), 1.0, 1e-3);
}
}
