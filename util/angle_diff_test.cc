#include "gtest/gtest.h"

#include "util/angle_diff.hh"
namespace util {
TEST(AngleDiffTest, happy_case) {
    constexpr double a = 0;
    constexpr double b = M_PI / 2;    
    
    EXPECT_NEAR(angle_diff(a, b), M_PI / 2, 1e-6);
    EXPECT_NEAR(angle_diff(b, a), -M_PI / 2, 1e-6);
}

TEST(AngleDiffTest, wrapped_around) {
    constexpr double a = M_PI;
    constexpr double b = 3 * M_PI;
    EXPECT_NEAR(angle_diff(a, b), 0.0, 1e-6);
    EXPECT_NEAR(angle_diff(b, a), 0.0, 1e-6);
    EXPECT_NEAR(angle_diff(a, -b), 0.0, 1e-6);
    EXPECT_NEAR(angle_diff(-b, a), 0.0, 1e-6);
}

}
