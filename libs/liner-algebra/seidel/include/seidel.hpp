#pragma once

#include "rectangle-matrix.hpp"
#include "vector.hpp"
#include <cstddef>

namespace linear_algebra::seidel {

bool is_diagonally_dominant(const RectangleMatrix& m);

class SimpleIterationResolver {
private:
    double eps_;
    std::size_t max_iterations_;

public:
    explicit SimpleIterationResolver(double eps = 1e-6, std::size_t max_iterations = 100000);

    Vector resolve(const RectangleMatrix& m, const Vector& b) const;
    Vector deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const;
};

class GaussSeidelResolver {
private:
    double eps_;
    std::size_t max_iterations_;

public:
    explicit GaussSeidelResolver(double eps = 1e-6, std::size_t max_iterations = 100000);

    Vector resolve(const RectangleMatrix& m, const Vector& b) const;
    Vector deviate(const RectangleMatrix& m, const Vector& x, const Vector& b) const;
};

}