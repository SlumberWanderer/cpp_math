#pragma once

#include "equation-core.hpp"

namespace equation::newton {

class NewtonResolver : public core::IEquationResolver {
private:
    core::ScalarFunction derivative_;
    bool has_custom_derivative_;

public:
    NewtonResolver(core::ScalarFunction derivative, double tolerance, std::size_t max_iters = kDefaultMaxIterations) noexcept
        : IEquationResolver(tolerance, max_iters)
        , derivative_(std::move(derivative))
        , has_custom_derivative_(true) {}

    NewtonResolver(double tolerance, std::size_t max_iters) noexcept
        : IEquationResolver(tolerance, max_iters)
        , derivative_(nullptr)
        , has_custom_derivative_(false) {}

    explicit NewtonResolver(double tolerance) noexcept
        : NewtonResolver(tolerance, kDefaultMaxIterations) {}

    NewtonResolver() noexcept
        : NewtonResolver(kDefaultTolerance, kDefaultMaxIterations) {}

    ~NewtonResolver() override = default;

    [[nodiscard]] core::Result resolve(const core::ScalarFunction& func,
        const core::Range& range) const override;
};

} 