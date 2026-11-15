#include "spline-core.hpp"

namespace interpolation::spline::core {

Nodes create_from(const Points& points) {
    Nodes result;
    result.reserve(points.size());
    for (std::size_t i = 0; i < points.size(); ++i) {
        result.emplace_back(static_cast<int>(i), points[i]);
    }
    return result;
}

}