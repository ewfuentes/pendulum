#pragma once

#include <functional>
#include <vector>

#include "Eigen/Core"

namespace ilqr {
template <int STATE_DIM, int CONTROL_DIM>
class ILQR {
public:
    using State = Eigen::Matrix<double, STATE_DIM, 1>;
    using Control = Eigen::Matrix<double, CONTROL_DIM, 1>;
    using StateJacobian = Eigen::Matrix<double, STATE_DIM, STATE_DIM>;
    using ControlJacobian = Eigen::Matrix<double, STATE_DIM, CONTROL_DIM>;

    using CostJacobian = Eigen::Matrix<double, 1, STATE_DIM + CONTROL_DIM>;
    using CostHessian = Eigen::Matrix<double, STATE_DIM + CONTROL_DIM, STATE_DIM + CONTROL_DIM>;

    using Dynamics = std::function<State(const State&,const Control&)>;
    using Cost = std::function<double(const State&, const Control&, const bool)>;

    class DynamicsExpansion {
    public:
        DynamicsExpansion(const Dynamics &d, const State &x, const Control &u); 
        State evaluate(const State &x, const Control &u) const;
        const State& const_term() { return const_term_; };
        const StateJacobian& state_jac() { return state_jac_; };
        const ControlJacobian& control_jac() { return control_jac_; };

    private:
        StateJacobian compute_state_jac(const Dynamics &d, const State &x, const Control &u);
        ControlJacobian compute_control_jac(const Dynamics &d, const State &x, const Control &u);
        const State state_lin_point_;
        const Control control_lin_point_;

        const State const_term_;
        const StateJacobian state_jac_;
        const ControlJacobian control_jac_;
    };

    class CostExpansion {
    public:
        CostExpansion(const Cost &c, const State &x, const Control &u, const bool is_terminal);
        double evaluate(const State &x, const Control &u) const;
        double const_term() { return const_term_; };
        const CostJacobian& linear_term() { return linear_term_; };
        const CostHessian& quadratic_term() { return quadratic_term_; }; 

        const Eigen::Matrix<double, STATE_DIM, STATE_DIM> state_quadratic() {
            return quadratic_term_.block(STATE_DIM, STATE_DIM, 0, 0);
        };
        const Eigen::Matrix<double, CONTROL_DIM, CONTROL_DIM> control_quadratic() {
            return quadratic_term_.block(CONTROL_DIM, CONTROL_DIM, STATE_DIM, STATE_DIM);
        };

        const Eigen::Matrix<double, STATE_DIM, CONTROL_DIM> cross_quadratic() {
            return quadratic_term_.block(STATE_DIM, CONTROL_DIM, STATE_DIM, 0);
        };

    private:
        CostJacobian compute_jac(const Cost &c, const State &x, const Control &u, const bool is_terminal);
        CostHessian compute_hess(const Cost &c, const State &x, const Control &u, const bool is_terminal);
        const State state_lin_point_;
        const Control control_lin_point_;

        const double const_term_;
        const CostJacobian linear_term_;
        const CostHessian quadratic_term_;
    };

    struct Result {

    };

    ILQR(const Dynamics &d, const Cost &cost) : dynamics_(d), cost_(cost) {};

    Result solve(const State &x0, const std::vector<Control> &u);
private:

    std::vector<ILQR<STATE_DIM, CONTROL_DIM>> forward_pass(const State &x0, const std::vector<Control> &u);
    void backward_pass(const std::vector<State> &x0,
                       const std::vector<Control> &u,
                       const std::vector<DynamicsExpansion> &dyn_ex,
                       const std::vector<CostExpansion> &cost_ex);
    
    const Dynamics dynamics_;
    const Cost cost_;
};

}  // namespace ilqr

#include "ilqr_impl.hh"
