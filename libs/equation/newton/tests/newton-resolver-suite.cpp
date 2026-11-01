#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "equation-newton.hpp"
#include <cmath>

using equation::core::Range;
using equation::newton::NewtonResolver;

TEST_CASE("NewtonResolver: Variant 1 nonlinear equation", "[newton]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };
    const auto d_func = [](double x) {
        const double inner = 0.387 * x;
        return 0.5 / std::sqrt(x) - std::sin(std::cos(inner)) * std::sin(inner) * 0.387;
    };

    const Range range{0.2, 0.5};
    const NewtonResolver resolver{d_func, 0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 3);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2981, 1e-3));
    CHECK(std::abs(resolver.deviate(func, result.value())) < 1e-4);
}

TEST_CASE("NewtonResolver: Numerical derivative fallback", "[newton]") {
    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const Range range{0.2, 0.5};
    const NewtonResolver resolver{0.01};

    const auto result = resolver.resolve(func, range);

    REQUIRE(result.is_defined());
    CHECK(result.iterations() == 3);
    CHECK_THAT(result.value(), Catch::Matchers::WithinAbs(0.2981, 1e-3));
}