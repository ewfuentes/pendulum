#include "gtest/gtest.h"

#include "dynamics/pendulum.hh"

namespace dynamics::pendulum {
namespace {
TEST(PendulumTest, test_one) {
    Pendulum::Config config = {10.0, 2.0, 20.0, .1};
    
    Pendulum p{config};
    EXPECT_TRUE(true);
}
}
}  // namespace dynamics::pendulum
