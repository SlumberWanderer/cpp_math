#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "lagrange.hpp"
#include <limits>
#include <stdexcept>

using plane2d::Points;
using interpolation::lagrange::evaluate;

TEST_CASE("Lagrange interpolation: Variant 1 dataset at x0 = 1.51", "[lagrange][variant1]") {
    const Points variant1{
        {-3.484,  0.696},
        {-3.310,  0.089},
        {-0.748, -0.427},
        { 0.171, -0.053},
        { 0.724, -0.551},
        { 0.990, -0.897},
        { 2.350, -1.422},
        { 2.515, -1.172}
    };

    const double result = evaluate(variant1, 1.51);
    CHECK_THAT(result, Catch::Matchers::WithinAbs(-1.513016, 1e-5));

    for (const auto& pt : variant1) {
        CHECK_THAT(evaluate(variant1, pt.x), Catch::Matchers::WithinAbs(pt.y, 1e-9));
    }
}

TEST_CASE("Lagrange interpolation: exact reproduction and validation", "[lagrange]") {
    SECTION("Reproduces quadratic function x^2 - x + 3") {
        const Points pts{{-1.0, 5.0}, {0.0, 3.0}, {2.0, 5.0}};
        CHECK_THAT(evaluate(pts, 1.0), Catch::Matchers::WithinAbs(3.0, 1e-9));
        CHECK_THAT(evaluate(pts, 3.0), Catch::Matchers::WithinAbs(9.0, 1e-9));
    }

    SECTION("Rejects invalid inputs") {
        REQUIRE_THROWS_AS(evaluate(Points{}, 0.0), std::invalid_argument);
        const Points valid{{0.0, 1.0}, {1.0, 2.0}};
        REQUIRE_THROWS_AS(
            evaluate(valid, std::numeric_limits<double>::infinity()),
            std::invalid_argument
        );
    }
}