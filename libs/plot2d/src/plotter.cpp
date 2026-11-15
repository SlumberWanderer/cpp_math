#include "plotter.hpp"

#include <sciplot/sciplot.hpp>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace plot2d {
namespace {

sciplot::Plot2D build_plot(const Plot& source) {
    Plotter::validate(source);

    sciplot::Plot2D result;
    result.xlabel(source.x_label());
    result.ylabel(source.y_label());

    if (source.grid_enabled()) {
        result.grid().show();
    }

    for (const auto& curve : source.curves()) {
        std::vector<double> x;
        std::vector<double> y;
        x.reserve(curve.points.size());
        y.reserve(curve.points.size());

        for (const auto& pt : curve.points) {
            x.push_back(pt.x);
            y.push_back(pt.y);
        }

        switch (curve.style) {
            case CurveStyle::Line:
                result.drawCurve(x, y).label(curve.label).lineWidth(curve.line_width);
                break;
            case CurveStyle::Points:
                result.drawPoints(x, y).label(curve.label);
                break;
            case CurveStyle::LineWithPoints:
                result.drawCurveWithPoints(x, y).label(curve.label).lineWidth(curve.line_width);
                break;
        }
    }
    return result;
}

sciplot::Canvas build_canvas(const Plot& plot) {
    auto graph = build_plot(plot);
    sciplot::Figure figure = {{graph}};
    if (!plot.title().empty()) {
        figure.title(plot.title());
    }
    sciplot::Canvas canvas = {{figure}};
    canvas.size(plot.width(), plot.height());
    return canvas;
}

}

void Plotter::validate(const Plot& plot) {
    if (plot.width() == 0 || plot.height() == 0) {
        throw std::invalid_argument("Plot dimensions must be positive");
    }
    if (plot.curves().empty()) {
        throw std::invalid_argument("Plot must contain at least one curve");
    }
    for (const auto& curve : plot.curves()) {
        if (curve.points.empty()) {
            throw std::invalid_argument("Curve must contain at least one point");
        }
        if (curve.line_width <= 0) {
            throw std::invalid_argument("Line width must be positive");
        }
        for (const auto& pt : curve.points) {
            if (!std::isfinite(pt.x) || !std::isfinite(pt.y)) {
                throw std::invalid_argument("Plot coordinates must be finite");
            }
        }
    }
}

void Plotter::save(const Plot& plot, const std::string& filename) const {
    if (filename.empty()) {
        throw std::invalid_argument("Output filename must not be empty");
    }
    auto canvas = build_canvas(plot);
    canvas.save(filename);
}

void Plotter::show(const Plot& plot) const {
    auto canvas = build_canvas(plot);
    canvas.show();
}

}