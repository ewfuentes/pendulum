
#include <cmath>

#include "Eigen/Core"
#include "gtest/gtest.h"

#include "numerical_diff.hh"

namespace Eigen {
namespace {
using Vector1d = Matrix<double, 1, 1>;
}
}

namespace util {
namespace {
Eigen::Vector1d scalar_scalar_func(const Eigen::Vector1d &x) {
    return .1 * x * x * x + 3 * x * x + 4 * x + Eigen::Vector1d{5.0};
}

Eigen::Vector1d scalar_scalar_analytical_jacobian(const Eigen::Vector1d &x) {
    return .3 * x * x + 6 * x + Eigen::Vector1d{4.0};
}

Eigen::Vector1d scalar_scalar_analytical_hessian(const Eigen::Vector1d &x) {
    return .6 * x + Eigen::Vector1d{6};
}

Eigen::Vector3d three_by_two_func(const Eigen::Vector2d &x) {
    return {x(0) * x(0),
            x(0) * x(1),
            x(1) * x(1) * x(1)};
}

Eigen::Matrix<double, 3,2> three_by_two_jac(const Eigen::Vector2d &x) {
    Eigen::Matrix<double, 3,2> out;
    out.row(0) << 2 * x(0), 0.0;
    out.row(1) << x(1), x(0);
    out.row(2) << 0, 3 * x(1) * x(1);
    return out;    
}

Eigen::Vector1d one_by_two_func(const Eigen::Vector2d &x) {
    return Eigen::Vector1d{4 * x(0) * x(0)
            + 2 * x(0) * x(1)
            + 5 * x(1) * x(1)
            + 3 * x(0)
            + 4 * x(1)
            + 5};
}

Eigen::Matrix<double, 1, 2> one_by_two_jac(const Eigen::Vector2d &x) {
    return {8 * x(0) + 2 * x(1) + 3, 2 * x(0) + 10 * x(1) + 4};
}

Eigen::Matrix2d one_by_two_hess(const Eigen::Vector2d &x) {
    (void)x;
    Eigen::Matrix2d out;
    out.row(0) << 8, 2;
    out.row(1) << 2, 10;
    return out;
}
}  // namespace

TEST(JacobianTest, scalar_scalar_test) {
    const Eigen::Vector1d EVAL_POINT{20};
    const Callable f = scalar_scalar_func;
    const auto num_jac = compute_jacobian(f, EVAL_POINT);
    const Eigen::Vector1d ana_jac = scalar_scalar_analytical_jacobian(EVAL_POINT);

    EXPECT_LT(std::abs(num_jac(0) - ana_jac(0)) / ana_jac(0), .01);
}

TEST(JacobianTest, three_by_two_test) {
    constexpr double EPS = 1e-8;
    const Eigen::Vector2d eval_point{3, 4};
    const Callable f = three_by_two_func;
    const Eigen::MatrixXd num_jac = compute_jacobian(f, eval_point);
    const Eigen::MatrixXd ana_jac = three_by_two_jac(eval_point);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_LT(std::abs(num_jac(i, j) - ana_jac(i, j)) / (ana_jac(i,j) + EPS), .01);
        }
    }
}

TEST(JacobianTest, one_by_two_test) {
    constexpr double EPS = 1e-8;
    const Eigen::Vector2d eval_point{3, 4};
    const Callable f = one_by_two_func;
    const Eigen::MatrixXd num_jac = compute_jacobian(f, eval_point);
    const Eigen::MatrixXd ana_jac = one_by_two_jac(eval_point);

    for (int i = 0; i < 2; i++) {
            EXPECT_LT(std::abs(num_jac(i) - ana_jac(i)) / (ana_jac(i) + EPS), .01);
    }
}

TEST(HessianTest, scalar_scalar_test) {
    const Eigen::Vector1d eval_point{20};
    const Callable f = scalar_scalar_func;
    const Eigen::MatrixXd num_hess = compute_hessian(f, eval_point);
    const Eigen::MatrixXd ana_hess = scalar_scalar_analytical_hessian(eval_point);
    EXPECT_LT(std::abs(num_hess(0) - ana_hess(0)) / ana_hess(0), .01);
}

TEST(HessianTest, one_by_two_test) {
    const Eigen::Vector2d eval_point{3, 4};
    const Callable f = one_by_two_func;
    const Eigen::MatrixXd num_hess = compute_hessian(f, eval_point);
    const Eigen::MatrixXd ana_hess = one_by_two_hess(eval_point);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_LT(std::abs(num_hess(i, j) - ana_hess(i, j)) / ana_hess(i, j), .01);
        }
    }
}
}  // namespace util
