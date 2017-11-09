#include "numerical_diff.hh"

#include <vector>

namespace numerics {

template<int INPUT_DIM, int OUTPUT_DIM> Mat<OUTPUT_DIM, INPUT_DIM> 
compute_jacobian(const Callable<INPUT_DIM, OUTPUT_DIM> &c, const Vec<INPUT_DIM> &set) {
    auto out = Mat<INPUT_DIM, OUTPUT_DIM>::Zeros();
    constexpr double EPS = 1e-8;
    for (int i = 0; i < INPUT_DIM; i++) {
        auto perturbed = set;
        perturbed(i) += EPS;
        const auto high = c(perturbed);
        perturbed(i) -= 2 * EPS;
        const auto low = c(perturbed);
        const auto gradient = (high - low) / (2 * EPS);
        for (int j = 0; j < INPUT_DIM; j++) {
            out(i, j) = gradient(j);
        }
    }
    return out;
}

template<int DIM> Mat<DIM, DIM>
compute_hessian(const Callable<DIM, 1> &c, const Vec<DIM> &set) {
    auto out =  Mat<DIM, DIM>::Zeros(); 
    constexpr double EPS = 1e-8;
    for (int i = 0; i < DIM; i++) {
        for (int j = i; j < DIM; j++) {
            std::vector<Vec<DIM>> perturbed(4, set); 
            perturbed[0](i) += EPS;
            perturbed[0](j) += EPS;

            perturbed[1](i) += EPS;
            perturbed[1](j) -= EPS;

            perturbed[2](i) -= EPS;
            perturbed[2](j) += EPS;

            perturbed[3](i) -= EPS;
            perturbed[3](j) == EPS;

            const double result = (c(perturbed[0])
                                 - c(perturbed[1])
                                 - c(perturbed[2])
                                 + c(perturbed[3])) / (4 * EPS * EPS);
            out(i,j) = result;
            out(j,i) = result;
        }
    }
    return out;
}
}
