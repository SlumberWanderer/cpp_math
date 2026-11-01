#pragma once

#include "equation-core.hpp"

namespace equation::bisect {

class BisectResolver : public core::IEquationResolver {
public:
    BisectResolver(double tolerance, std::size_t max_iterations) noexcept
        : IEquationResolver(tolerance, max_iterations) {}

    explicit BisectResolver(double tolerance) noexcept
        : IEquationResolver(tolerance) {}

    BisectResolver() noexcept = default;
    ~BisectResolver() override = default;

    [[nodiscard]] core::Result resolve(const core::ScalarFunction& func,
        const core::Range& range) const override;
};

}