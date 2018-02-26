#pragma once
#include <cmath>
#include <iostream>

namespace util {
// Returns the difference between two angles in the range of [-pi, pi]
double angle_diff(const double a, const double b) {
    const double raw_diff = b - a;
    const double wrapped = std::fmod(raw_diff, 2 * M_PI);
    return wrapped > M_PI ? wrapped - 2 * M_PI : wrapped;
}
}
