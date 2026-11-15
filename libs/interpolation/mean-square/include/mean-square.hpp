#pragma once

#include <cmath>
#include <cstddef>
#include "plane2d.hpp"
#include "polynom.hpp"
#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace interpolation::mean_square {

struct ExponentialFit {
    double a{0.0};
    double b{0.0};

    [[nodiscard]] double evaluate(double x) const noexcept {
        return a * std::exp(b * x);
    }

    [[nodiscard]] double operator()(double x) const noexcept {
        return evaluate(x);
    }
};

class MeanSquare {
private:
    plane2d::Points points_;

public:
    explicit MeanSquare(const plane2d::Points& points);

    [[nodiscard]] polynomial::Polynomial approx(std::size_t degree) const;
    [[nodiscard]] double squared_error(const polynomial::Polynomial& poly) const;
    [[nodiscard]] double rmse(const polynomial::Polynomial& poly) const;

    [[nodiscard]] bool has_only_positive_y() const noexcept;
    [[nodiscard]] ExponentialFit approx_exponential() const;
    [[nodiscard]] double squared_error(const ExponentialFit& exp_fit) const;

    [[nodiscard]] std::pair<linear_algebra::RectangleMatrix, linear_algebra::Vector>
    normal_system(std::size_t degree) const;
};

}