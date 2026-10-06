#include "partial-gauss.hpp"
#include "matrix-exception.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace linear_algebra::gauss {

namespace {
    constexpr double kEps = std::numeric_limits<double>::epsilon();
}

Vector PartialGaussResolver::resolve(const RectangleMatrix& m, const Vector& v) const {
    validate_matrix(m);
    validate_vector(m, v);

    const std::size_t size = m.rows();
    if (size == 0) {
        return Vector(0);
    }

    RectangleMatrix work(m);
    Vector rhs(v);

    for (std::size_t row = 0; row < size; ++row) {
        const std::size_t pivot_row = work.index_of_max_abs_in_col(row, row);
        if (pivot_row > row) {
            work.swap_rows(pivot_row, row);
            std::swap(rhs[pivot_row], rhs[row]);
        }

        validate_at(work, rhs, row);

        for (std::size_t next_row = row + 1; next_row < size; ++next_row) {
            const double factor = work(next_row, row) / work(row, row);
            work(next_row, row) = 0.0;

            for (std::size_t col = row + 1; col < size; ++col) {
                work(next_row, col) -= factor * work(row, col);
            }
            rhs[next_row] -= factor * rhs[row];
        }
    }

    validate_at(work, rhs, size - 1);

    Vector solution(size);
    for (std::size_t row = size; row > 0; --row) {
        double value = rhs[row - 1];
        for (std::size_t col = row; col < size; ++col) {
            value -= work(row - 1, col) * solution[col];
        }
        solution[row - 1] = value / work(row - 1, row - 1);
    }

    return solution;
}

Vector PartialGaussResolver::deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const {
    return b - (m * x);
}

double PartialGaussDeterminantResolver::determinant(const RectangleMatrix& m) const {
    if (m.rows() != m.cols()) {
        throw exception::xImpossibleMatrixOperation();
    }

    const std::size_t size = m.rows();
    if (size == 0) {
        return 0.0;
    }

    RectangleMatrix work(m);
    int sign = 1;

    for (std::size_t row = 0; row < size; ++row) {
        const std::size_t pivot_row = work.index_of_max_abs_in_col(row, row);
        if (pivot_row > row) {
            work.swap_rows(pivot_row, row);
            sign = -sign;
        }

        if (std::abs(work(row, row)) <= kEps) {
            return 0.0;
        }

        for (std::size_t next_row = row + 1; next_row < size; ++next_row) {
            const double factor = work(next_row, row) / work(row, row);
            work(next_row, row) = 0.0;

            for (std::size_t col = row + 1; col < size; ++col) {
                work(next_row, col) -= factor * work(row, col);
            }
        }
    }

    double result = 1.0;
    for (std::size_t row = 0; row < size; ++row) {
        result *= work(row, row);
    }

    return sign * result;
}

RectangleMatrix PartialGaussInverseMatrixResolver::inverse(const RectangleMatrix& m) const {
    if (m.rows() != m.cols()) {
        throw exception::xImpossibleMatrixOperation();
    }

    const std::size_t size = m.rows();
    RectangleMatrix work(m);
    RectangleMatrix identity = RectangleMatrix::identity(size);

    for (std::size_t row = 0; row < size; ++row) {
        const std::size_t pivot_row = work.index_of_max_abs_in_col(row, row);
        if (pivot_row > row) {
            work.swap_rows(pivot_row, row);
            identity.swap_rows(pivot_row, row);
        }

        if (std::abs(work(row, row)) <= kEps) {
            throw exception::xImpossibleMatrixOperation();
        }

        for (std::size_t next_row = row + 1; next_row < size; ++next_row) {
            const double factor = work(next_row, row) / work(row, row);
            work(next_row, row) = 0.0;

            for (std::size_t col = row + 1; col < size; ++col) {
                work(next_row, col) -= factor * work(row, col);
            }
            for (std::size_t col = 0; col < size; ++col) {
                identity(next_row, col) -= factor * identity(row, col);
            }
        }
    }

    for (std::size_t row = size; row > 0; --row) {
        for (std::size_t next_row = row - 1; next_row > 0; --next_row) {
            const double factor = work(next_row - 1, row - 1) / work(row - 1, row - 1);
            work(next_row - 1, row - 1) = 0.0;

            for (std::size_t col = size; col >= row; --col) {
                work(next_row - 1, col - 1) -= factor * work(row - 1, col - 1);
            }
            for (std::size_t col = size; col > 0; --col) {
                identity(next_row - 1, col - 1) -= factor * identity(row - 1, col - 1);
            }
        }
    }

    for (std::size_t row = 0; row < size; ++row) {
        const double divider = work(row, row);
        for (std::size_t col = 0; col < size; ++col) {
            identity(row, col) /= divider;
        }
    }

    return identity;
}

}