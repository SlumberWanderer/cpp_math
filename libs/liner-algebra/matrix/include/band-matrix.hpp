#pragma once

#include "vector.hpp"
#include <iostream>
#include <cstddef>

namespace linear_algebra {

class BandMatrix {
private:
    std::size_t size_{0};
    Vector a_; 
    Vector b_; 
    Vector c_; 

public:
    BandMatrix() = default;
    BandMatrix(const Vector& a, const Vector& b, const Vector& c);

    std::size_t size() const noexcept;
    std::size_t rows() const noexcept;
    std::size_t cols() const noexcept;

    double a(std::size_t index) const;
    double b(std::size_t index) const;
    double c(std::size_t index) const;

    double operator()(std::size_t row, std::size_t col) const;

    Vector operator*(const Vector& vec) const;

    friend std::ostream& operator<<(std::ostream& os, const BandMatrix& mat);
};

}