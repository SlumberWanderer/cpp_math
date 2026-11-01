#include "equation-secant.hpp"
#include <cmath>
#include <limits>

namespace equation::secant {

namespace {
constexpr double kEps = std::numeric_limits<double>::epsilon();
constexpr double kDiffStep = 1e-5;

double second_derivative(const core::ScalarFunction& func, double x) {
    return (func(x + kDiffStep) - 2.0 * func(x) + func(x - kDiffStep)) / (kDiffStep * kDiffStep);
}
} 

core::Result SecantResolver::resolve(const core::ScalarFunction& func,
        const core::Range& range) const {
    iterations_count_ = 0;

    const double f_left = func(range.left());
    const double f_right = func(range.right());

    if (std::abs(f_left) <= kEps) {
        return core::Result{core::ResultType::DEFINED, range.left(), 0};
    }
    if (std::abs(f_right) <= kEps) {
        return core::Result{core::ResultType::DEFINED, range.right(), 0};
    }
    if (!has_root_for(func, range)) {
        return core::Result{core::ResultType::UNDEFINED, 0.0, 0};
    }

    const double d2f = second_derivative(func, range.center());
    const bool fourier_left = (f_left * d2f > 0.0);

    double x_prev = fourier_left ? range.right() : range.left();
    double x_curr = fourier_left ? range.left() : range.right();

    while (iterations_count_ < max_iterations_) {
        ++iterations_count_;

        const double f_curr = func(x_curr);
        const double f_prev = func(x_prev);
        const double denom = f_curr - f_prev;

        if (std::abs(denom) <= kEps || !std::isfinite(denom)) {
            return core::Result{core::ResultType::UNDEFINED, x_curr, iterations_count_};
        }

        const double x_next = x_curr - f_curr * (x_curr - x_prev) / denom;

        if (!std::isfinite(x_next)) {
            return core::Result{core::ResultType::UNDEFINED, x_next, iterations_count_};
        }

        if (std::abs(x_next - x_curr) <= tolerance_ || std::abs(func(x_next)) <= kEps) {
            return core::Result{core::ResultType::DEFINED, x_next, iterations_count_};
        }

        x_prev = x_curr;
        x_curr = x_next;
    }

    return core::Result{core::ResultType::UNDEFINED, x_curr, iterations_count_};
}

} 