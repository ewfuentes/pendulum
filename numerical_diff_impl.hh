#include <vector>

#include <iostream>

namespace numerics {

template<int OUTPUT_DIM, int INPUT_DIM> Mat<OUTPUT_DIM, INPUT_DIM> 
compute_jacobian(const Callable<OUTPUT_DIM, INPUT_DIM> &c, const Vec<INPUT_DIM> &set) {
    Mat<OUTPUT_DIM, INPUT_DIM> out = Mat<OUTPUT_DIM, INPUT_DIM>::Zero();
    constexpr double EPS = 1e-4;
    for (int i = 0; i < INPUT_DIM; i++) {
        Vec<INPUT_DIM> perturbed = set;
        perturbed(i) += EPS;
        const Vec<OUTPUT_DIM> high = c(perturbed);
        perturbed(i) -= 2 * EPS;
        const Vec<OUTPUT_DIM> low = c(perturbed);
        const Vec<OUTPUT_DIM> gradient = (high - low) / (2 * EPS);
        for (int j = 0; j < OUTPUT_DIM; j++) {
            out(j, i) = gradient(j);
        }
    }
    return out;
}

template<int DIM> Mat<DIM, DIM>
compute_hessian(const Callable<1, DIM> &c, const Vec<DIM> &set) {
    Mat<DIM, DIM> out =  Mat<DIM, DIM>::Zero(); 
    constexpr double EPS = 1e-4;
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
            perturbed[3](j) -= EPS;

            const Vec<1> result = (c(perturbed[0])
                                 - c(perturbed[1])
                                 - c(perturbed[2])
                                 + c(perturbed[3])) / (4 * EPS * EPS);
            out(i,j) = result(0);
            out(j,i) = result(0);
        }
    }
    return out;
}
}
