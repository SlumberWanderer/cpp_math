#pragma once

#include "boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

class ClampBoundaryEquationGenerator final : public IBoundaryEquationGenerator {
private:
    double slope_;

public:
    explicit ClampBoundaryEquationGenerator(double slope);
    [[nodiscard]] BoundaryEquation generate(const BoundaryContext& context) const override;
};

}