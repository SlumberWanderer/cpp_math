#pragma once

#include <vector>
#include <iostream>
#include <cstddef>
#include <initializer_list>

namespace linear_algebra {

class Vector {
private:
    std::vector<double> data_;

public:
    Vector() = default;
    explicit Vector(std::size_t size, double initial_value = 0.0);
    Vector(std::initializer_list<double> list);

    std::size_t size() const noexcept;

    double& operator[](std::size_t index);
    const double& operator[](std::size_t index) const;

    auto begin() noexcept { return data_.begin(); }
    auto end() noexcept { return data_.end(); }
    auto begin() const noexcept { return data_.begin(); }
    auto end() const noexcept { return data_.end(); }    

    Vector operator-(const Vector& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Vector& vec);
};

}