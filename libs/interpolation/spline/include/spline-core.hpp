#pragma once

#include <array>
#include <cstddef>
#include <stdexcept>
#include <vector>
#include "plane2d.hpp"

namespace interpolation::spline::core {

using plane2d::Point;
using plane2d::Points;

class Node {
private:
    int index_;
    Point point_;

public:
    constexpr Node(int index, Point point) noexcept
        : index_(index), point_(point) {}

    [[nodiscard]] constexpr int index() const noexcept { return index_; }
    [[nodiscard]] constexpr Point point() const noexcept { return point_; }
};

using Nodes = std::vector<Node>;

template <int Degree>
class Segment {
public:
    using Coefficients = std::array<double, Degree + 1>;

private:
    Coefficients coefficients_;
    Node start_;
    Node end_;

    static void check_index(int index) {
        if (index < 0 || index > Degree) {
            throw std::out_of_range("Invalid index of spline segment coefficient");
        }
    }

public:
    Segment(const Coefficients& coefficients, const Node& start, const Node& end) noexcept
        : coefficients_(coefficients), start_(start), end_(end) {}

    [[nodiscard]] const Node& start() const noexcept { return start_; }
    [[nodiscard]] const Node& end() const noexcept { return end_; }

    [[nodiscard]] double get_coefficient(int index) const {
        check_index(index);
        return coefficients_[static_cast<std::size_t>(index)];
    }

    [[nodiscard]] const Coefficients& coefficients() const noexcept {
        return coefficients_;
    }
};

[[nodiscard]] Nodes create_from(const Points& points);

}