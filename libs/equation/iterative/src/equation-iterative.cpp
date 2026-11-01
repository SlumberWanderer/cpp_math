#include "equation-iterative.hpp"
#include <cmath>
#include <limits>

namespace equation::iterative {

namespace {
constexpr double kEps = std::numeric_limits<double>::epsilon();
}

core::Result IterativeResolver::resolve(const core::ScalarFunction& func,
        const core::Range& range) const {
    iterations_count_ = 0;

    if (std::abs(func(range.left())) <= kEps) {
        return core::Result{core::ResultType::DEFINED, range.left(), 0};
    }
    if (std::abs(func(range.right())) <= kEps) {
        return core::Result{core::ResultType::DEFINED, range.right(), 0};
    }
    if (!has_root_for(func, range)) {
        return core::Result{core::ResultType::UNDEFINED, 0.0, 0};
    }

    auto step_fn = [&](double x) {
        return use_custom_phi_ ? phi_(x) : (x + factor_ * func(x));
    };

    double x_curr = range.center();

    while (iterations_count_ < max_iterations_) {
        ++iterations_count_;

        const double x_next = step_fn(x_curr);

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