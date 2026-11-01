#pragma once

#include <cstddef>
#include <functional>
#include <limits>
#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace nonlinear::core {

using Matrix = linear_algebra::RectangleMatrix;
using Vector = linear_algebra::Vector;
using num_limits = std::numeric_limits<double>;

using NonlinearFunction = std::function<Vector(const Vector&)>;
using JacobianFunction = std::function<Matrix(const Vector&)>;
using LinearResolver = std::function<Vector(const Matrix&, const Vector&)>;

struct NonlinearSystem {
    std::size_t dimension = 0;
    NonlinearFunction function;
    JacobianFunction custom_jacobian = nullptr;
};

struct IterationOptions {
    std::size_t max_iterations = 100;
    double tolerance = 1e-6;
    double jacobian_step = 1e-6;
    double min_damping = 1.0 / 1024.0;
};

enum class ConvergenceStatus {
    CONVERGED,
    MAX_ITERS,
    SINGULAR_JACOBIAN,
    STAGNATION,
    NON_FINITE_VALUE
};

struct IterationResult {
    Vector solution;
    std::size_t iterations = 0;
    double step_norm = num_limits::infinity();
    double residual_norm = num_limits::infinity();
    ConvergenceStatus status = ConvergenceStatus::MAX_ITERS;

    [[nodiscard]] bool converged() const noexcept {
        return status == ConvergenceStatus::CONVERGED;
    }
};

} 