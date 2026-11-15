#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include "image.hpp"

namespace numeric::graphics {

struct BmpLayoutInfo {
    std::uint32_t width;
    std::uint32_t height;
    std::size_t row_useful_bytes;
    std::size_t row_stride_bytes;
    std::size_t padding_per_row;
    std::size_t raw_memory_bytes;
    std::size_t bmp_pixel_data_bytes;
    std::size_t total_file_bytes;
};

[[nodiscard]] constexpr std::size_t bmp_row_stride(std::uint32_t width) noexcept {
    return ((static_cast<std::size_t>(width) * 3u + 3u) / 4u) * 4u;
}

[[nodiscard]] BmpLayoutInfo analyze_bmp_layout(std::uint32_t width, std::uint32_t height);

[[nodiscard]] Image load_bmp(const std::string& path);
void save_bmp(const Image& image, const std::string& path);

}