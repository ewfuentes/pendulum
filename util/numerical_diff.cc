#include <cassert>

#include <vector>

#include "util/numerical_diff.hh"

namespace util {
Eigen::MatrixXd compute_jacobian(const Callable &c, const Eigen::VectorXd &set) {
    Eigen::VectorXd test_eval = c(set);
    const int num_cols = set.size();
    const int num_rows = test_eval.size();
    Eigen::MatrixXd out = Eigen::MatrixXd(num_rows, num_cols);
    constexpr double EPS = 1e-4;
    for (int i = 0; i < num_cols; i++) {
        Eigen::VectorXd perturbed = set;
        perturbed(i) += EPS;
        const Eigen::VectorXd high = c(perturbed);
        perturbed(i) -= 2 * EPS;
        const Eigen::VectorXd low = c(perturbed);
        const Eigen::VectorXd gradient = (high - low) / (2 * EPS);
        out.col(i) = gradient;
    }
    return out;
}

Eigen::MatrixXd compute_hessian(const Callable &c, const Eigen::VectorXd &set) {
    Eigen::VectorXd test_eval = c(set);
    assert(test_eval.size() == 1);
    Eigen::MatrixXd out =  Eigen::MatrixXd(set.size(), set.size()); 
    constexpr double EPS = 1e-4;
    for (int i = 0; i < set.size(); i++) {
        for (int j = i; j < set.size(); j++) {
            std::vector<Eigen::VectorXd> perturbed(4, set); 
            perturbed[0](i) += EPS;
            perturbed[0](j) += EPS;

            perturbed[1](i) += EPS;
            perturbed[1](j) -= EPS;

            perturbed[2](i) -= EPS;
            perturbed[2](j) += EPS;

            perturbed[3](i) -= EPS;
            perturbed[3](j) -= EPS;

            const Eigen::VectorXd result = (c(perturbed[0])
                                 - c(perturbed[1])
                                 - c(perturbed[2])
                                 + c(perturbed[3])) / (4 * EPS * EPS);
            out(i,j) = result(0);
            out(j,i) = result(0);
        }
    }
    return out;
}
}  // namespace util
