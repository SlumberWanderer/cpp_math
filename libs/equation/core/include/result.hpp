#pragma once

#include <cstddef>
#include <iostream>

namespace equation::core {

enum class ResultType {
    DEFINED,
    UNDEFINED
};

class Result {
private:
    ResultType type_;
    double value_;
    std::size_t iterations_;

public:
    Result(ResultType type, double value, std::size_t iterations = 0) noexcept
        : type_(type), value_(value), iterations_(iterations) {}

    [[nodiscard]] ResultType type() const noexcept { return type_; }
    [[nodiscard]] double value() const noexcept { return value_; }
    [[nodiscard]] std::size_t iterations() const noexcept { return iterations_; }

    [[nodiscard]] bool is_defined() const noexcept {
        return type_ == ResultType::DEFINED;
    }
};

std::ostream& operator<<(std::ostream& os, const Result& result);

} 