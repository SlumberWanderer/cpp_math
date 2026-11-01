#include "equation-newton.hpp"
#include <cmath>
#include <limits>

namespace equation::newton {

namespace {
constexpr double kEps = std::numeric_limits<double>::epsilon();
constexpr double kDiffStep = 1e-6;

double first_derivative(const core::ScalarFunction& func, double x) {
    return (func(x + kDiffStep) - func(x - kDiffStep)) / (2.0 * kDiffStep);
}

double second_derivative(const core::ScalarFunction& func, double x) {
    return (func(x + kDiffStep) - 2.0 * func(x) + func(x - kDiffStep)) / (kDiffStep * kDiffStep);
}
} 

core::Result NewtonResolver::resolve(const core::ScalarFunction& func,
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

    auto df = [&](double x) {
        return has_custom_derivative_ ? derivative_(x) : first_derivative(func, x);
    };

    const double d2f = second_derivative(func, range.center());
    double x_curr = (f_left * d2f > 0.0) ? range.left() : range.right();

    while (iterations_count_ < max_iterations_) {
        ++iterations_count_;

        const double f_curr = func(x_curr);
        const double df_curr = df(x_curr);

        if (std::abs(df_curr) <= kEps || !std::isfinite(df_curr)) {
            return core::Result{core::ResultType::UNDEFINED, x_curr, iterations_count_};
        }

        const double x_next = x_curr - f_curr / df_curr;

        if (!std::isfinite(x_next)) {
            return core::Result{core::ResultType::UNDEFINED, x_next, iterations_count_};
        }

        if (std::abs(x_next - x_curr) <= tolerance_ || std::abs(func(x_next)) <= kEps) {
            return core::Result{core::ResultType::DEFINED, x_next, iterations_count_};
        }

        x_curr = x_next;
    }

    return core::Result{core::ResultType::UNDEFINED, x_curr, iterations_count_};
}

} 