#pragma once

namespace interpolation::spline::core::boundary {

struct BoundaryEquation {
    double diagonal;
    double adjacent;
    double rhs;
};

enum class BoundarySide {
    Left,
    Right
};

struct BoundaryContext {
    BoundarySide side;
    double h;
    double delta;
};

class IBoundaryEquationGenerator {
public:
    virtual ~IBoundaryEquationGenerator() = default;
    [[nodiscard]] virtual BoundaryEquation generate(const BoundaryContext& context) const = 0;
};

}