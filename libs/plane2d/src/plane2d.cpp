#include "plane2d.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace plane2d {
namespace {
constexpr double kEps = 1e-12;
}

void validate(const Points& points) {
    if (points.empty()) {
        throw std::invalid_argument("Empty dataset");
    }

    const std::size_t size = points.size();
    for (std::size_t i = 0; i < size; ++i) {
        const auto [x, y] = points[i];
        if (!std::isfinite(x) || !std::isfinite(y)) {
            throw std::invalid_argument("Point coordinates must be finite");
        }
        for (std::size_t j = i + 1; j < size; ++j) {
            if (std::abs(points[i].x - points[j].x) <= kEps) {
                throw std::invalid_argument("Points must have distinct x coordinates");
            }
        }
    }
}

Points tabulate(const ScalarFunction& func, double a, double b, std::size_t count) {
    if (!func) {
        throw std::invalid_argument("Function must be defined");
    }
    if (count < 2) {
        throw std::invalid_argument("Point count must be at least 2");
    }
    if (!std::isfinite(a) || !std::isfinite(b) || b <= a) {
        throw std::invalid_argument("Invalid tabulation interval");
    }

    Points points;
    points.reserve(count);
    const double step = (b - a) / static_cast<double>(count - 1);

    for (std::size_t i = 0; i < count; ++i) {
        const double x = (i + 1 == count) ? b : (a + step * static_cast<double>(i));
        points.emplace_back(x, func(x));
    }
    return points;
}

Points tabulate_by_step(const ScalarFunction& func, double a, double b, double step) {
    if (!func) {
        throw std::invalid_argument("Function must be defined");
    }
    if (!std::isfinite(a) || !std::isfinite(b) || !std::isfinite(step) || step <= 0.0 || b <= a) {
        throw std::invalid_argument("Invalid tabulation parameters");
    }

    Points points;
    for (double x = a; x < b - 1e-9; x += step) {
        points.emplace_back(x, func(x));
    }
    points.emplace_back(b, func(b));
    return points;
}

Point left_point(const Points& points) {
    if (points.empty()) {
        throw std::invalid_argument("Empty dataset");
    }
    const auto it = std::min_element(points.begin(), points.end(),
        [](const Point& lhs, const Point& rhs) { return lhs.x < rhs.x; });
    return *it;
}

Point right_point(const Points& points) {
    if (points.empty()) {
        throw std::invalid_argument("Empty dataset");
    }
    const auto it = std::max_element(points.begin(), points.end(),
        [](const Point& lhs, const Point& rhs) { return lhs.x < rhs.x; });
    return *it;
}

std::ostream& operator<<(std::ostream& os, const Point& p) {
    os << "(" << p.x << "; " << p.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Points& points) {
    os << "[";
    for (std::size_t i = 0; i < points.size(); ++i) {
        os << points[i];
        if (i + 1 < points.size()) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

}