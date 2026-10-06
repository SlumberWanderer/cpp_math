#pragma once

#include <exception>
#include <cstddef>
#include "rectangle-matrix.hpp"
#include "vector.hpp"

namespace linear_algebra::linear_core {

    struct xInfinitySolution : std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "System has infinitely many solutions";
        }
    };

    struct xNoSolution : std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "System has no solutions";
        }
    };

    class ILinearSystemResolver {
    protected:
        static void validate_dimensions(const RectangleMatrix& mat, const Vector& vec);
        static void validate_matrix(const RectangleMatrix& mat);
        static void validate_finite_vector(const Vector& vec);
        static void validate_vector(const RectangleMatrix& mat, const Vector& vec);
        static void validate_at(const RectangleMatrix& mat, const Vector& vec, const std::size_t& step);
        static void reorder_by_max(RectangleMatrix& mat, Vector& vec, std::size_t step);

    public:
        ILinearSystemResolver() = default;
        virtual ~ILinearSystemResolver() = default;

        [[nodiscard]] virtual Vector resolve(const RectangleMatrix& mat, const Vector& vec) const = 0;
        [[nodiscard]] virtual Vector deviate(const RectangleMatrix& mat, const Vector& x, const Vector& b) const;
    };

    class IDeterminantResolver {
    public:
        IDeterminantResolver() = default;
        virtual ~IDeterminantResolver() = default;

        [[nodiscard]] virtual double determinant(const RectangleMatrix& mat) const = 0;
    };

    class IInverseMatrixResolver {
    public:
        IInverseMatrixResolver() = default;
        virtual ~IInverseMatrixResolver() = default;

        [[nodiscard]] virtual RectangleMatrix inverse(const RectangleMatrix& mat) const = 0;
    };

    double max_matrix_norm(const RectangleMatrix& mat);
    double max_vector_norm(const Vector& vec);

}