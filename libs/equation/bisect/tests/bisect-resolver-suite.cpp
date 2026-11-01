#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "equation-bisect.hpp"
#include <cmath>

using equation::bisect::BisectResolver;
using equation::core::Range;

TEST_CASE("BisectResolver: Variant 1 nonlinear equation", "[bisect]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const Range range{0.2, 0.5};
    const BisectResolver resolver{0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 5);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.3031, 1e-3));
    CHECK(std::abs(resolver.deviate(func, result.value())) < 0.01);
}

TEST_CASE("BisectResolver: No root on interval returns UNDEFINED", "[bisect]") {
    const auto func = [](double x) { return x * x + 1.0; };
    const Range range{0.0, 2.0};
    const BisectResolver resolver{1e-4};

    const auto result = resolver.resolve(func, range);
    REQUIRE_FALSE(result.is_defined());
}