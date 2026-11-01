#pragma once

#include "non-linear-core.hpp"

namespace nonlinear::newton {

using core::IterationOptions;
using core::IterationResult;
using core::LinearResolver;
using core::Matrix;
using core::NonlinearSystem;
using core::Vector;

[[nodiscard]] IterationResult resolve(
    const NonlinearSystem& system,
    const Vector& init,
    const LinearResolver& ls_resolver,
    const IterationOptions& options = {}
);

[[nodiscard]] IterationResult resolve(
    const NonlinearSystem& system,
    const Vector& init,
    const IterationOptions& options = {}
);

} 