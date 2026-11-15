#pragma once

#include <cstddef>
#include "plane2d.hpp"

namespace interpolation::lagrange {

[[nodiscard]] double basis_polynomial(const plane2d::Points& points, std::size_t index, double x);
[[nodiscard]] double evaluate(const plane2d::Points& points, double x);

}