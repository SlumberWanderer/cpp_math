#pragma once

#include <cstddef>
#include <string>
#include <utility>
#include <vector>
#include "curve.hpp"

namespace plot2d {

class Plot {
private:
    std::string title_;
    std::string x_label_ = "x";
    std::string y_label_ = "y";
    std::vector<Curve> curves_;
    bool grid_enabled_ = true;
    std::size_t width_ = 1200;
    std::size_t height_ = 750;

public:
    explicit Plot(std::string title = {}) noexcept : title_(std::move(title)) {}

    Plot& add_curve(Curve curve) {
        curves_.push_back(std::move(curve));
        return *this;
    }

    Plot& add_line(plane2d::Points points, std::string label, int line_width = 2) {
        curves_.push_back(Curve{std::move(points), std::move(label), CurveStyle::Line, line_width});
        return *this;
    }

    Plot& add_points(plane2d::Points points, std::string label) {
        curves_.push_back(Curve{std::move(points), std::move(label), CurveStyle::Points, 2});
        return *this;
    }

    Plot& add_line_with_points(plane2d::Points points, std::string label, int line_width = 2) {
        curves_.push_back(Curve{std::move(points), std::move(label), CurveStyle::LineWithPoints, line_width});
        return *this;
    }

    Plot& set_labels(std::string x, std::string y) {
        x_label_ = std::move(x);
        y_label_ = std::move(y);
        return *this;
    }

    Plot& set_grid_enabled(bool enabled) noexcept {
        grid_enabled_ = enabled;
        return *this;
    }

    Plot& set_size(std::size_t width, std::size_t height) noexcept {
        width_ = width;
        height_ = height;
        return *this;
    }

    [[nodiscard]] const std::string& title() const noexcept { return title_; }
    [[nodiscard]] const std::string& x_label() const noexcept { return x_label_; }
    [[nodiscard]] const std::string& y_label() const noexcept { return y_label_; }
    [[nodiscard]] const std::vector<Curve>& curves() const noexcept { return curves_; }
    [[nodiscard]] bool grid_enabled() const noexcept { return grid_enabled_; }
    [[nodiscard]] std::size_t width() const noexcept { return width_; }
    [[nodiscard]] std::size_t height() const noexcept { return height_; }
};

}