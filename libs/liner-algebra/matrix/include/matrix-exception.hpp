#pragma once

#include <exception>
#include <cstddef>
#include <string>

namespace linear_algebra::exception {

    struct xIndexError : std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Index error occurred";
        }
    };

    struct xInvalidIndex : xIndexError {
    private:
        std::size_t index_;

    public:
        explicit xInvalidIndex(std::size_t index) : index_(index) {}

        [[nodiscard]] const char* what() const noexcept override {
            return "Vector index out of range";
        }

        [[nodiscard]] std::size_t getIndex() const noexcept {
            return index_;
        }
    };

    struct xOutOfRange : xIndexError {
    private:
        std::size_t row_;
        std::size_t col_;

    public:
        xOutOfRange(std::size_t row, std::size_t col) 
            : row_(row), col_(col) {}

        [[nodiscard]] const char* what() const noexcept override {
            return "Matrix row or column index out of range";
        }

        [[nodiscard]] std::size_t getRow() const noexcept {
            return row_;
        }

        [[nodiscard]] std::size_t getCol() const noexcept {
            return col_;
        }
    };

    struct xImpossibleMatrixOperation : std::exception {
        [[nodiscard]] const char* what() const noexcept override {
            return "Impossible matrix operation";
        }
    };

    struct xNonEqualMatrixDimensions : xImpossibleMatrixOperation {
    private:
        std::size_t lhs_rows_;
        std::size_t lhs_cols_;
        std::size_t rhs_rows_;
        std::size_t rhs_cols_;

    public:
        xNonEqualMatrixDimensions(std::size_t lhs_rows, std::size_t lhs_cols,
                                  std::size_t rhs_rows, std::size_t rhs_cols)
            : lhs_rows_(lhs_rows), lhs_cols_(lhs_cols),
              rhs_rows_(rhs_rows), rhs_cols_(rhs_cols) {}

        [[nodiscard]] const char* what() const noexcept override {
            return "Matrix or vector dimensions do not match for this operation";
        }

        [[nodiscard]] std::size_t getLhsRows() const noexcept { return lhs_rows_; }
        [[nodiscard]] std::size_t getLhsCols() const noexcept { return lhs_cols_; }
        [[nodiscard]] std::size_t getRhsRows() const noexcept { return rhs_rows_; }
        [[nodiscard]] std::size_t getRhsCols() const noexcept { return rhs_cols_; }
    };

}