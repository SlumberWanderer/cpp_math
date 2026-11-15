#pragma once

#include "polynom-core.hpp"

namespace polynomial::horner {

[[nodiscard]] double evaluate(const Coefficients& coeffs, double x);

}