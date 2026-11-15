#include "polynom.hpp"
#include "horner.hpp"
#include <cmath>
#include <iomanip>
#include <stdexcept>
#include <utility>

namespace polynomial {

Polynomial::Polynomial(Coefficients coefficients)
    : coefficients_(std::move(coefficients)) {
    if (coefficients_.empty()) {
        throw std::invalid_argument("Polynomial coefficients cannot be empty");
    }
    for (const double c : coefficients_) {
        if (!std::isfinite(c)) {
            throw std::invalid_argument("Polynomial coefficient must be finite");
        }
    }
}

Polynomial::Polynomial(const linear_algebra::Vector& coefficients) {
    if (coefficients.size() == 0) {
        throw std::invalid_argument("Polynomial coefficients cannot be empty");
    }
    coefficients_.reserve(coefficients.size());
    for (std::size_t i = 0; i < coefficients.size(); ++i) {
        if (!std::isfinite(coefficients[i])) {
            throw std::invalid_argument("Polynomial coefficient must be finite");
        }
        coefficients_.push_back(coefficients[i]);
    }
}

Polynomial::Polynomial(std::initializer_list<double> coefficients)
    : Polynomial(Coefficients(coefficients)) {}

double Polynomial::evaluate(double x) const {
    return horner::evaluate(coefficients_, x);
}

std::size_t Polynomial::degree() const noexcept {
    return coefficients_.size() - 1;
}

double Polynomial::coefficient(std::size_t index) const {
    if (index >= coefficients_.size()) {
        throw std::out_of_range("Polynomial coefficient index out of range");
    }
    return coefficients_[index];
}

std::ostream& operator<<(std::ostream& os, const Polynomial& poly) {
    const auto& c = poly.coefficients();
    for (std::size_t i = 0; i < c.size(); ++i) {
        if (i > 0) {
            os << (c[i] >= 0.0 ? " + " : " - ");
            os << std::abs(c[i]) << "*x^" << i;
        } else {
            os << c[i];
        }
    }
    return os;
}

}