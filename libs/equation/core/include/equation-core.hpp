#pragma once

#include "range.hpp"
#include "result.hpp"
#include <cstddef>
#include <functional>

namespace equation::core {

using ScalarFunction = std::function<double(double)>;

class IEquationResolver {
protected:
    double tolerance_;
    std::size_t max_iterations_;
    mutable std::size_t iterations_count_;

    [[nodiscard]] static bool has_root_for(const ScalarFunction& func, const Range& range);

public:
    static constexpr std::size_t kDefaultMaxIterations = 10000;
    static constexpr double kDefaultTolerance = 1e-6;

    IEquationResolver(double tolerance, std::size_t max_iterations) noexcept
        : tolerance_(tolerance)
        , max_iterations_(max_iterations)
        , iterations_count_(0) {}

    explicit IEquationResolver(double tolerance) noexcept
        : IEquationResolver(tolerance, kDefaultMaxIterations) {}

    IEquationResolver() noexcept
        : IEquationResolver(kDefaultTolerance, kDefaultMaxIterations) {}

    virtual ~IEquationResolver() = default;

    [[nodiscard]] virtual Result resolve(const ScalarFunction& func, const Range& range) const = 0;

    [[nodiscard]] double deviate(const ScalarFunction& func, double root) const {
        return func(root);
    }

    [[nodiscard]] std::size_t count_of_iterations() const noexcept {
        return iterations_count_;
    }
};

} 