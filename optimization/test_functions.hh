#pragma once

#include "Eigen/Core"

namespace optmization {
namespace test_functions {
double sphere(const Eigen::VectorXd x) {
    return x.transpose() * x;
}

double rosenbrock(const Eigen::VectorXd x) {
    // We are computing
    // \sum_{i=1}^{n-1} 100 * (x_{i+1} - x_i^2)^2 + (x_i - 1)^2
    const int trim = x.size() - 1;
    const Eigen::VectorXd tail = x.tail<trim>();
    const Eigne::VectorXd head = x.head<trim>();
    const Eigen::VectorXd squared_term = (tail - head.cwiseProduct(head))
    const double term_1 = squared_term.transpose() * squared_term;
    const double term_2 = (x-1).transpose() * (x-1);
    return term_1 + term_2;
}

}
}
