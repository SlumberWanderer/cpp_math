#pragma once

#include "equation-core.hpp"

namespace equation::iterative {

class IterativeResolver : public core::IEquationResolver {
private:
    core::ScalarFunction phi_;
    double factor_;
    bool use_custom_phi_;

public:
    IterativeResolver(core::ScalarFunction phi, double tolerance, std::size_t max_iterations = kDefaultMaxIterations) noexcept
        : IEquationResolver(tolerance, max_iterations)
        , phi_(std::move(phi))
        , factor_(0.0)
        , use_custom_phi_(true) {}

    IterativeResolver(double factor, double tolerance, std::size_t max_iterations = kDefaultMaxIterations) noexcept
        : IEquationResolver(tolerance, max_iterations)
        , phi_(nullptr)
        , factor_(factor)
        , use_custom_phi_(false) {}

    explicit IterativeResolver(double factor) noexcept
        : IterativeResolver(factor, kDefaultTolerance, kDefaultMaxIterations) {}

    ~IterativeResolver() override = default;

    [[nodiscard]] core::Result resolve(const core::ScalarFunction& func,
        const core::Range& range) const override;
};

} 