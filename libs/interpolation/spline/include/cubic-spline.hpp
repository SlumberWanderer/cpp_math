#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include "band-matrix.hpp"
#include "boundary-generator.hpp"
#include "spline-core.hpp"
#include "vector.hpp"

namespace interpolation::spline::cubic {

using core::Node;
using core::Nodes;
using core::Segment;
using plane2d::Point;
using plane2d::Points;

class CubicSpline {
public:
    using CubicSegment = Segment<3>;
    using Segments = std::vector<CubicSegment>;
    using Generator = std::shared_ptr<const core::boundary::IBoundaryEquationGenerator>;

private:
    Generator bcl_generator_;
    Generator bcr_generator_;
    Nodes nodes_;
    Segments segments_;

    void build();
    [[nodiscard]] std::size_t interval(double x, bool allow_extrapolation) const;
    [[nodiscard]] double eval_on_segment(std::size_t seg_idx, double x, std::size_t order) const;

public:
    explicit CubicSpline(const Points& points);
    CubicSpline(const Points& points, Generator left, Generator right);

    [[nodiscard]] double evaluate(double x) const;
    [[nodiscard]] double evaluate_clamped(double x) const;
    [[nodiscard]] double operator()(double x) const { return evaluate_clamped(x); }

    [[nodiscard]] double derivative(double x, std::size_t order = 1) const;

    [[nodiscard]] const Nodes& nodes() const noexcept { return nodes_; }
    [[nodiscard]] const Segments& segments() const noexcept { return segments_; }
};

}