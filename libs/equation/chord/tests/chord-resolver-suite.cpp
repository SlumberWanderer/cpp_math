#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "equation-chord.hpp"
#include <cmath>

using equation::chord::ChordResolver;
using equation::core::Range;

TEST_CASE("ChordResolver: Variant 1 nonlinear equation", "[chord]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const Range range{0.2, 0.5};
    const ChordResolver resolver{0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 3);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2982, 1e-3));
    CHECK(std::abs(resolver.deviate(func, result.value())) < 0.01);
}

TEST_CASE("ChordResolver: Simple quadratic equation", "[chord]") {
    const auto func = [](double x) { return x * x - 4.0; };
    const Range range{1.0, 3.0};
    const ChordResolver resolver{1e-6};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(2.0, 1e-5));
}