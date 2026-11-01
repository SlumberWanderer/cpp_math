#pragma once

#include "equation-core.hpp"

namespace equation::chord {

class ChordResolver : public core::IEquationResolver {
public:
    ChordResolver(double tolerance, std::size_t max_iterations) noexcept
        : IEquationResolver(tolerance, max_iterations) {}

    explicit ChordResolver(double tolerance) noexcept
        : IEquationResolver(tolerance) {}

    ChordResolver() noexcept = default;
    ~ChordResolver() override = default;

    [[nodiscard]] core::Result resolve(const core::ScalarFunction& func,
        const core::Range& range) const override;
};

}