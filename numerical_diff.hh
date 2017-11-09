#include <functional>

#include "Eigen/Core"

namespace numerics{

template<int DIM>
using Vec = Eigen::Matrix<double, DIM, 1>;

template<int ROW, int COL>
using Mat = Eigen::Matrix<double, ROW, COL>;

template<int OUTPUT_DIM, int INPUT_DIM>
using Callable = std::function<Vec<OUTPUT_DIM>(const Vec<INPUT_DIM>&)>;


template<int INPUT_DIM, int OUTPUT_DIM> Mat<OUTPUT_DIM, INPUT_DIM> 
compute_jacobian(const Callable<INPUT_DIM, OUTPUT_DIM> &c, const Vec<INPUT_DIM> &set);

template<int DIM> Mat<DIM, DIM>
compute_hessian(const Callable<DIM, 1> &c, const Vec<DIM> &set);

}
