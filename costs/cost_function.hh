#pragma once

#include "dynamics/dynamical_system.hh"

namespace costs {
template <typename T>
class CostFunction {
 public:
    static_assert(std::is_base_of<
            dynamics::DynamicalSystem<T::STATE_DIM, T::CONTROL_DIM>, T>::value,
        "T is not derived from DynamicalSystem");
    virtual double operator()(const typename T::State &state,
                              const typename T::Control &u,
                              const bool is_terminal) const = 0;
};
}  // namespace costs
