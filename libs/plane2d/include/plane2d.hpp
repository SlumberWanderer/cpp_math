#pragma once

#include <cstddef>
#include <functional>
#include <iostream>
#include <vector>

namespace plane2d {

struct Point {
    double x{0.0};
    double y{0.0};

    constexpr Point() noexcept = default;
    constexpr Point(double x_val, double y_val) noexcept : x(x_val), y(y_val) {}
};

using Points = std::vector<Point>;
using ScalarFunction = std::function<double(double)>;

void validate(const Points& points);

[[nodiscard]] Points tabulate(const ScalarFunction& func, double a, double b, std::size_t count);
[[nodiscard]] Points tabulate_by_step(const ScalarFunction& func, double a, double b, double step);

[[nodiscard]] Point left_point(const Points& points);
[[nodiscard]] Point right_point(const Points& points);

std::ostream& operator<<(std::ostream& os, const Point& point);
std::ostream& operator<<(std::ostream& os, const Points& points);

}