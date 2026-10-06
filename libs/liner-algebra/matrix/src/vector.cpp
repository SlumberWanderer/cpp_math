#include "vector.hpp"
#include "matrix-exception.hpp"
#include <iomanip>
#include <sstream>

namespace linear_algebra {

Vector::Vector(std::size_t size, double initial_value)
    : data_(size, initial_value) {}

Vector::Vector(std::initializer_list<double> list)
    : data_(list) {}

std::size_t Vector::size() const noexcept {
    return data_.size();
}

double& Vector::operator[](std::size_t index) {
    if (index >= data_.size()) {
        throw exception::xInvalidIndex(index);
    }
    return data_[index];
}

const double& Vector::operator[](std::size_t index) const {
    if (index >= data_.size()) {
        throw exception::xInvalidIndex(index);
    }
    return data_[index];
}

Vector Vector::operator-(const Vector& other) const {
    if (data_.size() != other.data_.size()) {
        throw exception::xNonEqualMatrixDimensions(data_.size(), 1, other.data_.size(), 1);
    }
    Vector result(data_.size());
    for (std::size_t i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] - other.data_[i];
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Vector& vec) {
    std::ostringstream oss;
    oss << "[ ";
    for (const double val : vec.data_) {
        oss << std::setw(10) << std::setprecision(5) << val << " ";
    }
    oss << "]";
    return os << oss.str();
}

}