#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "lagrange-differentiator.hpp"
#include <cmath>
#include <limits>
#include <stdexcept>

using interpolation::differentiation::LagrangeDifferentiator;
using plane2d::Points;

TEST_CASE("LagrangeDifferentiator: exact derivatives for polynomials", "[differentiation]") {
    SECTION("Quadratic polynomial y = 2x^2 - 3x + 5") {
        const Points pts{
            {-1.0, 10.0},
            { 0.5,  4.0},
            { 2.0,  7.0}
        };
        const LagrangeDifferentiator diff(pts);

        const auto res = diff.evaluate(1.25);
        CHECK_THAT(res.first, Catch::Matchers::WithinAbs(2.0, 1e-9));
        CHECK_THAT(res.second, Catch::Matchers::WithinAbs(4.0, 1e-9));
    }

    SECTION("Cubic polynomial y = x^3 - 2x^2 + 3x - 4") {
        const auto f = [](double x) { return x * x * x - 2.0 * x * x + 3.0 * x - 4.0; };
        const auto df = [](double x) { return 3.0 * x * x - 4.0 * x + 3.0; };
        const auto d2f = [](double x) { return 6.0 * x - 4.0; };

        const Points pts{
            {-1.0, f(-1.0)},
            { 0.0, f(0.0)},
            { 1.5, f(1.5)},
            { 3.0, f(3.0)}
        };
        const LagrangeDifferentiator diff(pts);

        for (const double x : {-0.5, 0.75, 2.2}) {
            const auto res = diff.evaluate(x);
            CHECK_THAT(res.first, Catch::Matchers::WithinAbs(df(x), 1e-9));
            CHECK_THAT(res.second, Catch::Matchers::WithinAbs(d2f(x), 1e-9));
        }
    }
}

TEST_CASE("LagrangeDifferentiator: Variant 1 Task 1 and Task 2 verification", "[differentiation][variant1]") {
    SECTION("Task 1: non-uniform 5-node grid") {
        const Points nodes{
            {0.0, 1.00},
            {0.4, 1.49},
            {1.1, 2.65},
            {1.7, 4.13},
            {2.5, 7.25}
        };
        const LagrangeDifferentiator diff(nodes);

        const auto r1 = diff.evaluate(0.3);
        CHECK_THAT(r1.first, Catch::Matchers::WithinAbs(1.284354, 1e-5));
        CHECK_THAT(r1.second, Catch::Matchers::WithinAbs(0.652476, 1e-5));

        const auto r2 = diff.evaluate(1.3);
        CHECK_THAT(r2.first, Catch::Matchers::WithinAbs(2.297821, 1e-5));
        CHECK_THAT(r2.second, Catch::Matchers::WithinAbs(1.463724, 1e-5));

        const auto r3 = diff.evaluate(2.2);
        CHECK_THAT(r3.first, Catch::Matchers::WithinAbs(4.117262, 1e-5));
        CHECK_THAT(r3.second, Catch::Matchers::WithinAbs(2.651784, 1e-5));
    }

    SECTION("Task 2: tabulated f(x) = exp(x/2)*sin(2x) + x^2 on [0, 2], h = 0.25") {
        const auto f = [](double x) {
            return std::exp(x / 2.0) * std::sin(2.0 * x) + x * x;
        };
        const auto df = [](double x) {
            return std::exp(x / 2.0) * (0.5 * std::sin(2.0 * x) + 2.0 * std::cos(2.0 * x)) + 2.0 * x;
        };
        const auto d2f = [](double x) {
            return std::exp(x / 2.0) * (-3.75 * std::sin(2.0 * x) + 2.0 * std::cos(2.0 * x)) + 2.0;
        };

        const Points nodes = plane2d::tabulate_by_step(f, 0.0, 2.0, 0.25);
        REQUIRE(nodes.size() == 9);

        const LagrangeDifferentiator diff(nodes);
        for (const double t : {0.35, 1.15, 1.8}) {
            const auto res = diff.evaluate(t);
            CHECK_THAT(res.first, Catch::Matchers::WithinAbs(df(t), 1e-3));
            CHECK_THAT(res.second, Catch::Matchers::WithinAbs(d2f(t), 5e-3));
        }
    }
}

TEST_CASE("LagrangeDifferentiator: input validation", "[differentiation]") {
    SECTION("Rejects fewer than 3 nodes") {
        REQUIRE_THROWS_AS(LagrangeDifferentiator(Points{}), std::invalid_argument);
        REQUIRE_THROWS_AS(LagrangeDifferentiator(Points{{0.0, 1.0}, {1.0, 2.0}}), std::invalid_argument);
    }

    SECTION("Rejects duplicate x coordinates and non-finite inputs") {
        const Points dup{{0.0, 1.0}, {1.0, 2.0}, {1.0, 3.0}};
        REQUIRE_THROWS_AS(LagrangeDifferentiator(dup), std::invalid_argument);

        const Points inf{{0.0, 1.0}, {1.0, std::numeric_limits<double>::infinity()}, {2.0, 3.0}};
        REQUIRE_THROWS_AS(LagrangeDifferentiator(inf), std::invalid_argument);

        const LagrangeDifferentiator valid(Points{{0.0, 0.0}, {1.0, 1.0}, {2.0, 4.0}});
        REQUIRE_THROWS_AS(valid.evaluate(std::numeric_limits<double>::quiet_NaN()), std::invalid_argument);
    }
}