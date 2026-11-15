#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "polynom.hpp"
#include <limits>
#include <stdexcept>

using namespace polynomial;

TEST_CASE("Polynomial class construction and evaluation", "[polynomial]") {
    const Polynomial poly{-1.0, 3.0, 2.0};
    CHECK(poly.degree() == 2);
    CHECK_THAT(poly.evaluate(2.0), Catch::Matchers::WithinAbs(13.0, 1e-9));
    CHECK_THAT(poly(2.0), Catch::Matchers::WithinAbs(13.0, 1e-9));
    CHECK_THAT(poly.coefficient(0), Catch::Matchers::WithinAbs(-1.0, 1e-9));
    CHECK_THAT(poly.coefficient(1), Catch::Matchers::WithinAbs(3.0, 1e-9));
    CHECK_THAT(poly.coefficient(2), Catch::Matchers::WithinAbs(2.0, 1e-9));

    SECTION("Construct from linear_algebra::Vector") {
        const linear_algebra::Vector vec{4.0, -2.0, 1.0};
        const Polynomial p_vec(vec);
        CHECK(p_vec.degree() == 2);
        CHECK_THAT(p_vec(3.0), Catch::Matchers::WithinAbs(7.0, 1e-9));
    }

    SECTION("Constant and sparse polynomials") {
        const Polynomial c_poly{-3.5};
        CHECK(c_poly.degree() == 0);
        CHECK_THAT(c_poly(-10.0), Catch::Matchers::WithinAbs(-3.5, 1e-9));

        const Polynomial sparse{1.0, 0.0, -2.0, 0.0, 1.0};
        CHECK(sparse.degree() == 4);
        CHECK_THAT(sparse(-2.0), Catch::Matchers::WithinAbs(9.0, 1e-9));
    }

    SECTION("Validation and exceptions") {
        REQUIRE_THROWS_AS(Polynomial(Coefficients{}), std::invalid_argument);
        REQUIRE_THROWS_AS(Polynomial(linear_algebra::Vector(0)), std::invalid_argument);
        REQUIRE_THROWS_AS(
            Polynomial({1.0, std::numeric_limits<double>::infinity()}),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(poly.coefficient(5), std::out_of_range);
    }
}