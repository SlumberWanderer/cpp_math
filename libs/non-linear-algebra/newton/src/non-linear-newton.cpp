#include "non-linear-newton.hpp"
#include "non-linear-utils.hpp"
#include "partial-gauss.hpp"
#include <stdexcept>
#include <utility>

namespace nonlinear::newton {

IterationResult resolve(
    const NonlinearSystem& system,
    const Vector& init,
    const LinearResolver& ls_resolver,
    const IterationOptions& options
) {
    using core::ConvergenceStatus;
    using core::utils::evaluate;
    using core::utils::is_finite;
    using core::utils::jacobian;
    using core::utils::norm;
    using core::utils::validate;

    validate(system, options);

    if (init.size() != system.dimension) {
        throw std::invalid_argument("Incorrect initial vector dimension");
    }
    if (!is_finite(init)) {
        throw std::invalid_argument("Initial vector contains non-finite values");
    }
    if (!ls_resolver) {
        throw std::invalid_argument("Linear solver is not defined");
    }

    IterationResult result;
    result.solution = init;

    Vector x = init;
    Vector fx = evaluate(system, x);

    if (!is_finite(fx)) {
        result.status = ConvergenceStatus::NON_FINITE_VALUE;
        return result;
    }

    double residual = norm(fx);
    result.residual_norm = residual;

    if (residual <= 1e-15) {
        result.step_norm = 0.0;
        result.status = ConvergenceStatus::CONVERGED;
        return result;
    }

    for (std::size_t k = 0; k < options.max_iterations; ++k) {
        Matrix J(system.dimension, system.dimension);
        try {
            J = jacobian(system, x, options.jacobian_step);
        } catch (const std::domain_error&) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            result.solution = x;
            result.residual_norm = residual;
            result.iterations = k;
            return result;
        }

        Vector rhs = fx;
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            rhs[i] = -rhs[i];
        }

        Vector delta;
        try {
            delta = ls_resolver(J, rhs);
        } catch (const std::exception&) {
            result.status = ConvergenceStatus::SINGULAR_JACOBIAN;
            result.solution = x;
            result.residual_norm = residual;
            result.iterations = k;
            return result;
        }

        if (delta.size() != system.dimension || !is_finite(delta)) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            result.solution = x;
            result.residual_norm = residual;
            result.iterations = k;
            return result;
        }

        Vector candidate = x;
        for (std::size_t i = 0; i < system.dimension; ++i) {
            candidate[i] += delta[i];
        }

        if (!is_finite(candidate)) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            return result;
        }

        Vector candidate_fx = evaluate(system, candidate);
        if (!is_finite(candidate_fx)) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            return result;
        }

        const double step_norm = norm(delta);
        residual = norm(candidate_fx);

        x = std::move(candidate);
        fx = std::move(candidate_fx);

        result.solution = x;
        result.step_norm = step_norm;
        result.residual_norm = residual;
        result.iterations = k + 1;

        if (step_norm <= options.tolerance) {
            result.status = ConvergenceStatus::CONVERGED;
            return result;
        }
    }

    result.status = ConvergenceStatus::MAX_ITERS;
    return result;
}

IterationResult resolve(
    const NonlinearSystem& system,
    const Vector& init,
    const IterationOptions& options
) {
    const linear_algebra::gauss::PartialGaussResolver gauss_solver;
    const LinearResolver default_solver = [&gauss_solver](const Matrix& m, const Vector& rhs) {
        return gauss_solver.resolve(m, rhs);
    };
    return resolve(system, init, default_solver, options);
}

}