#include "seidel.hpp"
#include "matrix-exception.hpp"
#include <cmath>
#include <limits>
#include <algorithm>

namespace linear_algebra::seidel {

namespace {
    constexpr double kEps = std::numeric_limits<double>::epsilon();

    double max_diff(const Vector& a, const Vector& b) {
        double max_val = 0.0;
        for (std::size_t i = 0; i < a.size(); ++i) {
            max_val = std::max(max_val, std::abs(a[i] - b[i]));
        }
        return max_val;
    }
}

bool is_diagonally_dominant(const RectangleMatrix& m) {
    if (m.rows() != m.cols() || m.rows() == 0) {
        return false;
    }
    const std::size_t n = m.rows();
    for (std::size_t i = 0; i < n; ++i) {
        double off_diag_sum = 0.0;
        for (std::size_t j = 0; j < n; ++j) {
            if (i != j) {
                off_diag_sum += std::abs(m(i, j));
            }
        }
        if (std::abs(m(i, i)) <= off_diag_sum) {
            return false;
        }
    }
    return true;
}


SimpleIterationResolver::SimpleIterationResolver(double eps, std::size_t max_iterations)
    : eps_(eps), max_iterations_(max_iterations) {}

Vector SimpleIterationResolver::resolve(const RectangleMatrix& m, const Vector& b) const {
    if (m.rows() != m.cols()) {
        throw exception::xImpossibleMatrixOperation();
    }
    const std::size_t n = m.rows();
    if (n != b.size()) {
        throw exception::xNonEqualMatrixDimensions(n, n, b.size(), 1);
    }
    if (n == 0) {
        return Vector(0);
    }

    RectangleMatrix work_m = m;
    Vector work_b = b;

    if (!is_diagonally_dominant(work_m)) {
        const RectangleMatrix at = m.transpose();
        work_m = at * m;
        work_b = at * b;
    }

    for (std::size_t i = 0; i < n; ++i) {
        if (std::abs(work_m(i, i)) <= kEps) {
            throw exception::xImpossibleMatrixOperation();
        }
    }

    Vector x(n, 0.0); 
    Vector x_next(n, 0.0);

    for (std::size_t iter = 0; iter < max_iterations_; ++iter) {
        for (std::size_t i = 0; i < n; ++i) {
            double sum = work_b[i];
            double row_abs_sum = 0.0;
            for (std::size_t j = 0; j < n; ++j) {
                row_abs_sum += std::abs(work_m(i, j));
                if (i != j) {
                    sum -= work_m(i, j) * x[j];
                }
            }

            const double diag = work_m(i, i);
            if (std::abs(diag) > row_abs_sum - std::abs(diag)) {
                x_next[i] = sum / diag;
            } else {
                const double residual = sum - diag * x[i];
                const double denom = (diag >= 0.0) ? row_abs_sum : -row_abs_sum;
                x_next[i] = x[i] + residual / denom;
            }
        }

        if (max_diff(x_next, x) < eps_) {
            return x_next;
        }
        x = x_next;
    }

    return x;
}

Vector SimpleIterationResolver::deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const {
    return b - (m * x);
}

GaussSeidelResolver::GaussSeidelResolver(double eps, std::size_t max_iterations)
    : eps_(eps), max_iterations_(max_iterations) {}

Vector GaussSeidelResolver::resolve(const RectangleMatrix& m, const Vector& b) const {
    if (m.rows() != m.cols()) {
        throw exception::xImpossibleMatrixOperation();
    }
    const std::size_t n = m.rows();
    if (n != b.size()) {
        throw exception::xNonEqualMatrixDimensions(n, n, b.size(), 1);
    }
    if (n == 0) {
        return Vector(0);
    }

    RectangleMatrix work_m = m;
    Vector work_b = b;

    if (!is_diagonally_dominant(work_m)) {
        const RectangleMatrix at = m.transpose();
        work_m = at * m;
        work_b = at * b;
    }

    for (std::size_t i = 0; i < n; ++i) {
        if (std::abs(work_m(i, i)) <= kEps) {
            throw exception::xImpossibleMatrixOperation();
        }
    }

    Vector x(n, 0.0); 

    for (std::size_t iter = 0; iter < max_iterations_; ++iter) {
        Vector x_prev = x;

        for (std::size_t i = 0; i < n; ++i) {
            double sum = work_b[i];
            for (std::size_t j = 0; j < n; ++j) {
                if (i != j) {
                    sum -= work_m(i, j) * x[j];
                }
            }
            x[i] = sum / work_m(i, i);
        }

        if (max_diff(x, x_prev) < eps_) {
            return x;
        }
    }

    return x;
}

Vector GaussSeidelResolver::deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const {
    return b - (m * x);
}

}