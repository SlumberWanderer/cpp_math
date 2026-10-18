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

    template <std::size_t R, std::size_t C>
    RectangleMatrix(const double (&arr)[R][C])
        : rows_(R), cols_(C), data_(R * C) {
        for (std::size_t i = 0; i < R; ++i) {
            for (std::size_t j = 0; j < C; ++j) {
                data_[i * C + j] = arr[i][j];
            }
        }
    }

    std::size_t rows() const noexcept;
    std::size_t cols() const noexcept;

    double& operator()(std::size_t row, std::size_t col);
    const double& operator()(std::size_t row, std::size_t col) const;

    std::size_t index_of_max_abs_in_col(std::size_t col, std::size_t start_row = 0) const;

    void swap_rows(std::size_t row1, std::size_t row2);
    void swap_cols(std::size_t col1, std::size_t col2);

    Vector operator*(const Vector& vec) const;
    RectangleMatrix operator*(const RectangleMatrix& other) const;

    RectangleMatrix transpose() const;

    static RectangleMatrix identity(std::size_t size);

    friend std::ostream& operator<<(std::ostream& os, const RectangleMatrix& mat);
};

}