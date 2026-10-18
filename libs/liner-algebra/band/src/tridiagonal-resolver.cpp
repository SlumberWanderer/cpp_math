#include "tridiagonal-resolver.hpp"
#include "matrix-exception.hpp"
#include <cmath>
#include <limits>

namespace linear_algebra::band {

namespace {
    constexpr double kEps = std::numeric_limits<double>::epsilon();
}

Vector TridiagonalResolver::resolve(const BandMatrix& m, const Vector& d) const {
    const std::size_t n = m.size();
    if (n != d.size()) {
        throw exception::xNonEqualMatrixDimensions(n, n, d.size(), 1);
    }
    if (n == 0) {
        return Vector(0);
    }

    if (std::abs(m.b(0)) <= kEps) {
        throw exception::xImpossibleMatrixOperation();
    }

    Vector alpha(n, 0.0);
    Vector delta(n, 0.0);

    alpha[0] = -m.c(0) / m.b(0);
    delta[0] = d[0] / m.b(0);

    for (std::size_t i = 1; i < n; ++i) {
        const double denom = m.a(i) * alpha[i - 1] + m.b(i);
        if (std::abs(denom) <= kEps) {
            throw exception::xImpossibleMatrixOperation();
        }
        alpha[i] = -m.c(i) / denom;
        delta[i] = (d[i] - m.a(i) * delta[i - 1]) / denom;
    }

    Vector x(n);
    x[n - 1] = delta[n - 1];

    for (std::size_t i = n - 1; i > 0; --i) {
        const std::size_t idx = i - 1;
        x[idx] = alpha[idx] * x[idx + 1] + delta[idx];
    }

    return x;
}

Vector TridiagonalResolver::deviate(const BandMatrix& m, const Vector& x, const Vector& d) const {
    return d - (m * x);
}

}