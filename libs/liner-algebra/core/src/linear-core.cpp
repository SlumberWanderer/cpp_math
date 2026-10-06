#include "linear-core.hpp"
#include "matrix-exception.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace linear_algebra::linear_core {

namespace {
    constexpr double kEps = std::numeric_limits<double>::epsilon();
}

void ILinearSystemResolver::validate_dimensions(const RectangleMatrix& mat, const Vector& vec) {
    if (mat.rows() != vec.size()) {
        throw exception::xNonEqualMatrixDimensions(mat.rows(), mat.cols(), vec.size(), 1);
    }
}

void ILinearSystemResolver::validate_matrix(const RectangleMatrix& mat) {
    if (mat.rows() != mat.cols()) {
        throw exception::xImpossibleMatrixOperation();
    }
    for (std::size_t i = 0; i < mat.rows(); ++i) {
        for (std::size_t j = 0; j < mat.cols(); ++j) {
            if (!std::isfinite(mat(i, j))) {
                throw exception::xImpossibleMatrixOperation();
            }
        }
    }
}

void ILinearSystemResolver::validate_finite_vector(const Vector& vec) {
    for (const double val : vec) {
        if (!std::isfinite(val)) {
            throw exception::xImpossibleMatrixOperation();
        }
    }
}

void ILinearSystemResolver::validate_vector(const RectangleMatrix& mat, const Vector& vec) {
    validate_dimensions(mat, vec);
    validate_finite_vector(vec);
}

void ILinearSystemResolver::validate_at(const RectangleMatrix& mat, const Vector& vec, const std::size_t& step) {
    if (std::abs(mat(step, step)) <= kEps) {
        if (std::abs(vec[step]) > kEps) {
            throw xNoSolution();
        } else {
            throw xInfinitySolution();
        }
    }
}

void ILinearSystemResolver::reorder_by_max(RectangleMatrix& mat, Vector& vec, std::size_t step) {
    const std::size_t pivot_row = mat.index_of_max_abs_in_col(step, step);
    if (pivot_row != step) {
        mat.swap_rows(step, pivot_row);
        std::swap(vec[step], vec[pivot_row]);
    }
}

Vector ILinearSystemResolver::deviate(const RectangleMatrix& mat, const Vector& x, const Vector& b) const {
    return b - (mat * x);
}

double max_matrix_norm(const RectangleMatrix& mat) {
    double max_sum = 0.0;
    for (std::size_t i = 0; i < mat.rows(); ++i) {
        double current_sum = 0.0;
        for (std::size_t j = 0; j < mat.cols(); ++j) {
            current_sum += std::abs(mat(i, j));
        }
        max_sum = std::max(max_sum, current_sum);
    }
    return max_sum;
}

double max_vector_norm(const Vector& vec) {
    double max_val = 0.0;
    for (const double val : vec) {
        max_val = std::max(max_val, std::abs(val));
    }
    return max_val;
}

}