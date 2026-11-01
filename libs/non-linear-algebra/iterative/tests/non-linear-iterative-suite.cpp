#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "non-linear-iterative.hpp"
#include <cmath>

using nonlinear::core::IterationOptions;
using nonlinear::core::NonlinearSystem;
using nonlinear::core::Vector;

TEST_CASE("Nonlinear Iterative: Variant 1 system", "[nonlinear-iterative]") {
    NonlinearSystem system;
    system.dimension = 2;
    system.function = [](const Vector& v) {
        const double x = v[0];
        const double y = v[1];
        return Vector{
            2.0 * x + std::cos(y) - 2.0,
            std::sin(x + y) - y - 1.2
        };
    };

    const auto phi = [](const Vector& v) {
        const double x = v[0];
        const double y = v[1];
        return Vector{
            1.0 - 0.5 * std::cos(y),
            std::sin(x + y) - 1.2
        };
    };

    const Vector init{1.1, -1.8};
    IterationOptions options;
    options.tolerance = 0.01;

    const auto result = nonlinear::iterative::resolve(system, phi, init, options);

    REQUIRE(result.converged());
    CHECK(result.iterations == 4);
    CHECK_THAT(result.solution[0], Catch::Matchers::WithinAbs(1.1467, 1e-3));
    CHECK_THAT(result.solution[1], Catch::Matchers::WithinAbs(-1.8613, 1e-3));
    CHECK(result.residual_norm < 0.01);
}