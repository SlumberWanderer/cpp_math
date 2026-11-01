#pragma once

#include "non-linear-core.hpp"

namespace nonlinear::core::utils {

[[nodiscard]] double norm(const Vector& v) noexcept;
[[nodiscard]] double max_diff(const Vector& a, const Vector& b);
[[nodiscard]] bool is_finite(const Vector& v) noexcept;
void validate(const NonlinearSystem& system, const IterationOptions& options);
[[nodiscard]] Vector evaluate(const NonlinearSystem& system, const Vector& x);
[[nodiscard]] Matrix jacobian(const NonlinearSystem& system, const Vector& x, double step);

}