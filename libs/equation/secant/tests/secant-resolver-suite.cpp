#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "equation-secant.hpp"
#include <cmath>

using equation::core::Range;
using equation::secant::SecantResolver;

TEST_CASE("SecantResolver: Variant 1 nonlinear equation", "[secant]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const Range range{0.2, 0.5};
    const SecantResolver resolver{0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 3);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2976, 1e-3));
    CHECK(std::abs(resolver.deviate(func, result.value())) < 0.01);
}