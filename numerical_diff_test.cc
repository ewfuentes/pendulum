#include "numerical_diff.hh"
#include "gtest/gtest.h"

#include <iostream>

#include <cmath>

namespace numerics {
namespace {
Vec<1> scalar_scalar_func(const Vec<1> &x) {
    return .1 * x * x * x + 3 * x * x + 4 * x + Vec<1>{5.0};
}

Vec<1> scalar_scalar_analytical_jacobian(const Vec<1> &x) {
    return .3 * x * x + 6 * x + Vec<1>{4.0};
}

Vec<1> scalar_scalar_analytical_hessian(const Vec<1> &x) {
    return .6 * x + Vec<1>{6};
}

Vec<3> three_by_two_func(const Vec<2> &x) {
    return {x(0) * x(0),
            x(0) * x(1),
            x(1) * x(1) * x(1)};
}

Mat<3,2> three_by_two_jac(const Vec<2> &x) {
    Mat<3,2> out;
    out.row(0) << 2 * x(0), 0.0;
    out.row(1) << x(1), x(0);
    out.row(2) << 0, 3 * x(1) * x(1);
    return out;    
}

Vec<1> one_by_two_func(const Vec<2> &x) {
    return Vec<1>{4 * x(0) * x(0)
            + 2 * x(0) * x(1)
            + 5 * x(1) * x(1)
            + 3 * x(0)
            + 4 * x(1)
            + 5};
}

Mat<1, 2> one_by_two_jac(const Vec<2> &x) {
    return {8 * x(0) + 2 * x(1) + 3, 2 * x(0) + 10 * x(1) + 4};
}

Mat<2, 2> one_by_two_hess(const Vec<2> &x) {
    (void)x;
    Mat<2, 2> out;
    out.row(0) << 8, 2;
    out.row(1) << 2, 10;
    return out;
}
}  // namespace

TEST(JacobianTest, scalar_scalar_test) {
    const Vec<1> EVAL_POINT{20};
    const Callable<1,1> f = scalar_scalar_func;
    const auto num_jac = compute_jacobian<1,1>(f, EVAL_POINT);
    const Vec<1> ana_jac = scalar_scalar_analytical_jacobian(EVAL_POINT);

    EXPECT_LT(std::abs(num_jac(0) - ana_jac(0)) / ana_jac(0), .01);
}

TEST(JacobianTest, three_by_two_test) {
    constexpr double EPS = 1e-8;
    const Vec<2> eval_point{3, 4};
    const Callable<3, 2> f = three_by_two_func;
    const Mat<3,2> num_jac = compute_jacobian<3, 2>(f, eval_point);
    const Mat<3,2> ana_jac = three_by_two_jac(eval_point);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_LT(std::abs(num_jac(i, j) - ana_jac(i, j)) / (ana_jac(i,j) + EPS), .01);
        }
    }
}

TEST(JacobianTest, one_by_two_test) {
    constexpr double EPS = 1e-8;
    const Vec<2> eval_point{3, 4};
    const Callable<1, 2> f = one_by_two_func;
    const Mat<1,2> num_jac = compute_jacobian<1, 2>(f, eval_point);
    const Mat<1,2> ana_jac = one_by_two_jac(eval_point);

    for (int i = 0; i < 2; i++) {
            EXPECT_LT(std::abs(num_jac(i) - ana_jac(i)) / (ana_jac(i) + EPS), .01);
    }
}

TEST(HessianTest, scalar_scalar_test) {
    const Vec<1> eval_point{20};
    const Callable<1,1> f = scalar_scalar_func;
    const Mat<1,1> num_hess = compute_hessian<1>(f, eval_point);
    const Vec<1> ana_hess = scalar_scalar_analytical_hessian(eval_point);
    EXPECT_LT(std::abs(num_hess(0) - ana_hess(0)) / ana_hess(0), .01);
}

TEST(HessianTest, one_by_two_test) {
    const Vec<2> eval_point{3, 4};
    const Callable<1,2> f = one_by_two_func;
    const auto num_hess = compute_hessian<2>(f, eval_point);
    const Mat<2,2> ana_hess = one_by_two_hess(eval_point);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            EXPECT_LT(std::abs(num_hess(i, j) - ana_hess(i, j)) / ana_hess(i, j), .01);
        }
    }
}
}  // namespace numerics

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
