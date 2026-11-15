#include "mean-square.hpp"
#include "partial-gauss.hpp"

#include <cmath>
#include <stdexcept>

namespace interpolation::mean_square {

using linear_algebra::RectangleMatrix;
using linear_algebra::Vector;
using linear_algebra::gauss::PartialGaussResolver;

MeanSquare::MeanSquare(const plane2d::Points& points)
    : points_(points) {
    plane2d::validate(points_);
}

std::pair<RectangleMatrix, Vector> MeanSquare::normal_system(std::size_t degree) const {
    if (degree >= points_.size()) {
        throw std::invalid_argument("Polynomial degree must be less than point count");
    }

    const std::size_t size = degree + 1;
    RectangleMatrix A(size, size, 0.0);
    Vector b(size, 0.0);

    for (const auto& [x, y] : points_) {
        std::vector<double> powers(size, 1.0);
        for (std::size_t j = 1; j < size; ++j) {
            powers[j] = powers[j - 1] * x;
        }
        for (std::size_t row = 0; row < size; ++row) {
            b[row] += y * powers[row];
            for (std::size_t col = 0; col < size; ++col) {
                A(row, col) += powers[row] * powers[col];
            }
        }
    }
    return {A, b};
}

polynomial::Polynomial MeanSquare::approx(std::size_t degree) const {
    const auto [A, b] = normal_system(degree);
    const PartialGaussResolver solver;
    const Vector coeffs = solver.resolve(A, b);
    return polynomial::Polynomial(coeffs);
}

double MeanSquare::squared_error(const polynomial::Polynomial& poly) const {
    double sum_sq = 0.0;
    for (const auto& [x, y] : points_) {
        const double diff = poly.evaluate(x) - y;
        sum_sq += diff * diff;
    }
    return sum_sq;
}

double MeanSquare::rmse(const polynomial::Polynomial& poly) const {
    return std::sqrt(squared_error(poly) / static_cast<double>(points_.size()));
}

bool MeanSquare::has_only_positive_y() const noexcept {
    for (const auto& pt : points_) {
        if (pt.y <= 0.0) {
            return false;
        }
    }
    return true;
}

ExponentialFit MeanSquare::approx_exponential() const {
    if (!has_only_positive_y()) {
        throw std::domain_error("Exponential approximation requires all y > 0");
    }

    plane2d::Points log_points;
    log_points.reserve(points_.size());
    for (const auto& [x, y] : points_) {
        log_points.emplace_back(x, std::log(y));
    }

    const MeanSquare log_ms(log_points);
    const auto lin_poly = log_ms.approx(1);
    return ExponentialFit{std::exp(lin_poly.coefficient(0)), lin_poly.coefficient(1)};
}

double MeanSquare::squared_error(const ExponentialFit& exp_fit) const {
    double sum_sq = 0.0;
    for (const auto& [x, y] : points_) {
        const double diff = exp_fit.evaluate(x) - y;
        sum_sq += diff * diff;
    }
    return sum_sq;
}

}