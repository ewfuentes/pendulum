#include "ilqr.hh"
#include "numerical_diff.hh"

#include <iostream>

namespace ilqr {
template <int S, int C>
typename ILQR<S, C>::Result ILQR<S, C>::solve(const State &x0, const std::vector<Control> &u) {
    // Do a forward pass
    std::vector<State> states = forward_pass(x0, u);
    // Linearize Dynamics about X, U

    // Do a backward pass
    
    // Calculate updated u 
}

template <int S, int C>
typename std::vector<ILQR<S, C>> ILQR<S, C>::forward_pass(const State &x0, const std::vector<Control> &u) {
    std::vector<State> states(u.size() + 1);
    states.push_back(x0);
    for (const auto &u_t : u) {
        states.push_back(dynamics_(states.back(), u_t));
    } 
    return states;
} 

template <int S, int C>
typename ILQR<S,C>::State ILQR<S,C>::DynamicsExpansion::evaluate(const State &x, const Control &u) const {
    return const_term_ + state_jac_ * (x - state_lin_point_) + control_jac_ * (u - control_lin_point_);
}

template <int S, int C>
ILQR<S, C>::DynamicsExpansion::DynamicsExpansion(const Dynamics &d, const State &x, const Control &u)
    : state_lin_point_(x),
      control_lin_point_(u),
      const_term_(d(x, u)),
      state_jac_(compute_state_jac(d, x, u)),
      control_jac_(compute_control_jac(d, x, u)) {}

template <int S, int C> typename ILQR<S,C>::StateJacobian
ILQR<S,C>::DynamicsExpansion::compute_state_jac(
    const Dynamics &d, const State &s, const Control &u) {
    numerics::Callable<S,S> f = [d, u](const numerics::Vec<S> &s) {
        return d(s, u);
    };
    return numerics::compute_jacobian(f, s);
}

template <int S, int C> typename ILQR<S,C>::ControlJacobian
ILQR<S,C>::DynamicsExpansion::compute_control_jac(
    const Dynamics &d, const State &s, const Control &u) {
    const numerics::Callable<S,C> f = [d, s](const numerics::Vec<C> &u) {
        return d(s, u);
    };
    return numerics::compute_jacobian<S, C>(f, u);
}

template <int S, int C>
ILQR<S,C>::CostExpansion::CostExpansion(const Cost &c, const State &x, const Control &u, const bool is_terminal) :
    const_term_(c(x, u, is_terminal)),
    linear_term_(compute_jac(c, x, u, is_terminal)),
    quadratic_term_(compute_hess(c, x, u, is_terminal)) {}

template <int S, int C> typename ILQR<S, C>::CostJacobian
ILQR<S, C>::CostExpansion::compute_jac(const Cost &c, const State &x, const Control &u, const bool is_terminal) {
    const numerics::Callable<1, S+C> f = [c, is_terminal](const numerics::Vec<S+C> &x) {
        return numerics::Vec<1>{c(x.head(S), x.tail(C), is_terminal)};
    };
    numerics::Vec<S+C> set = numerics::Vec<S+C>::Zero();
    for (int i = 0; i < S; i++) {
        set(i) = x(i);
    }
    for (int i = 0; i < C; i++) {
        set(i+S) = u(i);
    }
    return numerics::compute_jacobian(f, set);
}

template <int S, int C> typename ILQR<S, C>::CostHessian
ILQR<S, C>::CostExpansion::compute_hess(const Cost &c, const State &x, const Control &u, const bool is_terminal) {
    const numerics::Callable<1, S+C> f = [c, is_terminal](const numerics::Vec<S+C> &x) {
        return numerics::Vec<1>{c(x.head(S), x.tail(C), is_terminal)};
    };
    numerics::Vec<S+C> set = numerics::Vec<S+C>::Zero();
    for (int i = 0; i < S; i++) {
        set(i) = x(i);
    }
    for (int i = 0; i < C; i++) {
        set(i+S) = u(i);
    }
    return numerics::compute_hessian(f, set);
}

template <int S, int C> double
ILQR<S, C>::CostExpansion::evaluate(const State &x, const Control &u) const {
    numerics::Vec<S+C> set = numerics::Vec<S+C>::Zero();
    for (int i = 0; i < S; i++) {
        set(i) = x(i);
    }
    for (int i = 0; i < C; i++) {
        set(i+S) = u(i);
    }
    return const_term_ + linear_term_ * set + .5 * set.transpose() * quadratic_term_ * set;
}
}  // namespace ilqr

