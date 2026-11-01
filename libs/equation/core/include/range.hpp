#pragma once

#include <cmath>
#include <functional>
#include <iostream>

namespace equation::core {

class Range {
private:
    double left_;
    double right_;

public:
    Range(double left, double right) noexcept
        : left_(left), right_(right) {}

    [[nodiscard]] double left() const noexcept { return left_; }
    [[nodiscard]] double right() const noexcept { return right_; }

    [[nodiscard]] double center() const noexcept {
        return left_ + (right_ - left_) / 2.0;
    }

    [[nodiscard]] double length() const noexcept {
        return right_ - left_;
    }

    [[nodiscard]] bool length_is_lq(double tolerance) const noexcept {
        return std::abs(right_ - left_) <= tolerance;
    }

    [[nodiscard]] bool is_border(double x) const noexcept {
        return x == left_ || x == right_;
    }

    [[nodiscard]] Range left_center() const noexcept {
        return Range{left_, center()};
    }

    [[nodiscard]] Range right_center() const noexcept {
        return Range{center(), right_};
    }

    [[nodiscard]] Range map(const std::function<double(double)>& mapper) const {
        return Range{mapper(left_), mapper(right_)};
    }
};

std::ostream& operator<<(std::ostream& os, const Range& range);

} 