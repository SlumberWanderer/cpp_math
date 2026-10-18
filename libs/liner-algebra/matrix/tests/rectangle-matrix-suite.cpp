#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "rectangle-matrix.hpp"
#include "vector.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;

TEST_CASE("RectangleMatrix initialization and access", "[matrix]") {
    RectangleMatrix m = {
        {1.0, 2.0},
        {3.0, 4.0}
    };

    REQUIRE(m.rows() == 2);
    REQUIRE(m.cols() == 2);
    REQUIRE(m(0, 0) == Catch::Approx(1.0));
    REQUIRE(m(1, 1) == Catch::Approx(4.0));
}

TEST_CASE("Matrix by vector multiplication", "[matrix][math]") {
    RectangleMatrix m = {
        {1.0, 2.0},
        {3.0, 4.0}
    };
    Vector v = {2.0, 1.0};
    
    Vector result = m * v;
    
    REQUIRE(result.size() == 2);
    REQUIRE(result[0] == Catch::Approx(4.0));  
    REQUIRE(result[1] == Catch::Approx(10.0)); 
}

TEST_CASE("Matrix dimensions exception handling", "[matrix][exceptions]") {
    RectangleMatrix m(2, 2);
    Vector v(3); 
    
    REQUIRE_THROWS_AS(m * v, exception::xNonEqualMatrixDimensions);
}

TEST_CASE("RectangleMatrix transpose", "[rectangle-matrix][transpose]") {
    RectangleMatrix m = {
        {1.0, 2.0, 3.0},
        {4.0, 5.0, 6.0}
    };

    RectangleMatrix t = m.transpose();

    REQUIRE(t.rows() == 3);
    REQUIRE(t.cols() == 2);
    REQUIRE(t(0, 0) == Catch::Approx(1.0));
    REQUIRE(t(0, 1) == Catch::Approx(4.0));
    REQUIRE(t(1, 0) == Catch::Approx(2.0));
    REQUIRE(t(1, 1) == Catch::Approx(5.0));
    REQUIRE(t(2, 0) == Catch::Approx(3.0));
    REQUIRE(t(2, 1) == Catch::Approx(6.0));
}