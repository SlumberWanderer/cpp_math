#pragma once

#include "equation-core.hpp"

namespace equation::secant {

class SecantResolver : public core::IEquationResolver {
public:
    explicit SecantResolver(double tolerance = kDefaultTolerance, 
                            std::size_t max_iters = kDefaultMaxIterations) noexcept
        : IEquationResolver(tolerance, max_iters) {}

    ~SecantResolver() override = default;

    [[nodiscard]] core::Result resolve(const core::ScalarFunction& func,
        const core::Range& range) const override;
};

}