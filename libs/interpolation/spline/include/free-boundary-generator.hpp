#pragma once

#include "boundary-generator.hpp"

namespace interpolation::spline::core::boundary {

class FreeBoundaryEquationGenerator final : public IBoundaryEquationGenerator {
public:
    [[nodiscard]] BoundaryEquation generate(const BoundaryContext& context) const override;
};

}