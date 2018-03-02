#include <math.h>

#include "gtest/gtest.h"

#include "dynamics/pendulum.hh"

namespace dynamics::pendulum {
namespace {
TEST(PendulumTest, zero_position_zero_vel_tests) {
    // SETUP
    const Pendulum::Config config = {10.0, 2.0, 20.0, .1};
    Pendulum p{config};
    const Pendulum::State state = Pendulum::State::Zero();
    
    {
        // No Action
        // ACTION
        Pendulum::Control control = Pendulum::Control::Zero();
        Pendulum::State new_state = p.simulate(state, control);

        // VERIFICATION
        EXPECT_EQ(new_state[0], 0.0);
        EXPECT_EQ(new_state[1], 0.0);
    }

    {
        // Push Right
        // ACTION
        const Pendulum::Control control{1.0};
        Pendulum::State new_state_1 = p.simulate(state, control);
        Pendulum::State new_state_2 = p.simulate(new_state_1, Pendulum::Control::Zero());
        
        // VERIFICATION
        EXPECT_GE(new_state_1[Pendulum::States::POSITION], state[Pendulum::States::POSITION]);
        EXPECT_GT(new_state_1[Pendulum::States::VELOCITY], state[Pendulum::States::VELOCITY]);
        EXPECT_GT(new_state_2[Pendulum::States::POSITION], state[Pendulum::States::POSITION]);
    }

    {
        // Push Left
        // ACTION
        const Pendulum::Control control{-1.0};
        Pendulum::State new_state_1 = p.simulate(state, control);
        Pendulum::State new_state_2 = p.simulate(new_state_1, Pendulum::Control::Zero());
        
        // VERIFICATION
        EXPECT_LE(new_state_1[Pendulum::States::POSITION], state[Pendulum::States::POSITION]);
        EXPECT_LT(new_state_1[Pendulum::States::VELOCITY], state[Pendulum::States::VELOCITY]);
        EXPECT_LT(new_state_2[Pendulum::States::POSITION], state[Pendulum::States::POSITION]);
    }
}

TEST(PendulumTest, side_position_tests) {
    // SETUP
    const Pendulum::Config config = {10.0, 2.0, 20.0, .1};
    Pendulum p{config};
    const Pendulum::State state{M_PI / 2.0, 0};
    
    {
        // No Action
        // ACTION
        const Pendulum::Control control{0};
        const Pendulum::State new_state_1 = p.simulate(state, control);
        const Pendulum::State new_state_2 = p.simulate(new_state_1, control);

        // VERIFICATION
        EXPECT_LT(new_state_2[Pendulum::States::POSITION], state[Pendulum::States::POSITION]);
        EXPECT_LT(new_state_2[Pendulum::States::VELOCITY], state[Pendulum::States::VELOCITY]);
    }

}
}
}  // namespace dynamics::pendulum
