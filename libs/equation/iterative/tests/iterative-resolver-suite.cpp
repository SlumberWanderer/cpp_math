#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "equation-iterative.hpp"
#include <cmath>

using equation::core::Range;
using equation::iterative::IterativeResolver;

TEST_CASE("IterativeResolver: Variant 1 with canonical phi(x)", "[iterative]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };
    const auto phi = [](double x) {
        const double c = std::cos(std::cos(0.387 * x));
        return c * c;
    };

    const Range range{0.2, 0.5};
    const IterativeResolver resolver{phi, 0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 2);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2981, 1e-3));
}

TEST_CASE("IterativeResolver: Relaxation factor mode", "[iterative]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const Range range{0.2, 0.5};
    const IterativeResolver resolver{-0.9, 0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2981, 1e-2));
}