#pragma once

#include "plane2d.hpp"

namespace interpolation::differentiation {

struct Derivatives {
    double first{0.0};
    double second{0.0};
};

class LagrangeDifferentiator {
private:
    plane2d::Points points_;

    static void validate(const plane2d::Points& points);
    static void validate(double x);
    static void validate(const Derivatives& result);

public:
    explicit LagrangeDifferentiator(plane2d::Points points);

    [[nodiscard]] Derivatives evaluate(double x) const;
    [[nodiscard]] const plane2d::Points& points() const noexcept { return points_; }
};

}