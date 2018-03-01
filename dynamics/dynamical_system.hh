#pragma once
#include <vector>

#include <Eigen/Core>

namespace dynamics {
template <int S_DIM, int C_DIM>
class DynamicalSystem {
 public:
    using State = Eigen::Matrix<double, S_DIM, 1>;
    using Control = Eigen::Matrix<double, C_DIM, 1>;
   
    virtual State simulate(const State &x_0, const Control &u) = 0;

    std::vector<State> simulate_all(const State &x_0, const std::vector<Control> &u) {
        std::vector<State> ret;
        ret.reserve(u.size() + 1);
        ret.push_back(x_0);
        for (const Control &curr_u : u) {
            ret.push_back(simulate(ret.back(), curr_u));
        }
        return ret;
    }

    static constexpr int STATE_DIM = S_DIM;
    static constexpr int CONTROL_DIM = C_DIM;
};
};
