#include "rectangle-matrix.hpp"
#include "matrix-exception.hpp"
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cmath>

namespace linear_algebra {

RectangleMatrix::RectangleMatrix(std::size_t rows, std::size_t cols, double initial_value)
    : rows_(rows), cols_(cols), data_(rows * cols, initial_value) {}

RectangleMatrix::RectangleMatrix(std::initializer_list<std::initializer_list<double>> list) {
    rows_ = list.size();
    cols_ = rows_ > 0 ? list.begin()->size() : 0;
    data_.reserve(rows_ * cols_);

    for (const auto& row_list : list) {
        if (row_list.size() != cols_) {
            throw exception::xImpossibleMatrixOperation();
        }
        for (const double val : row_list) {
            data_.push_back(val);
        }
    }
}

std::size_t RectangleMatrix::rows() const noexcept {
    return rows_;
}

std::size_t RectangleMatrix::cols() const noexcept {
    return cols_;
}

double& RectangleMatrix::operator()(std::size_t row, std::size_t col) {
    if (row >= rows_ || col >= cols_) {
        throw exception::xOutOfRange(row, col);
    }
    return data_[row * cols_ + col];
}

const double& RectangleMatrix::operator()(std::size_t row, std::size_t col) const {
    if (row >= rows_ || col >= cols_) {
        throw exception::xOutOfRange(row, col);
    }
    return data_[row * cols_ + col];
}

std::size_t RectangleMatrix::index_of_max_abs_in_col(std::size_t col, std::size_t start_row) const {
    if (col >= cols_ || start_row >= rows_) {
        throw exception::xOutOfRange(start_row, col);
    }

    std::size_t max_row = start_row;
    double max_val = std::abs(data_[start_row * cols_ + col]);

    for (std::size_t r = start_row + 1; r < rows_; ++r) {
        const double current_val = std::abs(data_[r * cols_ + col]);
        if (current_val > max_val) {
            max_val = current_val;
            max_row = r;
        }
    }

    return max_row;
}

void RectangleMatrix::swap_rows(std::size_t row1, std::size_t row2) {
    if (row1 >= rows_ || row2 >= rows_) {
        throw exception::xOutOfRange(row1 >= rows_ ? row1 : row2, 0);
    }
    if (row1 == row2) {
        return;
    }

    auto start1 = data_.begin() + static_cast<std::ptrdiff_t>(row1 * cols_);
    auto start2 = data_.begin() + static_cast<std::ptrdiff_t>(row2 * cols_);
    std::swap_ranges(start1, start1 + static_cast<std::ptrdiff_t>(cols_), start2);
}

void RectangleMatrix::swap_cols(std::size_t col1, std::size_t col2) {
    if (col1 >= cols_ || col2 >= cols_) {
        throw exception::xOutOfRange(0, col1 >= cols_ ? col1 : col2);
    }
    if (col1 == col2) {
        return;
    }
    
    for (std::size_t r = 0; r < rows_; ++r) {
        const std::size_t offset = r * cols_;
        std::swap(data_[offset + col1], data_[offset + col2]);
    }
}

Vector RectangleMatrix::operator*(const Vector& vec) const {
    if (cols_ != vec.size()) {
        throw exception::xNonEqualMatrixDimensions(rows_, cols_, vec.size(), 1);
    }

    Vector result(rows_, 0.0);
    for (std::size_t i = 0; i < rows_; ++i) {
        double sum = 0.0;
        const std::size_t row_offset = i * cols_;
        for (std::size_t j = 0; j < cols_; ++j) {
            sum += data_[row_offset + j] * vec[j];
        }
        result[i] = sum;
    }
    return result;
}

RectangleMatrix RectangleMatrix::operator*(const RectangleMatrix& other) const {
    if (cols_ != other.rows_) {
        throw exception::xNonEqualMatrixDimensions(rows_, cols_, other.rows_, other.cols_);
    }

    RectangleMatrix result(rows_, other.cols_, 0.0);
    for (std::size_t i = 0; i < rows_; ++i) {
        for (std::size_t k = 0; k < cols_; ++k) {
            const double factor = data_[i * cols_ + k];
            for (std::size_t j = 0; j < other.cols_; ++j) {
                result.data_[i * other.cols_ + j] += factor * other.data_[k * other.cols_ + j];
            }
        }
    }
    return result;
}

RectangleMatrix RectangleMatrix::identity(std::size_t size) {
    RectangleMatrix id(size, size, 0.0);
    for (std::size_t i = 0; i < size; ++i) {
        id(i, i) = 1.0;
    }
    return id;
}

std::ostream& operator<<(std::ostream& os, const RectangleMatrix& mat) {
    std::ostringstream oss;
    for (std::size_t i = 0; i < mat.rows_; ++i) {
        oss << "[ ";
        const std::size_t row_offset = i * mat.cols_;
        for (std::size_t j = 0; j < mat.cols_; ++j) {
            oss << std::setw(10) << std::setprecision(5) << mat.data_[row_offset + j] << " ";
        }
        oss << "]\n";
    }
    return os << oss.str();
}

}