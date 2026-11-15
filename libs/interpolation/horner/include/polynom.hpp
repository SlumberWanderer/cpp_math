#pragma once

#include <cstddef>
#include <initializer_list>
#include <iostream>
#include "polynom-core.hpp"
#include "vector.hpp"

namespace polynomial {

class Polynomial {
private:
    Coefficients coefficients_;

public:
    explicit Polynomial(Coefficients coefficients);
    explicit Polynomial(const linear_algebra::Vector& coefficients);
    Polynomial(std::initializer_list<double> coefficients);

    [[nodiscard]] double evaluate(double x) const;
    [[nodiscard]] double operator()(double x) const { return evaluate(x); }

    [[nodiscard]] std::size_t degree() const noexcept;
    [[nodiscard]] const Coefficients& coefficients() const noexcept { return coefficients_; }
    [[nodiscard]] double coefficient(std::size_t index) const;
};

std::ostream& operator<<(std::ostream& os, const Polynomial& poly);

}