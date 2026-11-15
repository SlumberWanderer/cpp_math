#include <catch2/catch_test_macros.hpp>
#include "plotter.hpp"
#include <limits>
#include <stdexcept>

using namespace plot2d;
using plane2d::Points;

TEST_CASE("plot2d: Plot builder and validation", "[plot2d]") {
    Plot plot("Test Plot");
    plot.set_labels("X axis", "Y axis")
        .set_size(1280, 720)
        .add_line({{0.0, 0.0}, {1.0, 1.0}}, "Linear", 2)
        .add_points({{0.0, 0.0}, {1.0, 1.0}}, "Nodes");

    CHECK(plot.title() == "Test Plot");
    CHECK(plot.curves().size() == 2);
    CHECK(plot.width() == 1280);
    CHECK(plot.height() == 720);
    REQUIRE_NOTHROW(Plotter::validate(plot));

    SECTION("Empty plot throws") {
        Plot empty_plot;
        REQUIRE_THROWS_AS(Plotter::validate(empty_plot), std::invalid_argument);
    }

    SECTION("Empty curve points throw") {
        Plot bad_plot;
        bad_plot.add_line({}, "Empty");
        REQUIRE_THROWS_AS(Plotter::validate(bad_plot), std::invalid_argument);
    }

    SECTION("Non-finite point throws") {
        Plot inf_plot;
        inf_plot.add_points({{0.0, std::numeric_limits<double>::quiet_NaN()}}, "NaN");
        REQUIRE_THROWS_AS(Plotter::validate(inf_plot), std::invalid_argument);
    }
}