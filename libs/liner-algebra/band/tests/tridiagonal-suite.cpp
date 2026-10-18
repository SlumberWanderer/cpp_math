#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "tridiagonal-resolver.hpp"
#include "band-matrix.hpp"
#include "vector.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;
using namespace linear_algebra::band;

TEST_CASE("TridiagonalResolver solves simple 3x3 system", "[band][tridiagonal]") {
    Vector a = {0.0, 1.0, 1.0};
    Vector b = {2.0, 3.0, 2.0};
    Vector c = {1.0, 1.0, 0.0};
    Vector d = {4.0, 10.0, 8.0};

    BandMatrix m(a, b, c);
    TridiagonalResolver resolver;

    Vector x = resolver.resolve(m, d);

    REQUIRE(x.size() == 3);
    REQUIRE(x[0] == Catch::Approx(1.0));
    REQUIRE(x[1] == Catch::Approx(2.0));
    REQUIRE(x[2] == Catch::Approx(3.0));

    Vector dev = resolver.deviate(m, x, d);
    REQUIRE(dev[0] == Catch::Approx(0.0).margin(1e-9));
    REQUIRE(dev[1] == Catch::Approx(0.0).margin(1e-9));
    REQUIRE(dev[2] == Catch::Approx(0.0).margin(1e-9));
}

TEST_CASE("TridiagonalResolver solves Variant 1 system", "[band][variant1]") {
    Vector a = {  0.0,    -14.969,  21.05,  -35.269, -49.108 };
    Vector b = { -30.668,  32.284, -19.601,  48.851,   3.166 };
    Vector c = {  8.501,  -32.587, -40.859, -38.092,   0.0   };
    Vector d = { -3.793,   36.222,  27.966,  49.68,   11.149 };

    BandMatrix m(a, b, c);
    TridiagonalResolver resolver;

    Vector x = resolver.resolve(m, d);
    Vector dev = resolver.deviate(m, x, d);

    REQUIRE(x.size() == 5);
    for (std::size_t i = 0; i < 5; ++i) {
        REQUIRE(dev[i] == Catch::Approx(0.0).margin(1e-9));
    }
}

TEST_CASE("TridiagonalResolver exception handling", "[band][exceptions]") {
    TridiagonalResolver resolver;

    SECTION("Dimension mismatch") {
        BandMatrix m({0.0, 1.0}, {2.0, 2.0}, {1.0, 0.0});
        Vector d = {1.0, 2.0, 3.0};
        REQUIRE_THROWS_AS(resolver.resolve(m, d), exception::xNonEqualMatrixDimensions);
    }

    SECTION("Zero on main diagonal") {
        BandMatrix m({0.0, 1.0}, {0.0, 2.0}, {1.0, 0.0});
        Vector d = {1.0, 2.0};
        REQUIRE_THROWS_AS(resolver.resolve(m, d), exception::xImpossibleMatrixOperation);
    }
}