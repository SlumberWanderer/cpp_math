#pragma once

#include "band-matrix.hpp"
#include "vector.hpp"

namespace linear_algebra::band {

class TridiagonalResolver {
public:
    TridiagonalResolver() = default;

    Vector resolve(const BandMatrix& m, const Vector& d) const;

    Vector deviate(const BandMatrix& m, const Vector& x, const Vector& d) const;
};

}