#pragma once

namespace util {
double huber(const double u) {
    return u < 1 ? u * u : u;
}
}
