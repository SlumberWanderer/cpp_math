#include "bmp.hpp"
#include <fstream>
#include <limits>
#include <stdexcept>
#include <vector>

namespace numeric::graphics {
namespace {

std::uint16_t read16(std::istream& in) {
    std::uint8_t b[2]{};
    if (!in.read(reinterpret_cast<char*>(b), 2)) {
        throw std::runtime_error("Truncated BMP header");
    }
    return static_cast<std::uint16_t>(b[0]) |
           static_cast<std::uint16_t>(static_cast<std::uint16_t>(b[1]) << 8);
}

std::uint32_t read32(std::istream& in) {
    const std::uint32_t lo = read16(in);
    const std::uint32_t hi = read16(in);
    return lo | (hi << 16);
}

void write16(std::ostream& out, std::uint16_t v) {
    out.put(static_cast<char>(v & 0xFFu));
    out.put(static_cast<char>((v >> 8) & 0xFFu));
}

void write32(std::ostream& out, std::uint32_t v) {
    write16(out, static_cast<std::uint16_t>(v & 0xFFFFu));
    write16(out, static_cast<std::uint16_t>((v >> 16) & 0xFFFFu));
}

} 

BmpLayoutInfo analyze_bmp_layout(std::uint32_t width, std::uint32_t height) {
    if (width == 0 || height == 0) {
        throw std::invalid_argument("Dimensions must be positive");
    }
    const std::size_t useful = static_cast<std::size_t>(width) * 3u;
    const std::size_t stride = bmp_row_stride(width);
    const std::size_t padding = stride - useful;
    const std::size_t raw_bytes = useful * height;
    const std::size_t bmp_bytes = stride * height;
    return BmpLayoutInfo{
        width,
        height,
        useful,
        stride,
        padding,
        raw_bytes,
        bmp_bytes,
        54u + bmp_bytes
    };
}

Image load_bmp(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw std::runtime_error("Cannot open BMP file: " + path);
    }
    if (read16(in) != 0x4D42) {
        throw std::runtime_error("Invalid BMP signature");
    }

    (void)read32(in); 
    (void)read16(in); 
    (void)read16(in); 
    const auto data_offset = read32(in);
    const auto dib_size = read32(in);
    if (dib_size < 40) {
        throw std::runtime_error("Unsupported DIB header size");
    }

    const auto w = static_cast<std::int32_t>(read32(in));
    const auto h = static_cast<std::int32_t>(read32(in));
    const auto planes = read16(in);
    const auto bpp = read16(in);
    const auto compression = read32(in);

    if (w <= 0 || h == 0 || h == std::numeric_limits<std::int32_t>::min() ||
        planes != 1 || bpp != 24 || compression != 0) {
        throw std::runtime_error("Only 24-bit uncompressed BI_RGB BMP is supported");
    }

    const bool bottom_up = (h > 0);
    const auto width = static_cast<std::uint32_t>(w);
    const auto height = static_cast<std::uint32_t>(bottom_up ? h : -h);
    const std::uint64_t stride = bmp_row_stride(width);

    Image image(width, height, 3);
    in.seekg(data_offset, std::ios::beg);
    if (!in) {
        throw std::runtime_error("Invalid BMP pixel data offset");
    }

    std::vector<std::uint8_t> row(static_cast<std::size_t>(stride));
    for (std::uint32_t file_y = 0; file_y < height; ++file_y) {
        if (!in.read(reinterpret_cast<char*>(row.data()), static_cast<std::streamsize>(stride))) {
            throw std::runtime_error("Truncated BMP pixel array");
        }
        const std::uint32_t y = bottom_up ? (height - 1u - file_y) : file_y;
        for (std::uint32_t x = 0; x < width; ++x) {
            image.at(x, y, 0) = row[3u * x + 2u]; // R
            image.at(x, y, 1) = row[3u * x + 1u]; // G
            image.at(x, y, 2) = row[3u * x + 0u]; // B
        }
    }
    return image;
}

void save_bmp(const Image& image, const std::string& path) {
    if (image.channels != 3) {
        throw std::invalid_argument("BMP writer expects a 3-channel RGB image");
    }

    const std::uint64_t stride = bmp_row_stride(image.width);
    const std::uint64_t pixel_bytes = stride * image.height;
    const std::uint64_t file_size = 54u + pixel_bytes;

    if (file_size > std::numeric_limits<std::uint32_t>::max()) {
        throw std::overflow_error("BMP file size exceeds 32-bit limit");
    }

    std::ofstream out(path, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Cannot open output BMP file: " + path);
    }

    write16(out, 0x4D42);
    write32(out, static_cast<std::uint32_t>(file_size));
    write16(out, 0);
    write16(out, 0);
    write32(out, 54);

    write32(out, 40);
    write32(out, image.width);
    write32(out, image.height);
    write16(out, 1);
    write16(out, 24);
    write32(out, 0);
    write32(out, static_cast<std::uint32_t>(pixel_bytes));
    write32(out, 2835);
    write32(out, 2835);
    write32(out, 0);
    write32(out, 0);

    for (std::uint32_t y = image.height; y-- > 0;) {
        for (std::uint32_t x = 0; x < image.width; ++x) {
            out.put(static_cast<char>(image.at(x, y, 2))); // B
            out.put(static_cast<char>(image.at(x, y, 1))); // G
            out.put(static_cast<char>(image.at(x, y, 0))); // R
        }
        for (std::uint64_t pad = static_cast<std::uint64_t>(image.width) * 3ULL; pad < stride; ++pad) {
            out.put(0);
        }
    }

    if (!out) {
        throw std::runtime_error("Failed while writing BMP file");
    }
}

}