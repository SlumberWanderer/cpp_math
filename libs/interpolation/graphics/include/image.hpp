#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <vector>

namespace numeric::graphics {

struct Rgb {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};

    constexpr bool operator==(const Rgb& other) const noexcept {
        return r == other.r && g == other.g && b == other.b;
    }
};

struct Rgba {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};

    constexpr bool operator==(const Rgba& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

[[nodiscard]] inline std::uint8_t to_byte(double value) noexcept {
    return static_cast<std::uint8_t>(std::lround(std::clamp(value, 0.0, 255.0)));
}

struct Image {
    std::uint32_t width{0};
    std::uint32_t height{0};
    std::uint8_t channels{3};
    std::vector<std::uint8_t> bytes;

    Image() = default;

    Image(std::uint32_t w, std::uint32_t h, std::uint8_t c = 3)
        : width(w), height(h), channels(c) {
        if (w == 0 || h == 0 || (c != 1 && c != 3 && c != 4)) {
            throw std::invalid_argument("Invalid image dimensions or channel count");
        }
        const std::size_t count = static_cast<std::size_t>(w) * h * c;
        if (count / c / h != w) {
            throw std::overflow_error("Image buffer size overflow");
        }
        bytes.resize(count, 0);
    }

    [[nodiscard]] std::size_t index(std::uint32_t x, std::uint32_t y, std::uint8_t channel) const {
        if (x >= width || y >= height || channel >= channels) {
            throw std::out_of_range("Pixel coordinates or channel out of range");
        }
        return (static_cast<std::size_t>(y) * width + x) * channels + channel;
    }

    std::uint8_t& at(std::uint32_t x, std::uint32_t y, std::uint8_t c) {
        return bytes.at(index(x, y, c));
    }

    [[nodiscard]] std::uint8_t at(std::uint32_t x, std::uint32_t y, std::uint8_t c) const {
        return bytes.at(index(x, y, c));
    }

    [[nodiscard]] Rgb get_rgb(std::uint32_t x, std::uint32_t y) const {
        if (channels < 3) {
            throw std::logic_error("get_rgb requires at least 3 channels");
        }
        return Rgb{at(x, y, 0), at(x, y, 1), at(x, y, 2)};
    }

    void set_rgb(std::uint32_t x, std::uint32_t y, Rgb color) {
        if (channels < 3) {
            throw std::logic_error("set_rgb requires at least 3 channels");
        }
        at(x, y, 0) = color.r;
        at(x, y, 1) = color.g;
        at(x, y, 2) = color.b;
    }

    [[nodiscard]] Rgba get_rgba(std::uint32_t x, std::uint32_t y) const {
        if (channels != 4) {
            throw std::logic_error("get_rgba requires 4 channels");
        }
        return Rgba{at(x, y, 0), at(x, y, 1), at(x, y, 2), at(x, y, 3)};
    }

    void set_rgba(std::uint32_t x, std::uint32_t y, Rgba color) {
        if (channels != 4) {
            throw std::logic_error("set_rgba requires 4 channels");
        }
        at(x, y, 0) = color.r;
        at(x, y, 1) = color.g;
        at(x, y, 2) = color.b;
        at(x, y, 3) = color.a;
    }
};

}