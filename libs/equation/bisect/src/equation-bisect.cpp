#include "equation-bisect.hpp"
#include <cmath>
#include <limits>

namespace equation::bisect {

namespace {
constexpr double kEps = std::numeric_limits<double>::epsilon();
}

core::Result BisectResolver::resolve(const core::ScalarFunction& func,
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

    core::Range current = range;

    while (iterations_count_ < max_iterations_) {
        ++iterations_count_;

        const double mid = current.center();
        const double f_mid = func(mid);

        if (std::abs(f_mid) <= kEps || (current.length() / 2.0) <= tolerance_) {
            return core::Result{core::ResultType::DEFINED, mid, iterations_count_};
        }

        const core::Range left_half = current.left_center();
        if (has_root_for(func, left_half)) {
            current = left_half;
        } else {
            current = current.right_center();
        }
    }

    return core::Result{core::ResultType::UNDEFINED, current.center(), iterations_count_};
}

}