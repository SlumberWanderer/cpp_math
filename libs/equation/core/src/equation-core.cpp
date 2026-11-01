#include "equation-core.hpp"
#include <cmath>

namespace equation::core {

bool IEquationResolver::has_root_for(const ScalarFunction& func, const Range& range) {
    const double f_left = func(range.left());
    const double f_right = func(range.right());
    return std::signbit(f_left) != std::signbit(f_right);
}

} 