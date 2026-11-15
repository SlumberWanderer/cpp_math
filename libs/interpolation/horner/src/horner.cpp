#include "horner.hpp"

#include <cmath>
#include <stdexcept>

namespace polynomial::horner {

double evaluate(const Coefficients& coeffs, double x) {
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Evaluation point x must be finite");
    }
    for (const double c : coeffs) {
        if (!std::isfinite(c)) {
            throw std::invalid_argument("Polynomial coefficient must be finite");
        }
    }
    if (coeffs.empty()) {
        return 0.0;
    }

    double result = 0.0;
    for (auto it = coeffs.crbegin(); it != coeffs.crend(); ++it) {
        result = result * x + *it;
    }
    return result;
}

}