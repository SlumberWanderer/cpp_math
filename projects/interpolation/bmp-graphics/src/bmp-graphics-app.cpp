#include "bmp-graphics-app.hpp"
#include "bmp.hpp"
#include "transforms.hpp"
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

numeric::graphics::Image make_synthetic_test_image(std::uint32_t width, std::uint32_t height) {
    using numeric::graphics::Image;
    using numeric::graphics::Rgb;
    using numeric::graphics::to_byte;

    Image img(width, height, 3);
    const double cx = width / 2.0;
    const double cy = height / 2.0;

    for (std::uint32_t y = 0; y < height; ++y) {
        for (std::uint32_t x = 0; x < width; ++x) {
            const double dx = x - cx;
            const double dy = y - cy;
            const double dist = std::sqrt(dx * dx + dy * dy);

            // High-frequency concentric rings + smooth color gradient
            const bool ring = (static_cast<int>(dist) % 4 < 2);
            const std::uint8_t r = to_byte(255.0 * x / static_cast<double>(width - 1));
            const std::uint8_t g = to_byte(255.0 * y / static_cast<double>(height - 1));
            const std::uint8_t b = ring ? 230 : 30;
            img.set_rgb(x, y, Rgb{r, g, b});
        }
    }
    return img;
}

} 

void process_bmp_graphics() {
    using namespace numeric::graphics;

    std::cout << "Task 5: BMP Structure and 2D Image Interpolation\n\n";

    const Image generated = make_synthetic_test_image(65, 64);
    const std::string src_path = "task5_source.bmp";
    save_bmp(generated, src_path);

    const Image loaded = load_bmp(src_path);
    std::cout << "1. Loaded 24-bit BMP '" << src_path << "' ("
              << loaded.width << "x" << loaded.height << ", channels="
              << static_cast<int>(loaded.channels) << ")\n";

    constexpr std::array<std::pair<std::uint32_t, std::uint32_t>, 4> probe_coords{{
        {0, 0}, {16, 16}, {32, 32}, {64, 63}
    }};

    for (const auto& [px, py] : probe_coords) {
        const Rgb c = loaded.get_rgb(px, py);
        std::cout << "   Pixel (" << std::setw(2) << px << ", " << std::setw(2) << py << ") -> "
                  << "R=" << std::setw(3) << static_cast<int>(c.r) << ", "
                  << "G=" << std::setw(3) << static_cast<int>(c.g) << ", "
                  << "B=" << std::setw(3) << static_cast<int>(c.b) << "\n";
    }

    std::cout << "\n2. BMP Pixel Data Area vs Raw RGB Buffer Size (Height = 3 and 64):\n";
    std::cout << std::right << std::setw(8) << "Width"
              << std::setw(8) << "Height"
              << std::setw(14) << "Useful/Row"
              << std::setw(12) << "Stride"
              << std::setw(10) << "Pad/Row"
              << std::setw(14) << "Raw Bytes"
              << std::setw(14) << "BMP Pixels"
              << std::setw(14) << "Total File\n";

    const std::array<std::pair<std::uint32_t, std::uint32_t>, 7> layouts{{
        {5, 3}, {6, 3}, {7, 3}, {8, 3}, {65, 64}, {127, 64}, {128, 64}
    }};

    for (const auto& [w, h] : layouts) {
        const auto info = analyze_bmp_layout(w, h);
        std::cout << std::setw(8) << info.width
                  << std::setw(8) << info.height
                  << std::setw(14) << info.row_useful_bytes
                  << std::setw(12) << info.row_stride_bytes
                  << std::setw(10) << info.padding_per_row
                  << std::setw(14) << info.raw_memory_bytes
                  << std::setw(14) << info.bmp_pixel_data_bytes
                  << std::setw(14) << info.total_file_bytes << "\n";
    }

    constexpr std::uint32_t up_w = 160;
    constexpr std::uint32_t up_h = 160;
    const Image scaled_nearest = resize_image(loaded, up_w, up_h, InterpolationMode::Nearest);
    const Image scaled_bilinear = resize_image(loaded, up_w, up_h, InterpolationMode::Bilinear);
    const Image scaled_bicubic = resize_image(loaded, up_w, up_h, InterpolationMode::Bicubic);

    save_bmp(scaled_nearest, "task5_scale_nearest.bmp");
    save_bmp(scaled_bilinear, "task5_scale_bilinear.bmp");
    save_bmp(scaled_bicubic, "task5_scale_bicubic.bmp");

    std::cout << "\n3. Resizing from " << loaded.width << "x" << loaded.height
              << " to " << up_w << "x" << up_h << ":\n";
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "   Mean abs diff (Nearest vs Bilinear)  = "
              << mean_absolute_diff(scaled_nearest, scaled_bilinear) << "\n";
    std::cout << "   Mean abs diff (Bilinear vs Bicubic)  = "
              << mean_absolute_diff(scaled_bilinear, scaled_bicubic) << "\n";

    constexpr double angle_deg = 35.0;
    constexpr Rgb bg_color{20, 30, 45};
    const Image rotated = rotate_image(loaded, angle_deg, bg_color, InterpolationMode::Bicubic);
    save_bmp(rotated, "task5_rotated_35deg.bmp");
    std::cout << "\n4. Rotated image by " << angle_deg
              << " deg with background RGB(20, 30, 45) -> saved to task5_rotated_35deg.bmp\n";

    Image rgba_test(32, 32, 4);
    for (std::uint32_t y = 0; y < 32; ++y) {
        for (std::uint32_t x = 0; x < 32; ++x) {
            const double dx = x - 15.5;
            const double dy = y - 15.5;
            if (dx * dx + dy * dy <= 100.0) {
                rgba_test.set_rgba(x, y, Rgba{255, 40, 20, 255}); 
            } else {
                rgba_test.set_rgba(x, y, Rgba{0, 0, 0, 0});      
            }
        }
    }

    const Rgba edge_direct = sample_rgba_direct(rgba_test, 5.5, 15.5);
    const Rgba edge_premul = sample_rgba_premultiplied(rgba_test, 5.5, 15.5);

    const Image rgba_up_direct = resize_rgba(rgba_test, 128, 128, false);
    const Image rgba_up_premul = resize_rgba(rgba_test, 128, 128, true);

    save_bmp(composite_over_background(rgba_up_direct, Rgb{255, 255, 255}), "task5_alpha_direct.bmp");
    save_bmp(composite_over_background(rgba_up_premul, Rgb{255, 255, 255}), "task5_alpha_premultiplied.bmp");

    std::cout << "\n5. Transparency Interpolation at Edge (Opaque Red RGB(255,40,20,A=255) next to Transparent RGB(0,0,0,A=0)):\n";
    std::cout << "   Direct RGBA interpolation  : R=" << static_cast<int>(edge_direct.r)
              << ", G=" << static_cast<int>(edge_direct.g)
              << ", B=" << static_cast<int>(edge_direct.b)
              << ", A=" << static_cast<int>(edge_direct.a) << " (dark halo artifact)\n";
    std::cout << "   Premultiplied Alpha interp : R=" << static_cast<int>(edge_premul.r)
              << ", G=" << static_cast<int>(edge_premul.g)
              << ", B=" << static_cast<int>(edge_premul.b)
              << ", A=" << static_cast<int>(edge_premul.a) << " (pure edge color preserved)\n";

    const Image hi_freq = make_synthetic_test_image(128, 128);
    std::cout << "\n6. Antialiasing Filter Effect Before Downsampling (128x128 source):\n";
    std::cout << std::right << std::setw(10) << "Factor"
              << std::setw(16) << "Target Size"
              << std::setw(24) << "Mean Diff (NoAA vs AA)\n";

    for (const std::uint32_t factor : {2u, 4u, 8u}) {
        const Image no_aa = downsample_image(hi_freq, factor, false, InterpolationMode::Bilinear);
        const Image with_aa = downsample_image(hi_freq, factor, true, InterpolationMode::Bilinear);

        const std::string tag = std::to_string(factor) + "x";
        save_bmp(no_aa, "task5_down_" + tag + "_noaa.bmp");
        save_bmp(with_aa, "task5_down_" + tag + "_aa.bmp");

        const std::string dim_str = std::to_string(no_aa.width) + "x" + std::to_string(no_aa.height);
        std::cout << std::setw(9) << factor << "x"
                  << std::setw(16) << dim_str
                  << std::setw(24) << mean_absolute_diff(no_aa, with_aa) << "\n";
    }
}