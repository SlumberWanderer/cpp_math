#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "non-linear-newton.hpp"
#include <cmath>

using nonlinear::core::IterationOptions;
using nonlinear::core::NonlinearSystem;
using nonlinear::core::Vector;

TEST_CASE("Nonlinear Newton: Variant 1 system", "[nonlinear-newton]") {
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

    const Vector init{1.1, -1.8};
    IterationOptions options;
    options.tolerance = 0.01;

    const auto result = nonlinear::newton::resolve(system, init, options);

    REQUIRE(result.converged());
    CHECK(result.iterations == 2);
    CHECK_THAT(result.solution[0], Catch::Matchers::WithinAbs(1.1405, 1e-3));
    CHECK_THAT(result.solution[1], Catch::Matchers::WithinAbs(-1.8557, 1e-3));
    CHECK(result.residual_norm < 1e-3);
}