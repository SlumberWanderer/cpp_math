#include "cubic-spline.hpp"
#include "natural-boundary-generator.hpp"
#include "tridiagonal-resolver.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace interpolation::spline::cubic {
namespace {

void validate_points(const Points& points) {
    if (points.size() < 2) {
        throw std::invalid_argument("At least two interpolation points are required");
    }
    for (std::size_t i = 0; i < points.size(); ++i) {
        if (!std::isfinite(points[i].x) || !std::isfinite(points[i].y)) {
            throw std::invalid_argument("Spline points must be finite");
        }
        if (i > 0 && points[i].x <= points[i - 1].x) {
            throw std::invalid_argument("Spline points must have strictly increasing x");
        }
    }
}

} // namespace

CubicSpline::CubicSpline(const Points& points)
    : CubicSpline(
          points,
          std::make_shared<core::boundary::NaturalBoundaryEquationGenerator>(),
          std::make_shared<core::boundary::NaturalBoundaryEquationGenerator>()
      ) {}

CubicSpline::CubicSpline(const Points& points, Generator left, Generator right)
    : bcl_generator_(std::move(left)),
      bcr_generator_(std::move(right)) {
    validate_points(points);
    if (!bcl_generator_ || !bcr_generator_) {
        throw std::invalid_argument("Boundary generators must not be null");
    }
    nodes_ = core::create_from(points);
    build();
}

void CubicSpline::build() {
    using core::boundary::BoundarySide;
    using linear_algebra::BandMatrix;
    using linear_algebra::Vector;
    using linear_algebra::band::TridiagonalResolver;

    const std::size_t n_count = nodes_.size();
    const std::size_t up_index = n_count - 1;

    std::vector<double> h(up_index);
    std::vector<double> delta(up_index);

    for (std::size_t i = 0; i < up_index; ++i) {
        const auto [x0, y0] = nodes_[i].point();
        const auto [x1, y1] = nodes_[i + 1].point();
        h[i] = x1 - x0;
        delta[i] = (y1 - y0) / h[i];
    }

    Vector lower(n_count, 0.0);
    Vector diag(n_count, 0.0);
    Vector upper(n_count, 0.0);
    Vector rhs(n_count, 0.0);

    for (std::size_t i = 1; i < up_index; ++i) {
        lower[i] = h[i - 1];
        diag[i] = 2.0 * (h[i - 1] + h[i]);
        upper[i] = h[i];
        rhs[i] = 3.0 * (delta[i] - delta[i - 1]);
    }

    {
        const auto [d0, adj0, r0] = bcl_generator_->generate({
            BoundarySide::Left, h.front(), delta.front()
        });
        diag[0] = d0;
        upper[0] = adj0;
        rhs[0] = r0;
    }

    {
        const auto [dn, adjn, rn] = bcr_generator_->generate({
            BoundarySide::Right, h.back(), delta.back()
        });
        lower[up_index] = adjn;
        diag[up_index] = dn;
        rhs[up_index] = rn;
    }

    const BandMatrix matrix(lower, diag, upper);
    const TridiagonalResolver solver;
    const Vector c = solver.resolve(matrix, rhs);

    segments_.clear();
    segments_.reserve(up_index);

    for (std::size_t i = 0; i < up_index; ++i) {
        CubicSegment::Coefficients coeffs{};
        coeffs[0] = nodes_[i].point().y;
        coeffs[1] = delta[i] - h[i] * (2.0 * c[i] + c[i + 1]) / 3.0;
        coeffs[2] = c[i];
        coeffs[3] = (c[i + 1] - c[i]) / (3.0 * h[i]);

        segments_.emplace_back(coeffs, nodes_[i], nodes_[i + 1]);
    }
}

std::size_t CubicSpline::interval(double x, bool allow_extrapolation) const {
    if (!std::isfinite(x)) {
        throw std::invalid_argument("Argument must be finite");
    }

    const double first = nodes_.front().point().x;
    const double last = nodes_.back().point().x;

    if (!allow_extrapolation && (x < first || x > last)) {
        throw std::out_of_range("Argument outside interpolation domain");
    }

    if (x <= first) {
        return 0;
    }
    if (x >= last) {
        return nodes_.size() - 2;
    }

    const auto it = std::upper_bound(
        nodes_.begin(),
        nodes_.end(),
        x,
        [](double value, const Node& node) {
            return value < node.point().x;
        }
    );

    return static_cast<std::size_t>(std::distance(nodes_.begin(), it) - 1);
}

double CubicSpline::eval_on_segment(std::size_t seg_idx, double x, std::size_t order) const {
    const auto& seg = segments_[seg_idx];
    const double t = x - nodes_[seg_idx].point().x;

    const double a = seg.get_coefficient(0);
    const double b = seg.get_coefficient(1);
    const double c = seg.get_coefficient(2);
    const double d = seg.get_coefficient(3);

    switch (order) {
        case 0:
            return ((d * t + c) * t + b) * t + a;
        case 1:
            return (3.0 * d * t + 2.0 * c) * t + b;
        case 2:
            return 6.0 * d * t + 2.0 * c;
        case 3:
            return 6.0 * d;
        default:
            return 0.0;
    }
}

double CubicSpline::evaluate(double x) const {
    return eval_on_segment(interval(x, false), x, 0);
}

double CubicSpline::evaluate_clamped(double x) const {
    return eval_on_segment(interval(x, true), x, 0);
}

double CubicSpline::derivative(double x, std::size_t order) const {
    return eval_on_segment(interval(x, false), x, order);
}

}