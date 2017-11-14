#pragma once
#include <functional>

#include "Eigen/Core"

namespace numerics{

template<int DIM>
using Vec = Eigen::Matrix<double, DIM, 1>;

template<int ROW, int COL>
using Mat = Eigen::Matrix<double, ROW, COL>;

template<int OUTPUT_DIM, int INPUT_DIM>
using Callable = std::function<Vec<OUTPUT_DIM>(const Vec<INPUT_DIM>&)>;

template<int OUTPUT_DIM, int INPUT_DIM> Mat<OUTPUT_DIM, INPUT_DIM> 
compute_jacobian(const Callable<OUTPUT_DIM, INPUT_DIM> &c, const Vec<INPUT_DIM> &set);

template<int DIM> Mat<DIM, DIM>
compute_hessian(const Callable<1, DIM> &c, const Vec<DIM> &set);

}

#include "numerical_diff_impl.hh"
