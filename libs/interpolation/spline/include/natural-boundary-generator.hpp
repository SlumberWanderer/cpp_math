#pragma once

#include "boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

class NaturalBoundaryEquationGenerator final : public IBoundaryEquationGenerator {
public:
    [[nodiscard]] BoundaryEquation generate(const BoundaryContext& context) const override;
};

}