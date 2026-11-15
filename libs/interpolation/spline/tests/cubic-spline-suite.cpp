#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "clamp-boundary-generator.hpp"
#include "cubic-spline.hpp"
#include "free-boundary-generator.hpp"
#include "natural-boundary-generator.hpp"
#include <cmath>
#include <memory>
#include <stdexcept>

using plane2d::Points;
using interpolation::spline::core::boundary::ClampBoundaryEquationGenerator;
using interpolation::spline::core::boundary::FreeBoundaryEquationGenerator;
using interpolation::spline::core::boundary::NaturalBoundaryEquationGenerator;
using interpolation::spline::cubic::CubicSpline;

namespace {
CubicSpline::Generator natural_bc() {
    return std::make_shared<NaturalBoundaryEquationGenerator>();
}
CubicSpline::Generator clamped_bc(double slope) {
    return std::make_shared<ClampBoundaryEquationGenerator>(slope);
}
CubicSpline::Generator free_bc() {
    return std::make_shared<FreeBoundaryEquationGenerator>();
}
} 

TEST_CASE("CubicSpline: Variant 1 Task 3 and Task 4 verification", "[spline][variant1]") {
    SECTION("Task 3: Table 5.1 Variant 1 at x0 = 1.51") {
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

        const CubicSpline spline(variant1);
        CHECK_THAT(spline.evaluate(1.51), Catch::Matchers::WithinAbs(-1.490393, 1e-5));

        for (const auto& pt : variant1) {
            CHECK_THAT(spline.evaluate(pt.x), Catch::Matchers::WithinAbs(pt.y, 1e-9));
        }
    }

    SECTION("Task 4: Tabulated f(x) on [0, 2] with h = 0.5") {
        const auto f = [](double x) {
            return std::exp(0.3 * x) * std::cos(1.2 * x) + std::log(x + 4.0);
        };
        const Points pts = plane2d::tabulate_by_step(f, 0.0, 2.0, 0.5);
        const CubicSpline spline(pts);

        CHECK_THAT(spline.evaluate(0.167), Catch::Matchers::WithinAbs(2.439587, 1e-5));
        CHECK_THAT(spline.evaluate(1.51), Catch::Matchers::WithinAbs(1.331216, 1e-5));
        CHECK_THAT(spline.evaluate_clamped(-0.333), Catch::Matchers::WithinAbs(2.300653, 1e-5));
        REQUIRE_THROWS_AS(spline.evaluate(-0.333), std::out_of_range);
    }
}

TEST_CASE("CubicSpline: Boundary conditions and continuity", "[spline]") {
    SECTION("Clamped spline reproduces exact cubic polynomial") {
        const auto poly = [](double x) { return x * x * x - 2.0 * x * x + 3.0 * x - 4.0; };
        const auto d1 = [](double x) { return 3.0 * x * x - 4.0 * x + 3.0; };

        const Points pts{{-1.0, poly(-1.0)}, {0.5, poly(0.5)}, {2.0, poly(2.0)}, {4.0, poly(4.0)}};
        const CubicSpline spline(pts, clamped_bc(d1(-1.0)), clamped_bc(d1(4.0)));

        CHECK_THAT(spline.evaluate(1.25), Catch::Matchers::WithinAbs(poly(1.25), 1e-9));
        CHECK_THAT(spline.derivative(1.25, 1), Catch::Matchers::WithinAbs(d1(1.25), 1e-9));
    }

    SECTION("Free boundary makes third derivative zero at endpoints") {
        const Points pts{{0.0, 1.0}, {1.0, 4.0}, {2.5, -2.0}};
        const CubicSpline spline(pts, free_bc(), free_bc());
        CHECK_THAT(spline.derivative(0.0, 3), Catch::Matchers::WithinAbs(0.0, 1e-9));
    }
}