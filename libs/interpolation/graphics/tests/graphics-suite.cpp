#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "bmp.hpp"
#include "transforms.hpp"
#include <cstdio>

using namespace numeric::graphics;

TEST_CASE("BMP layout and padding calculation (Task 5.2)", "[graphics][bmp]") {
    const auto info = analyze_bmp_layout(5, 3);
    CHECK(info.row_useful_bytes == 15);
    CHECK(info.row_stride_bytes == 16);
    CHECK(info.padding_per_row == 1);
    CHECK(info.raw_memory_bytes == 45);
    CHECK(info.bmp_pixel_data_bytes == 48);
    CHECK(info.total_file_bytes == 102);
}

TEST_CASE("BMP round-trip save and load with padding (Task 5.1)", "[graphics][bmp]") {
    Image img(5, 3, 3);
    img.set_rgb(0, 0, Rgb{255, 0, 0});
    img.set_rgb(4, 2, Rgb{0, 128, 255});
    img.set_rgb(2, 1, Rgb{42, 99, 200});

    const std::string temp_file = "test_roundtrip_5x3.bmp";
    save_bmp(img, temp_file);

    const Image loaded = load_bmp(temp_file);
    std::remove(temp_file.c_str());

    REQUIRE(loaded.width == 5);
    REQUIRE(loaded.height == 3);
    CHECK(loaded.get_rgb(0, 0) == Rgb{255, 0, 0});
    CHECK(loaded.get_rgb(4, 2) == Rgb{0, 128, 255});
    CHECK(loaded.get_rgb(2, 1) == Rgb{42, 99, 200});
}

TEST_CASE("Image resizing and rotation (Task 5.3 & 5.4)", "[graphics][transforms]") {
    Image src(8, 8, 3);
    for (std::uint32_t y = 0; y < 8; ++y) {
        for (std::uint32_t x = 0; x < 8; ++x) {
            src.set_rgb(x, y, Rgb{100, 150, 200});
        }
    }

    for (auto mode : {InterpolationMode::Nearest, InterpolationMode::Bilinear, InterpolationMode::Bicubic}) {
        const Image scaled = resize_image(src, 16, 16, mode);
        REQUIRE(scaled.width == 16);
        REQUIRE(scaled.height == 16);
        CHECK(scaled.get_rgb(8, 8) == Rgb{100, 150, 200});
    }

    const Rgb bg{12, 34, 56};
    const Image rotated = rotate_image(src, 45.0, bg, InterpolationMode::Bilinear);
    CHECK(rotated.get_rgb(0, 0) == bg);
    CHECK(rotated.get_rgb(4, 4) == Rgb{100, 150, 200});
}

TEST_CASE("Premultiplied alpha eliminates dark color bleeding on transparent edges (Task 5.5)", "[graphics][alpha]") {
    Image rgba(2, 1, 4);
    rgba.set_rgba(0, 0, Rgba{255, 0, 0, 255});
    rgba.set_rgba(1, 0, Rgba{0, 0, 0, 0});

    const Rgba direct = sample_rgba_direct(rgba, 0.5, 0.0);
    const Rgba premul = sample_rgba_premultiplied(rgba, 0.5, 0.0);

    CHECK(direct.r == 128);
    CHECK(direct.a == 128);

    CHECK(premul.r == 255);
    CHECK(premul.a == 128);
}

TEST_CASE("Antialiasing filter before 2x, 4x, 8x downsampling (Task 5.6)", "[graphics][antialias]") {
    Image checkerboard(32, 32, 3);
    for (std::uint32_t y = 0; y < 32; ++y) {
        for (std::uint32_t x = 0; x < 32; ++x) {
            const std::uint8_t val = ((x + y) % 2 == 0) ? 255 : 0;
            checkerboard.set_rgb(x, y, Rgb{val, val, val});
        }
    }

    for (std::uint32_t factor : {2u, 4u, 8u}) {
        const Image aliased = downsample_image(checkerboard, factor, false, InterpolationMode::Nearest);
        const Image filtered = downsample_image(checkerboard, factor, true, InterpolationMode::Bilinear);
        REQUIRE(aliased.width == 32 / factor);
        REQUIRE(filtered.width == 32 / factor);
        CHECK(mean_absolute_diff(aliased, filtered) > 10.0);
    }
}