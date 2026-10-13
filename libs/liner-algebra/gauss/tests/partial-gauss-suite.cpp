#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <cmath>

#include "partial-gauss.hpp"
#include "rectangle-matrix.hpp"
#include "vector.hpp"
#include "matrix-exception.hpp"

using namespace linear_algebra;
using namespace linear_algebra::gauss;

TEST_CASE("PartialGaussResolver solves linear systems", "[gauss][resolver]") {
    PartialGaussResolver resolver;

    SECTION("System with a unique solution") {
        RectangleMatrix A = {
            { 2.0,  1.0, -1.0},
            {-3.0, -1.0,  2.0},
            {-2.0,  1.0,  2.0}
        };
        Vector b = {8.0, -11.0, -3.0};

        Vector solution = resolver.resolve(A, b);

        REQUIRE(solution.size() == 3);
        REQUIRE(solution[0] == Catch::Approx(2.0));
        REQUIRE(solution[1] == Catch::Approx(3.0));
        REQUIRE(solution[2] == Catch::Approx(-1.0));

        Vector deviate = resolver.deviate(A, solution, b);
        REQUIRE(deviate.size() == 3);
        REQUIRE(deviate[0] == Catch::Approx(0.0).margin(1e-9));
        REQUIRE(deviate[1] == Catch::Approx(0.0).margin(1e-9));
        REQUIRE(deviate[2] == Catch::Approx(0.0).margin(1e-9));
    }
}

TEST_CASE("PartialGaussDeterminantResolver calculates determinant", "[gauss][determinant]") {
    PartialGaussDeterminantResolver det_resolver;

    SECTION("Non-singular matrix") {
        RectangleMatrix A = {
            {4.0, 3.0},
            {6.0, 3.0}
        };
        REQUIRE(det_resolver.determinant(A) == Catch::Approx(-6.0));
    }

    SECTION("Singular matrix (determinant is zero)") {
        RectangleMatrix A = {
            {1.0, 2.0},
            {2.0, 4.0} 
        };
        REQUIRE(det_resolver.determinant(A) == Catch::Approx(0.0).margin(1e-9));
    }

    SECTION("Exceptions for invalid dimensions") {
        RectangleMatrix A(2, 3); 
        REQUIRE_THROWS_AS(det_resolver.determinant(A), exception::xImpossibleMatrixOperation);
    }
}

TEST_CASE("PartialGaussInverseMatrixResolver calculates inverse", "[gauss][inverse]") {
    PartialGaussInverseMatrixResolver inv_resolver;

    SECTION("Valid inverse matrix") {
        RectangleMatrix A = {
            {4.0, 7.0},
            {2.0, 6.0}
        };
        
        RectangleMatrix inv = inv_resolver.inverse(A);
        RectangleMatrix identity = A * inv; 
        
        REQUIRE(identity.rows() == 2);
        REQUIRE(identity.cols() == 2);
        
        REQUIRE(identity(0, 0) == Catch::Approx(1.0));
        REQUIRE(identity(1, 1) == Catch::Approx(1.0));
        
        REQUIRE(identity(0, 1) == Catch::Approx(0.0).margin(1e-9));
        REQUIRE(identity(1, 0) == Catch::Approx(0.0).margin(1e-9));
    }

    SECTION("Exceptions for singular matrices") {
        RectangleMatrix A = {
            {1.0, 2.0},
            {2.0, 4.0}
        };
        REQUIRE_THROWS_AS(inv_resolver.inverse(A), exception::xImpossibleMatrixOperation);
    }

    SECTION("Exceptions for invalid dimensions") {
        RectangleMatrix A(3, 2); 
        REQUIRE_THROWS_AS(inv_resolver.inverse(A), exception::xImpossibleMatrixOperation);
    }
}