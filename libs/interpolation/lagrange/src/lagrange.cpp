#include "lagrange.hpp"

#include <cmath>
#include <stdexcept>

namespace interpolation::lagrange {

double basis_polynomial(const plane2d::Points& points, std::size_t index, double x) {
    plane2d::validate(points);
    if (index >= points.size()) {
        throw std::out_of_range("Lagrange basis index out of range");
    }
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Interpolation argument x must be finite");
    }

    double weight = 1.0;
    const double xi = points[index].x;
    for (std::size_t j = 0; j < points.size(); ++j) {
        if (j != index) {
            weight *= (x - points[j].x) / (xi - points[j].x);
        }
    }
    return weight;
}

double evaluate(const plane2d::Points& points, double x) {
    plane2d::validate(points);
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Interpolation argument x must be finite");
    }

    double sum = 0.0;
    const std::size_t n = points.size();
    for (std::size_t i = 0; i < n; ++i) {
        double l_i = 1.0;
        const auto [xi, yi] = points[i];
        for (std::size_t j = 0; j < n; ++j) {
            if (i != j) {
                l_i *= (x - points[j].x) / (xi - points[j].x);
            }
        }
        sum += yi * l_i;
    }
    return sum;
}

}