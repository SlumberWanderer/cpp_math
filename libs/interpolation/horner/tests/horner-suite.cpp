#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "horner.hpp"
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using polynomial::Coefficients;
using polynomial::horner::evaluate;

namespace {
struct HornerCase {
    std::string expr;
    Coefficients coeffs;
    double x;
    double expected;
};
} 

TEST_CASE("Horner scheme evaluates polynomials accurately", "[horner]") {
    const std::vector<HornerCase> cases{
        {"5", {5.0}, 3.0, 5.0},
        {"3 + 2x", {3.0, 2.0}, 4.0, 11.0},
        {"6 - 5x + x^2", {6.0, -5.0, 1.0}, 2.0, 0.0},
        {"-1 + 3x + 2x^2", {-1.0, 3.0, 2.0}, 2.0, 13.0},
        {"-4 + 3x - 2x^2 + x^3", {-4.0, 3.0, -2.0, 1.0}, 2.0, 2.0},
        {"2 - 3x^2 + x^4", {2.0, 0.0, -3.0, 0.0, 1.0}, 2.0, 6.0}
    };

    for (const auto& tc : cases) {
        DYNAMIC_SECTION("Evaluate " << tc.expr << " at x = " << tc.x) {
            CHECK_THAT(evaluate(tc.coeffs, tc.x), Catch::Matchers::WithinAbs(tc.expected, 1e-9));
        }
    }

    SECTION("Empty coefficients return 0.0") {
        CHECK_THAT(evaluate(Coefficients{}, 5.0), Catch::Matchers::WithinAbs(0.0, 1e-12));
    }

    SECTION("Non-finite coefficient or x throws") {
        REQUIRE_THROWS_AS(
            evaluate(Coefficients{1.0, std::numeric_limits<double>::infinity()}, 1.0),
            std::invalid_argument
        );
        REQUIRE_THROWS_AS(
            evaluate(Coefficients{1.0, 2.0}, std::numeric_limits<double>::quiet_NaN()),
            std::invalid_argument
        );
    }
}