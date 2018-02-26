#pragma once
#include <functional>

#include "Eigen/Core"

namespace util {

using Callable = std::function<Eigen::VectorXd(const Eigen::VectorXd&)>;

Eigen::MatrixXd compute_jacobian(const Callable &c, const Eigen::VectorXd &set);

Eigen::MatrixXd compute_hessian(const Callable &c, const Eigen::VectorXd &set);

}  // namespace util
