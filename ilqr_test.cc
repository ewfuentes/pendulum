#include "ilqr.hh"

#include <cmath>

#include "gtest/gtest.h"

namespace ilqr {
namespace {
using block = ILQR<2,1>;
block::State sliding_block(const block::State &x, const block::Control &u) {
    constexpr double DT = .01;
    block::State out = block::State::Zero();
    out(0) = x(1) * DT;
    out(1) = u(0) * DT;
    return out; 
}

double sliding_block_cost(const block::State &x, const block::Control &u, const bool is_terminal) {
    (void) is_terminal;
    const double desired_position = 10;
    const double position_error = x(0) - desired_position;
    const double position_cost = position_error * 0.5 * position_error;
    const double velocity_cost = x(1) * 0.5 * x(1);
    const double control_cost = u(0) * 0.5 * u(0);
    return position_cost + velocity_cost + control_cost;
}

}  // namespace

TEST(IlqrSetup, TestDynamicsExpansion) {
    block::DynamicsExpansion ex =
        block::DynamicsExpansion(sliding_block, block::State{0.0, 0.0}, block::Control{0.0});
    
    // Dynamics are completely linear in the state, so we should expect them to be equal
    const block::State ana = sliding_block(block::State{1.0, 1.0}, block::Control{1.0});
    const block::State num = ex.evaluate(block::State{1.0, 1.0}, block::Control{1.0});
    constexpr double EPS = 1e-8;
    for (int i = 0; i < 2; i++) {
        EXPECT_LT(std::abs(num(i) - ana(i)) / (ana(i) + EPS), 0.01);
    }
}

TEST(IlqrSetup, TestCostExpansion) {
    block::CostExpansion ex =
        block::CostExpansion(sliding_block_cost, block::State{0.0, 0.0}, block::Control{0.0}, false);
    constexpr double EPS = 1e-8; 

    // Cost is quadratic, so this approximation should be valid everywhere
    {
        const double ana = sliding_block_cost(block::State{1.0, 1.0}, block::Control{1.0}, false);
        const double num = ex.evaluate(block::State{1.0, 1.0}, block::Control{1.0});
        EXPECT_LT(std::abs(num- ana) / (ana + EPS), 0.01);
    }

    {
        const double ana = sliding_block_cost(block::State{10.0, 1.0}, block::Control{-1.0}, false);
        const double num = ex.evaluate(block::State{10.0, 1.0}, block::Control{-1.0});
        EXPECT_LT(std::abs(num- ana) / (ana + EPS), 0.01);
    }
}

}  // namespace ilqr

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
