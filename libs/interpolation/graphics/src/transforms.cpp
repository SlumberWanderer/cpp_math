#include "transforms.hpp"
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <vector>

namespace numeric::graphics {
namespace {

const double kPi = std::acos(-1.0);

double cubic_hermite_weight(double t) noexcept {
    constexpr double a = -0.5;
    const double abs_t = std::abs(t);
    if (abs_t <= 1.0) {
        return (a + 2.0) * abs_t * abs_t * abs_t - (a + 3.0) * abs_t * abs_t + 1.0;
    }
    if (abs_t < 2.0) {
        return a * abs_t * abs_t * abs_t - 5.0 * a * abs_t * abs_t + 8.0 * a * abs_t - 4.0 * a;
    }
    return 0.0;
}

std::uint32_t clamp_coord(int val, std::uint32_t limit) noexcept {
    if (val < 0) {
        return 0;
    }
    const auto uval = static_cast<std::uint32_t>(val);
    return (uval >= limit) ? (limit - 1u) : uval;
}

} 

double sample_nearest(const Image& image, double x, double y, std::uint8_t c) {
    const int ix = static_cast<int>(std::lround(x));
    const int iy = static_cast<int>(std::lround(y));
    const auto cx = clamp_coord(ix, image.width);
    const auto cy = clamp_coord(iy, image.height);
    return static_cast<double>(image.at(cx, cy, c));
}

double sample_bilinear(const Image& image, double x, double y, std::uint8_t c) {
    x = std::clamp(x, 0.0, static_cast<double>(image.width - 1u));
    y = std::clamp(y, 0.0, static_cast<double>(image.height - 1u));

    const auto x0 = static_cast<std::uint32_t>(std::floor(x));
    const auto y0 = static_cast<std::uint32_t>(std::floor(y));
    const auto x1 = std::min(x0 + 1u, image.width - 1u);
    const auto y1 = std::min(y0 + 1u, image.height - 1u);

    const double tx = x - x0;
    const double ty = y - y0;

    return (1.0 - tx) * (1.0 - ty) * image.at(x0, y0, c) +
           tx * (1.0 - ty) * image.at(x1, y0, c) +
           (1.0 - tx) * ty * image.at(x0, y1, c) +
           tx * ty * image.at(x1, y1, c);
}

double sample_bicubic(const Image& image, double x, double y, std::uint8_t c) {
    x = std::clamp(x, 0.0, static_cast<double>(image.width - 1u));
    y = std::clamp(y, 0.0, static_cast<double>(image.height - 1u));

    const int ix = static_cast<int>(std::floor(x));
    const int iy = static_cast<int>(std::floor(y));
    const double dx = x - ix;
    const double dy = y - iy;

    double sum = 0.0;
    for (int m = -1; m <= 2; ++m) {
        const double wy = cubic_hermite_weight(dy - m);
        const auto sy = clamp_coord(iy + m, image.height);
        for (int n = -1; n <= 2; ++n) {
            const double wx = cubic_hermite_weight(dx - n);
            const auto sx = clamp_coord(ix + n, image.width);
            sum += wx * wy * static_cast<double>(image.at(sx, sy, c));
        }
    }
    return sum;
}

double sample_channel(const Image& image, double x, double y, std::uint8_t c, InterpolationMode mode) {
    switch (mode) {
        case InterpolationMode::Nearest:
            return sample_nearest(image, x, y, c);
        case InterpolationMode::Bilinear:
            return sample_bilinear(image, x, y, c);
        case InterpolationMode::Bicubic:
            return sample_bicubic(image, x, y, c);
    }
    return sample_bilinear(image, x, y, c);
}

Image resize_image(
    const Image& source,
    std::uint32_t new_width,
    std::uint32_t new_height,
    InterpolationMode mode
) {
    Image result(new_width, new_height, source.channels);
    for (std::uint32_t y = 0; y < new_height; ++y) {
        const double sy = (y + 0.5) * source.height / static_cast<double>(new_height) - 0.5;
        for (std::uint32_t x = 0; x < new_width; ++x) {
            const double sx = (x + 0.5) * source.width / static_cast<double>(new_width) - 0.5;
            for (std::uint8_t c = 0; c < source.channels; ++c) {
                result.at(x, y, c) = to_byte(sample_channel(source, sx, sy, c, mode));
            }
        }
    }
    return result;
}

Image rotate_image(
    const Image& source,
    double angle_degrees,
    Rgb background,
    InterpolationMode mode
) {
    if (source.channels != 3) {
        throw std::invalid_argument("rotate_image requires a 3-channel RGB image");
    }

    Image result(source.width, source.height, 3);
    const double rad = angle_degrees * kPi / 180.0;
    const double cos_a = std::cos(rad);
    const double sin_a = std::sin(rad);

    const double cx = (source.width - 1.0) / 2.0;
    const double cy = (source.height - 1.0) / 2.0;

    for (std::uint32_t y = 0; y < result.height; ++y) {
        for (std::uint32_t x = 0; x < result.width; ++x) {
            const double dx = x - cx;
            const double dy = y - cy;

            const double sx =  dx * cos_a + dy * sin_a + cx;
            const double sy = -dx * sin_a + dy * cos_a + cy;

            if (sx < 0.0 || sx > static_cast<double>(source.width - 1u) ||
                sy < 0.0 || sy > static_cast<double>(source.height - 1u)) {
                result.set_rgb(x, y, background);
            } else {
                result.at(x, y, 0) = to_byte(sample_channel(source, sx, sy, 0, mode));
                result.at(x, y, 1) = to_byte(sample_channel(source, sx, sy, 1, mode));
                result.at(x, y, 2) = to_byte(sample_channel(source, sx, sy, 2, mode));
            }
        }
    }
    return result;
}

Rgba sample_rgba_direct(const Image& rgba_img, double x, double y) {
    if (rgba_img.channels != 4) {
        throw std::invalid_argument("RGBA sampling requires 4 channels");
    }
    return Rgba{
        to_byte(sample_bilinear(rgba_img, x, y, 0)),
        to_byte(sample_bilinear(rgba_img, x, y, 1)),
        to_byte(sample_bilinear(rgba_img, x, y, 2)),
        to_byte(sample_bilinear(rgba_img, x, y, 3))
    };
}

Rgba sample_rgba_premultiplied(const Image& rgba_img, double x, double y) {
    if (rgba_img.channels != 4) {
        throw std::invalid_argument("RGBA sampling requires 4 channels");
    }

    x = std::clamp(x, 0.0, static_cast<double>(rgba_img.width - 1u));
    y = std::clamp(y, 0.0, static_cast<double>(rgba_img.height - 1u));

    const auto x0 = static_cast<std::uint32_t>(std::floor(x));
    const auto y0 = static_cast<std::uint32_t>(std::floor(y));
    const auto x1 = std::min(x0 + 1u, rgba_img.width - 1u);
    const auto y1 = std::min(y0 + 1u, rgba_img.height - 1u);

    const double tx = x - x0;
    const double ty = y - y0;

    const double w00 = (1.0 - tx) * (1.0 - ty);
    const double w10 = tx * (1.0 - ty);
    const double w01 = (1.0 - tx) * ty;
    const double w11 = tx * ty;

    const auto p00 = rgba_img.get_rgba(x0, y0);
    const auto p10 = rgba_img.get_rgba(x1, y0);
    const auto p01 = rgba_img.get_rgba(x0, y1);
    const auto p11 = rgba_img.get_rgba(x1, y1);

    const double a00 = p00.a / 255.0;
    const double a10 = p10.a / 255.0;
    const double a01 = p01.a / 255.0;
    const double a11 = p11.a / 255.0;

    const double a_interp = w00 * a00 + w10 * a10 + w01 * a01 + w11 * a11;
    if (a_interp <= 1e-9) {
        return Rgba{0, 0, 0, 0};
    }

    const double pr = w00 * (p00.r * a00) + w10 * (p10.r * a10) + w01 * (p01.r * a01) + w11 * (p11.r * a11);
    const double pg = w00 * (p00.g * a00) + w10 * (p10.g * a10) + w01 * (p01.g * a01) + w11 * (p11.g * a11);
    const double pb = w00 * (p00.b * a00) + w10 * (p10.b * a10) + w01 * (p01.b * a01) + w11 * (p11.b * a11);

    return Rgba{
        to_byte(pr / a_interp),
        to_byte(pg / a_interp),
        to_byte(pb / a_interp),
        to_byte(a_interp * 255.0)
    };
}

Image resize_rgba(
    const Image& rgba_source,
    std::uint32_t new_width,
    std::uint32_t new_height,
    bool use_premultiplied_alpha
) {
    if (rgba_source.channels != 4) {
        throw std::invalid_argument("resize_rgba expects a 4-channel RGBA image");
    }

    Image result(new_width, new_height, 4);
    for (std::uint32_t y = 0; y < new_height; ++y) {
        const double sy = (y + 0.5) * rgba_source.height / static_cast<double>(new_height) - 0.5;
        for (std::uint32_t x = 0; x < new_width; ++x) {
            const double sx = (x + 0.5) * rgba_source.width / static_cast<double>(new_width) - 0.5;
            const Rgba px = use_premultiplied_alpha
                ? sample_rgba_premultiplied(rgba_source, sx, sy)
                : sample_rgba_direct(rgba_source, sx, sy);
            result.set_rgba(x, y, px);
        }
    }
    return result;
}

Image composite_over_background(const Image& rgba_image, Rgb background) {
    if (rgba_image.channels != 4) {
        throw std::invalid_argument("composite_over_background requires 4-channel RGBA image");
    }

    Image rgb_out(rgba_image.width, rgba_image.height, 3);
    for (std::uint32_t y = 0; y < rgba_image.height; ++y) {
        for (std::uint32_t x = 0; x < rgba_image.width; ++x) {
            const auto px = rgba_image.get_rgba(x, y);
            const double alpha = px.a / 255.0;
            rgb_out.set_rgb(x, y, Rgb{
                to_byte(px.r * alpha + background.r * (1.0 - alpha)),
                to_byte(px.g * alpha + background.g * (1.0 - alpha)),
                to_byte(px.b * alpha + background.b * (1.0 - alpha))
            });
        }
    }
    return rgb_out;
}

Image gaussian_blur(const Image& source, double sigma) {
    if (sigma <= 0.0) {
        return source;
    }

    const int radius = std::max(1, static_cast<int>(std::ceil(3.0 * sigma)));
    const int ksize = 2 * radius + 1;
    std::vector<double> kernel(static_cast<std::size_t>(ksize));

    double weight_sum = 0.0;
    for (int i = -radius; i <= radius; ++i) {
        const double w = std::exp(-(i * i) / (2.0 * sigma * sigma));
        kernel[static_cast<std::size_t>(i + radius)] = w;
        weight_sum += w;
    }
    for (double& w : kernel) {
        w /= weight_sum;
    }

    Image temp(source.width, source.height, source.channels);
    Image result(source.width, source.height, source.channels);

    // Horizontal pass
    for (std::uint32_t y = 0; y < source.height; ++y) {
        for (std::uint32_t x = 0; x < source.width; ++x) {
            for (std::uint8_t c = 0; c < source.channels; ++c) {
                double acc = 0.0;
                for (int k = -radius; k <= radius; ++k) {
                    const auto sx = clamp_coord(static_cast<int>(x) + k, source.width);
                    acc += kernel[static_cast<std::size_t>(k + radius)] * source.at(sx, y, c);
                }
                temp.at(x, y, c) = to_byte(acc);
            }
        }
    }

    // Vertical pass
    for (std::uint32_t y = 0; y < source.height; ++y) {
        for (std::uint32_t x = 0; x < source.width; ++x) {
            for (std::uint8_t c = 0; c < source.channels; ++c) {
                double acc = 0.0;
                for (int k = -radius; k <= radius; ++k) {
                    const auto sy = clamp_coord(static_cast<int>(y) + k, source.height);
                    acc += kernel[static_cast<std::size_t>(k + radius)] * temp.at(x, sy, c);
                }
                result.at(x, y, c) = to_byte(acc);
            }
        }
    }

    return result;
}

Image downsample_image(
    const Image& source,
    std::uint32_t factor,
    bool apply_antialiasing,
    InterpolationMode mode
) {
    if (factor == 0 || source.width < factor || source.height < factor) {
        throw std::invalid_argument("Invalid downsampling factor");
    }

    const std::uint32_t target_w = source.width / factor;
    const std::uint32_t target_h = source.height / factor;

    if (apply_antialiasing && factor > 1) {
        const double sigma = 0.45 * static_cast<double>(factor);
        const Image filtered = gaussian_blur(source, sigma);
        return resize_image(filtered, target_w, target_h, mode);
    }
    return resize_image(source, target_w, target_h, mode);
}

double mean_absolute_diff(const Image& a, const Image& b) {
    if (a.width != b.width || a.height != b.height || a.channels != b.channels) {
        throw std::invalid_argument("Images must have matching dimensions and channels");
    }
    double sum = 0.0;
    for (std::size_t i = 0; i < a.bytes.size(); ++i) {
        sum += std::abs(static_cast<double>(a.bytes[i]) - static_cast<double>(b.bytes[i]));
    }
    return sum / static_cast<double>(a.bytes.size());
}

}