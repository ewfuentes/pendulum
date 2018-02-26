#pragma once

#include <dynamics/dynamical_system.hh>

namespace costs {
template <class T>
class CostFunction {
 public:
    static_assert(std::is_base_of<dynamics::DynamicalSystem, T>::value,
        "T is not derived from Base");
    virtual double operator()(const T::State &state,
                              const T::Control &u,
                              const bool is_terminal) const = 0;
};
}  // namespace costs
