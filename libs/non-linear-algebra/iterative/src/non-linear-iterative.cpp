#include "non-linear-iterative.hpp"
#include "non-linear-utils.hpp"
#include <stdexcept>
#include <utility>

namespace nonlinear::iterative {

IterationResult resolve(
    const NonlinearSystem& system,
    const NonlinearFunction& phi_function,
    const Vector& init,
    const IterationOptions& options
) {
    using core::ConvergenceStatus;
    using core::utils::evaluate;
    using core::utils::is_finite;
    using core::utils::max_diff;
    using core::utils::norm;
    using core::utils::validate;

    validate(system, options);

    if (!phi_function) {
        throw std::invalid_argument("Iteration phi function is not defined");
    }
    if (init.size() != system.dimension) {
        throw std::invalid_argument("Incorrect initial vector dimension");
    }
    if (!is_finite(init)) {
        throw std::invalid_argument("Initial vector contains non-finite values");
    }

    IterationResult result;
    result.solution = init;

    Vector x = init;
    Vector fx = evaluate(system, x);

    if (!is_finite(fx)) {
        result.status = ConvergenceStatus::NON_FINITE_VALUE;
        return result;
    }

    result.residual_norm = norm(fx);

    for (std::size_t k = 0; k < options.max_iterations; ++k) {
        Vector x_next = phi_function(x);

        if (x_next.size() != system.dimension || !is_finite(x_next)) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            result.solution = x;
            result.iterations = k;
            return result;
        }

        const double step_norm = max_diff(x_next, x);
        Vector fx_next = evaluate(system, x_next);

        if (!is_finite(fx_next)) {
            result.status = ConvergenceStatus::NON_FINITE_VALUE;
            return result;
        }

        const double residual = norm(fx_next);

        x = std::move(x_next);
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

} 