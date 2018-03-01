#pragma once

#include <memory>
#include <vector>

#include "dynamics/dynamical_system.hh"
#include "costs/cost_function.hh"

namespace ilqr {
template<class T>
class NaiveIlqr {
 public:
    // Make sure we have a model that derives from a dynamics::DynamicalSystem
    static_assert(std::is_base_of<
            dynamics::DynamicalSystem<T::STATE_DIM, T::CONTROL_DIM>, T>::value,
        "T is not derived from DynamicalSystem");

    struct Trajectory {
        std::vector<typename T::Control> u;
        std::vector<typename T::State> x;
        double cost;
    };

     NaiveIlqr(std::unique_ptr<T> dyn,
             std::unique_ptr<costs::CostFunction<T>> cost) :
        dynamics_(std::move(dyn)), cost_function_(std::move(cost)) {};

     // Given an initial set of controls, find the control sequence which
     // minimizes the cost function
     Trajectory solve(const typename T::State &x0,
             const std::vector<typename T::Control> &u) const;

 private:
    const std::unique_ptr<T> dynamics_;
    const std::unique_ptr<costs::CostFunction<T>> cost_function_; 
};
}  // namespace ilqr

#include "ilqr/naive_ilqr_impl.hh"
