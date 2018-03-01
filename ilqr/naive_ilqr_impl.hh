#include "optimization/gradient_descent.hh"
#include "dynamics/pendulum.hh"

#include <cassert>
#include <iostream>

namespace ilqr {
namespace{
template <class T>
Eigen::VectorXd flatten_vector_of_control(const std::vector<T> &x) {
    assert(x.size() > 0);
    const int total_size = x.size() * x[0].size();
    const int num_controls = x.size();
    const int controls_size = x[0].size();
    Eigen::VectorXd result(total_size);
    for (int i = 0; i < num_controls; i++) {
        for (int j = 0; j < controls_size; j++) {
            const int idx = i * controls_size + j;
            result[idx] = x[i][j];
        }
    }
    return result;
}

template <class T>
std::vector<T> build_vector_of_control(
        const Eigen::VectorXd &x) {
    constexpr int CONTROL_SIZE = T::SizeAtCompileTime;
    assert(x.size() % CONTROL_SIZE == 0);
    const int num_controls = x.size() / CONTROL_SIZE;
    std::vector<T> result;
    result.reserve(num_controls);
    for (int i = 0; i < num_controls; i++) {
        T curr_u;
        for (int j = 0; j < CONTROL_SIZE; j++) {
            curr_u[j] = x[i * CONTROL_SIZE + j];
        }
        result.push_back(curr_u);
    }
    return result;
}
}  // namespace
template <class T> typename NaiveIlqr<T>::Trajectory
NaiveIlqr<T>::solve(const typename T::State &x0, const std::vector<typename T::Control> &u) const {
    assert(u.size() > 0);
    const int control_size = u[0].size();
    // Setup the problem
    optimization::OptimFunc opt_func = [this, x0, control_size](const Eigen::VectorXd &new_u) -> double {
        // Make the control easier to work with
        const std::vector<typename T::Control> controls =
            build_vector_of_control<typename T::Control>(new_u);
        // This is going to be really slow....
        // Roll forward dynamics
        const std::vector<typename T::State> traj = dynamics_->simulate_all(x0, controls); 

        // compute stepwise costs
        double total_cost = 0;
        const int num_states = traj.size();
        for (int i = 0; i < num_states; i++) {
            const typename T::State curr_state = traj[i];
            if (i == num_states - 1) {
                const typename T::Control zero_control = T::Control::Zero();
                total_cost += (*cost_function_)(curr_state, zero_control, true);
            } else {
                const typename T::Control curr_u = controls[i];
                total_cost += (*cost_function_)(curr_state, curr_u, false);
            }
        }
        return total_cost;
    };

    const optimization::GradientDescent::Config gd_config = {
        .alpha = .01,
        .line_search_enabled = false,
        .line_search_expected_decrease = 0.9,
        .f_tol = 1e-9,
        .max_iters = 10000,
    };

    const Eigen::VectorXd flat_controls = flatten_vector_of_control(u);
    optimization::GradientDescent opt(gd_config);
    optimization::OptimizationResult result = opt.optimize(opt_func, flat_controls);
    std::vector<typename T::Control> controls =
        build_vector_of_control<typename T::Control>(result.result);
    std::vector<typename T::State> traj = dynamics_->simulate_all(x0, controls);
    
    std::cout << result.result.transpose() << std::endl;
    for (const auto &traj_pt : traj) {
        std::cout <<  traj_pt.transpose() << std::endl;
    }


    
    return Trajectory{};
}
}  // namespace ilqr
