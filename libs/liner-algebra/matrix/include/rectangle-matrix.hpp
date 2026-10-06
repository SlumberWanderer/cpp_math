#pragma once

#include "vector.hpp"

namespace linear_algebra {

class RectangleMatrix {
private:
    std::size_t rows_{0};
    std::size_t cols_{0};
    std::vector<double> data_;

public:
    RectangleMatrix() = default;
    RectangleMatrix(std::size_t rows, std::size_t cols, double initial_value = 0.0);
    RectangleMatrix(std::initializer_list<std::initializer_list<double>> list);

    [[nodiscard]] std::size_t rows() const noexcept;
    [[nodiscard]] std::size_t cols() const noexcept;
    [[nodiscard]] std::size_t index_of_max_abs_in_col(std::size_t col, std::size_t start_row = 0) const;

    double& operator()(std::size_t row, std::size_t col);
    const double& operator()(std::size_t row, std::size_t col) const;

    void swap_rows(std::size_t row1, std::size_t row2);
    void swap_cols(std::size_t col1, std::size_t col2);

    Vector operator*(const Vector& vec) const;

    static RectangleMatrix identity(std::size_t size);

    template <std::size_t Rows, std::size_t Cols>
    RectangleMatrix(const double (&arr)[Rows][Cols])
        : rows_(Rows), cols_(Cols), data_(Rows * Cols) {
        for (std::size_t i = 0; i < Rows; ++i) {
            for (std::size_t j = 0; j < Cols; ++j) {
                data_[i * Cols + j] = arr[i][j];
            }
        }
    }

    RectangleMatrix operator*(const RectangleMatrix& other) const;

    friend std::ostream& operator<<(std::ostream& os, const RectangleMatrix& mat);
};
}