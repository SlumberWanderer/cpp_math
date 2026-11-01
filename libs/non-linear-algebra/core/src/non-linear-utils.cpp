#include "non-linear-utils.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace nonlinear::core::utils {

double norm(const Vector& v) noexcept {
    double max_val = 0.0;
    for (std::size_t i = 0; i < v.size(); ++i) {
        max_val = std::max(max_val, std::abs(v[i]));
    }
    return max_val;
}

double max_diff(const Vector& a, const Vector& b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("Vectors must have equal dimensions");
    }
    double max_val = 0.0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        max_val = std::max(max_val, std::abs(a[i] - b[i]));
    }
    return max_val;
}

bool is_finite(const Vector& v) noexcept {
    for (std::size_t i = 0; i < v.size(); ++i) {
        if (!std::isfinite(v[i])) {
            return false;
        }
    }
    return true;
}

void validate(const NonlinearSystem& system, const IterationOptions& options) {
    if (system.dimension == 0) {
        throw std::invalid_argument("System dimension must be greater than zero");
    }
    if (!system.function) {
        throw std::invalid_argument("Nonlinear system function is not defined");
    }
    if (options.max_iterations == 0) {
        throw std::invalid_argument("Max iterations must be positive");
    }
    if (options.tolerance <= 0.0 || !std::isfinite(options.tolerance)) {
        throw std::invalid_argument("Tolerance must be positive and finite");
    }
    if (options.jacobian_step <= 0.0 || !std::isfinite(options.jacobian_step)) {
        throw std::invalid_argument("Jacobian step must be positive and finite");
    }
}

Vector evaluate(const NonlinearSystem& system, const Vector& x) {
    Vector fx = system.function(x);
    if (fx.size() != system.dimension) {
        throw std::runtime_error("System function returned vector of invalid dimension");
    }
    return fx;
}

Matrix jacobian(const NonlinearSystem& system, const Vector& x, double step) {
    if (system.custom_jacobian) {
        return system.custom_jacobian(x);
    }

    const std::size_t n = system.dimension;
    Matrix J(n, n);
    Vector x_plus = x;
    Vector x_minus = x;

    for (std::size_t j = 0; j < n; ++j) {
        const double orig = x[j];
        x_plus[j] = orig + step;
        x_minus[j] = orig - step;

        const Vector f_plus = evaluate(system, x_plus);
        const Vector f_minus = evaluate(system, x_minus);

        if (!is_finite(f_plus) || !is_finite(f_minus)) {
            throw std::domain_error("Non-finite function value during Jacobian computation");
        }

        for (std::size_t i = 0; i < n; ++i) {
            J(i, j) = (f_plus[i] - f_minus[i]) / (2.0 * step);
        }

        x_plus[j] = orig;
        x_minus[j] = orig;
    }

    return J;
}

}