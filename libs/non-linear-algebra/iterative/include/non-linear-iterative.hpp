#pragma once

#include "non-linear-core.hpp"

namespace nonlinear::iterative {

using core::IterationOptions;
using core::IterationResult;
using core::NonlinearFunction;
using core::NonlinearSystem;
using core::Vector;

[[nodiscard]] IterationResult resolve(
    const NonlinearSystem& system,
    const NonlinearFunction& phi_function,
    const Vector& init,
    const IterationOptions& options = {}
);

}