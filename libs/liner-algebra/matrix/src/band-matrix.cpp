#include "band-matrix.hpp"
#include "matrix-exception.hpp"
#include <iomanip>
#include <sstream>

namespace linear_algebra {

BandMatrix::BandMatrix(const Vector& a, const Vector& b, const Vector& c)
    : size_(b.size()), a_(a), b_(b), c_(c) {
    if (a.size() != size_ || c.size() != size_) {
        throw exception::xImpossibleMatrixOperation();
    }
    if (size_ > 0) {
        a_[0] = 0.0;
        c_[size_ - 1] = 0.0;
    }
}

std::size_t BandMatrix::size() const noexcept {
    return size_;
}

std::size_t BandMatrix::rows() const noexcept {
    return size_;
}

std::size_t BandMatrix::cols() const noexcept {
    return size_;
}

double BandMatrix::a(std::size_t index) const {
    return a_[index];
}

double BandMatrix::b(std::size_t index) const {
    return b_[index];
}

double BandMatrix::c(std::size_t index) const {
    return c_[index];
}

double BandMatrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= size_ || col >= size_) {
        throw exception::xOutOfRange(row, col);
    }
    if (row == col) {
        return b_[row];
    }
    if (row == col + 1) {
        return a_[row];
    }
    if (row + 1 == col) {
        return c_[row];
    }
    return 0.0;
}

Vector BandMatrix::operator*(const Vector& vec) const {
    if (size_ != vec.size()) {
        throw exception::xNonEqualMatrixDimensions(size_, size_, vec.size(), 1);
    }
    if (size_ == 0) {
        return Vector(0);
    }

    Vector result(size_, 0.0);
    for (std::size_t i = 0; i < size_; ++i) {
        double sum = b_[i] * vec[i];
        if (i > 0) {
            sum += a_[i] * vec[i - 1];
        }
        if (i + 1 < size_) {
            sum += c_[i] * vec[i + 1];
        }
        result[i] = sum;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const BandMatrix& mat) {
    std::ostringstream oss;
    oss << std::setprecision(5);
    for (std::size_t i = 0; i < mat.size_; ++i) {
        oss << "[ ";
        for (std::size_t j = 0; j < mat.size_; ++j) {
            oss << std::setw(10) << mat(i, j) << " ";
        }
        oss << "]\n";
    }
    return os << oss.str();
}

}