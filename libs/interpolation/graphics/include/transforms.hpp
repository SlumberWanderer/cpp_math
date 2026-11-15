#pragma once

#include <cstdint>
#include "image.hpp"

namespace numeric::graphics {

enum class InterpolationMode {
    Nearest,
    Bilinear,
    Bicubic
};

[[nodiscard]] double sample_nearest(const Image& image, double x, double y, std::uint8_t c);
[[nodiscard]] double sample_bilinear(const Image& image, double x, double y, std::uint8_t c);
[[nodiscard]] double sample_bicubic(const Image& image, double x, double y, std::uint8_t c);
[[nodiscard]] double sample_channel(const Image& image, double x, double y, std::uint8_t c, InterpolationMode mode);

[[nodiscard]] Image resize_image(
    const Image& source,
    std::uint32_t new_width,
    std::uint32_t new_height,
    InterpolationMode mode = InterpolationMode::Bilinear
);

[[nodiscard]] Image rotate_image(
    const Image& source,
    double angle_degrees,
    Rgb background = {0, 0, 0},
    InterpolationMode mode = InterpolationMode::Bilinear
);

[[nodiscard]] Rgba sample_rgba_direct(const Image& rgba_img, double x, double y);
[[nodiscard]] Rgba sample_rgba_premultiplied(const Image& rgba_img, double x, double y);

[[nodiscard]] Image resize_rgba(
    const Image& rgba_source,
    std::uint32_t new_width,
    std::uint32_t new_height,
    bool use_premultiplied_alpha
);

[[nodiscard]] Image composite_over_background(const Image& rgba_image, Rgb background);

[[nodiscard]] Image gaussian_blur(const Image& source, double sigma);

[[nodiscard]] Image downsample_image(
    const Image& source,
    std::uint32_t factor,
    bool apply_antialiasing,
    InterpolationMode mode = InterpolationMode::Bilinear
);

[[nodiscard]] double mean_absolute_diff(const Image& a, const Image& b);

}