#include "lagrange-differentiator.hpp"
#include <cmath>
#include <stdexcept>
#include <utility>

namespace interpolation::differentiation {

LagrangeDifferentiator::LagrangeDifferentiator(plane2d::Points points)
    : points_(std::move(points)) {
    validate(points_);
}

void LagrangeDifferentiator::validate(const plane2d::Points& points) {
    if (points.size() < 3) {
        throw std::invalid_argument("At least three nodes are required");
    }
    plane2d::validate(points);
}

void LagrangeDifferentiator::validate(double x) {
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Evaluation point must be finite");
    }
}

void LagrangeDifferentiator::validate(const Derivatives& result) {
    if (!std::isfinite(result.first) || !std::isfinite(result.second)) {
        throw std::overflow_error("Non-finite derivative result");
    }
}

Derivatives LagrangeDifferentiator::evaluate(double x) const {
    validate(x);

    Derivatives result{0.0, 0.0};
    const std::size_t n = points_.size();

    for (std::size_t i = 0; i < n; ++i) {
        double denominator = 1.0;
        double p = 1.0;
        double d1 = 0.0;
        double d2 = 0.0;

        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            const double a = x - points_[j].x;
            d2 = a * d2 + 2.0 * d1;
            d1 = a * d1 + p;
            p = a * p;
            denominator *= (points_[i].x - points_[j].x);
        }

        const double weight = points_[i].y / denominator;
        result.first += weight * d1;
        result.second += weight * d2;
    }

    validate(result);
    return result;
}

}