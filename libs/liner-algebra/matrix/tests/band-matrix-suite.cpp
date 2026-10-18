#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "band-matrix.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;

TEST_CASE("BandMatrix initialization and element access", "[band-matrix]") {
    Vector a = {0.0, 1.0, 2.0};
    Vector b = {3.0, 4.0, 5.0};
    Vector c = {6.0, 7.0, 0.0};

    BandMatrix m(a, b, c);

    REQUIRE(m.size() == 3);
    REQUIRE(m(0, 0) == Catch::Approx(3.0));
    REQUIRE(m(0, 1) == Catch::Approx(6.0));
    REQUIRE(m(0, 2) == Catch::Approx(0.0)); 
    REQUIRE(m(1, 0) == Catch::Approx(1.0));
    REQUIRE(m(1, 1) == Catch::Approx(4.0));
    REQUIRE(m(1, 2) == Catch::Approx(7.0));
    REQUIRE(m(2, 1) == Catch::Approx(2.0));
    REQUIRE(m(2, 2) == Catch::Approx(5.0));
}

TEST_CASE("BandMatrix multiplication by vector", "[band-matrix][math]") {
    Vector a = {0.0, 1.0, 1.0};
    Vector b = {2.0, 2.0, 2.0};
    Vector c = {1.0, 1.0, 0.0};
    BandMatrix m(a, b, c);

    Vector x = {1.0, 2.0, 3.0};
    Vector res = m * x;

    REQUIRE(res.size() == 3);
    REQUIRE(res[0] == Catch::Approx(4.0)); 
    REQUIRE(res[1] == Catch::Approx(8.0)); 
    REQUIRE(res[2] == Catch::Approx(8.0)); 
}

TEST_CASE("BandMatrix exceptions", "[band-matrix][exceptions]") {
    Vector a = {0.0, 1.0};
    Vector b = {1.0, 2.0, 3.0};
    Vector c = {1.0, 0.0};

    REQUIRE_THROWS_AS(BandMatrix(a, b, c), exception::xImpossibleMatrixOperation);
}