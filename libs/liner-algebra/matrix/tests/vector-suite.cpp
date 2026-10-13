#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "vector.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;

TEST_CASE("Vector initialization and size", "[vector]") {
    Vector v = {1.5, -2.5, 3.0};
    
    REQUIRE(v.size() == 3);
    REQUIRE(v[0] == Catch::Approx(1.5));
    REQUIRE(v[1] == Catch::Approx(-2.5));
    REQUIRE(v[2] == Catch::Approx(3.0));
}

TEST_CASE("Vector allocation and assignment", "[vector]") {
    Vector v(4);
    
    REQUIRE(v.size() == 4);
    
    v[0] = 10.0;
    v[3] = 40.0;
    
    REQUIRE(v[0] == Catch::Approx(10.0));
    REQUIRE(v[3] == Catch::Approx(40.0));
}

TEST_CASE("Vector subtraction", "[vector][math]") {
    Vector v1 = {4.0, 5.0, 6.0};
    Vector v2 = {1.0, 2.0, 3.0};

    Vector diff = v1 - v2;
    
    REQUIRE(diff.size() == 3);
    REQUIRE(diff[0] == Catch::Approx(3.0));
    REQUIRE(diff[1] == Catch::Approx(3.0));
    REQUIRE(diff[2] == Catch::Approx(3.0));
}

TEST_CASE("Vector exceptions", "[vector][exceptions]") {
    Vector v = {1.0, 2.0};
    
    REQUIRE_THROWS_AS(v[5], exception::xInvalidIndex);

    Vector v_large = {1.0, 2.0, 3.0};
    REQUIRE_THROWS_AS(v - v_large, exception::xNonEqualMatrixDimensions);
}