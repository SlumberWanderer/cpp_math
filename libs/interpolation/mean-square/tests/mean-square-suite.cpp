#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "mean-square.hpp"
#include <cmath>
#include <stdexcept>

using plane2d::Points;
using interpolation::mean_square::MeanSquare;

TEST_CASE("MeanSquare: Variant 1 approximations (degrees 1, 2, 3)", "[mean-square][variant1]") {
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

    const MeanSquare ms(variant1);

    const auto p1 = ms.approx(1);
    CHECK_THAT(p1.coefficient(0), Catch::Matchers::WithinAbs(-0.494865, 1e-5));
    CHECK_THAT(p1.coefficient(1), Catch::Matchers::WithinAbs(-0.280201, 1e-5));
    CHECK_THAT(ms.squared_error(p1), Catch::Matchers::WithinAbs(0.534132, 1e-5));

    const auto p2 = ms.approx(2);
    CHECK_THAT(p2.coefficient(0), Catch::Matchers::WithinAbs(-0.391997, 1e-5));
    CHECK_THAT(p2.coefficient(1), Catch::Matchers::WithinAbs(-0.308023, 1e-5));
    CHECK_THAT(p2.coefficient(2), Catch::Matchers::WithinAbs(-0.022816, 1e-5));
    CHECK_THAT(ms.squared_error(p2), Catch::Matchers::WithinAbs(0.475296, 1e-5));

    const auto p3 = ms.approx(3);
    CHECK_THAT(p3.coefficient(0), Catch::Matchers::WithinAbs(-0.412523, 1e-5));
    CHECK_THAT(p3.coefficient(1), Catch::Matchers::WithinAbs(-0.153528, 1e-5));
    CHECK_THAT(p3.coefficient(2), Catch::Matchers::WithinAbs(-0.040701, 1e-5));
    CHECK_THAT(p3.coefficient(3), Catch::Matchers::WithinAbs(-0.019473, 1e-5));
    CHECK_THAT(ms.squared_error(p3), Catch::Matchers::WithinAbs(0.435636, 1e-5));

    CHECK_FALSE(ms.has_only_positive_y());
    REQUIRE_THROWS_AS(ms.approx_exponential(), std::domain_error);
}

TEST_CASE("MeanSquare: exact polynomial and exponential fits", "[mean-square]") {
    SECTION("Fits exact quadratic 2 - 3x + x^2") {
        const Points quad{{-1.0, 6.0}, {0.0, 2.0}, {1.0, 0.0}, {2.0, 0.0}};
        const MeanSquare ms(quad);
        const auto p2 = ms.approx(2);
        CHECK_THAT(p2.coefficient(0), Catch::Matchers::WithinAbs(2.0, 1e-9));
        CHECK_THAT(p2.coefficient(1), Catch::Matchers::WithinAbs(-3.0, 1e-9));
        CHECK_THAT(p2.coefficient(2), Catch::Matchers::WithinAbs(1.0, 1e-9));
        CHECK_THAT(ms.squared_error(p2), Catch::Matchers::WithinAbs(0.0, 1e-10));
    }

    SECTION("Fits exact exponential 2 * exp(0.5 * x)") {
        const Points exp_pts{
            {0.0, 2.0},
            {1.0, 2.0 * std::exp(0.5)},
            {2.0, 2.0 * std::exp(1.0)}
        };
        const MeanSquare ms(exp_pts);
        REQUIRE(ms.has_only_positive_y());
        const auto fit = ms.approx_exponential();
        CHECK_THAT(fit.a, Catch::Matchers::WithinAbs(2.0, 1e-9));
        CHECK_THAT(fit.b, Catch::Matchers::WithinAbs(0.5, 1e-9));
        CHECK_THAT(ms.squared_error(fit), Catch::Matchers::WithinAbs(0.0, 1e-10));
    }
}