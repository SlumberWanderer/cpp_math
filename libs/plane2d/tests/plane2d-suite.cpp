#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "plane2d.hpp"
#include <limits>
#include <stdexcept>

using namespace plane2d;

TEST_CASE("plane2d: validation and boundaries", "[plane2d]") {
    const Points pts{{2.0, 4.0}, {-1.0, 1.0}, {3.0, 9.0}};
    REQUIRE_NOTHROW(validate(pts));
    CHECK_THAT(left_point(pts).x, Catch::Matchers::WithinAbs(-1.0, 1e-9));
    CHECK_THAT(right_point(pts).x, Catch::Matchers::WithinAbs(3.0, 1e-9));

    SECTION("Duplicate x throws") {
        const Points dup{{1.0, 2.0}, {1.0, 5.0}};
        REQUIRE_THROWS_AS(validate(dup), std::invalid_argument);
    }

    SECTION("Non-finite coordinate throws") {
        const Points inf{{0.0, 1.0}, {1.0, std::numeric_limits<double>::infinity()}};
        REQUIRE_THROWS_AS(validate(inf), std::invalid_argument);
    }
}

TEST_CASE("plane2d: tabulate and tabulate_by_step", "[plane2d]") {
    const auto sq = [](double x) { return x * x; };

    const auto by_count = tabulate(sq, 0.0, 2.0, 5);
    REQUIRE(by_count.size() == 5);
    CHECK_THAT(by_count.front().x, Catch::Matchers::WithinAbs(0.0, 1e-9));
    CHECK_THAT(by_count.back().x, Catch::Matchers::WithinAbs(2.0, 1e-9));

    const auto by_step = tabulate_by_step(sq, 0.0, 2.0, 0.5);
    REQUIRE(by_step.size() == 5);
    CHECK_THAT(by_step[2].y, Catch::Matchers::WithinAbs(1.0, 1e-9));
}