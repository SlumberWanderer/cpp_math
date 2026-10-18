#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "seidel.hpp"
#include "rectangle-matrix.hpp"
#include "vector.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;
using namespace linear_algebra::seidel;

TEST_CASE("Iterative resolvers solve Lecture Example 2.3 (diagonally dominant)", "[seidel][lecture]") {
    RectangleMatrix m = {
        { 22.10,   5.40,  -2.50,   1.70 },
        { -2.60, -14.30,   2.10,   3.10 },
        {  0.30,  -0.81,  11.60,   1.50 },
        { -0.70,   2.10,   3.20,  18.20 }
    };
    Vector b = { -11.20, 6.10, 4.80, 2.30 };

    REQUIRE(is_diagonally_dominant(m) == true);

    SimpleIterationResolver simple_resolver(1e-4);
    GaussSeidelResolver seidel_resolver(1e-4);

    Vector x_simple = simple_resolver.resolve(m, b);
    Vector x_seidel = seidel_resolver.resolve(m, b);

    REQUIRE(x_simple[0] == Catch::Approx(-0.3993).margin(1e-3));
    REQUIRE(x_simple[1] == Catch::Approx(-0.2799).margin(1e-3));
    REQUIRE(x_simple[2] == Catch::Approx( 0.3950).margin(1e-3));
    REQUIRE(x_simple[3] == Catch::Approx( 0.0739).margin(1e-3));

    REQUIRE(x_seidel[0] == Catch::Approx(-0.3993).margin(1e-3));
    REQUIRE(x_seidel[1] == Catch::Approx(-0.2799).margin(1e-3));
    REQUIRE(x_seidel[2] == Catch::Approx( 0.3950).margin(1e-3));
    REQUIRE(x_seidel[3] == Catch::Approx( 0.0739).margin(1e-3));
}

TEST_CASE("Iterative resolvers solve Variant 1 system via normal form A^T * A", "[seidel][variant1]") {
    RectangleMatrix m = {
        {  -6.4487, -49.28,    10.1556,   7.6665 },
        { -27.7668, -49.5715, -41.7735,  35.5074 },
        {  31.1055,  19.1492,   3.0142, -35.7218 },
        {  22.7898, -46.8907,  14.4522, -41.4534 }
    };
    Vector b = { 44.7878, -22.5032, 48.1769, -20.7736 };

    REQUIRE(is_diagonally_dominant(m) == false);

    SimpleIterationResolver simple_resolver(1e-7, 200000);
    GaussSeidelResolver seidel_resolver(1e-7, 200000);

    Vector x_simple = simple_resolver.resolve(m, b);
    Vector x_seidel = seidel_resolver.resolve(m, b);

    REQUIRE(x_seidel[0] == Catch::Approx(11.3857).margin(1e-3));
    REQUIRE(x_seidel[1] == Catch::Approx(-0.924342).margin(1e-3));
    REQUIRE(x_seidel[2] == Catch::Approx(0.99878).margin(1e-3));
    REQUIRE(x_seidel[3] == Catch::Approx(8.15443).margin(1e-3));

    REQUIRE(x_simple[0] == Catch::Approx(11.3857).margin(1e-2));
    REQUIRE(x_simple[1] == Catch::Approx(-0.924342).margin(1e-2));
    REQUIRE(x_simple[2] == Catch::Approx(0.99878).margin(1e-2));
    REQUIRE(x_simple[3] == Catch::Approx(8.15443).margin(1e-2));
}