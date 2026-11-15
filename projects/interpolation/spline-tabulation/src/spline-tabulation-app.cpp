#include "spline-tabulation-app.hpp"
#include "cubic-spline.hpp"
#include "plane2d.hpp"
#include "plotter.hpp"

#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

void process_spline_tabulation() {
    using interpolation::spline::cubic::CubicSpline;
    using plane2d::Points;
    using plot2d::Plot;
    using plot2d::Plotter;

    const auto func = [](double x) {
        return std::exp(0.3 * x) * std::cos(1.2 * x) + std::log(x + 4.0);
    };

    constexpr double a = 0.0;
    constexpr double b = 2.0;
    constexpr std::array<double, 3> steps{0.5, 0.333, 0.25};
    constexpr std::array<double, 3> interior_pts{0.167, 0.833, 1.833};
    constexpr std::array<double, 3> exterior_pts{-0.333, 2.333, 2.667};

    const Points nodes_h1 = plane2d::tabulate_by_step(func, a, b, steps[0]);
    const Points nodes_h2 = plane2d::tabulate_by_step(func, a, b, steps[1]);
    const Points nodes_h3 = plane2d::tabulate_by_step(func, a, b, steps[2]);

    const CubicSpline spline_h1(nodes_h1);
    const CubicSpline spline_h2(nodes_h2);
    const CubicSpline spline_h3(nodes_h3);

    std::cout << "Task 4: Function Tabulation and Natural Cubic Spline (Variant 1)\n";
    std::cout << "Function: f(x) = exp(0.3x)*cos(1.2x) + ln(x + 4) on [0; 2]\n";
    std::cout << "Steps: h1 = 0.5 (" << nodes_h1.size() << " nodes), "
              << "h2 = 0.333 (" << nodes_h2.size() << " nodes), "
              << "h3 = 0.25 (" << nodes_h3.size() << " nodes)\n\n";

    std::cout << "1. Interior non-node points (interpolation on [0; 2]):\n";
    std::cout << std::right << std::setw(10) << "x"
              << std::setw(14) << "Exact f(x)"
              << std::setw(14) << "S_h1(x)"
              << std::setw(12) << "|Err h1|"
              << std::setw(14) << "S_h2(x)"
              << std::setw(12) << "|Err h2|"
              << std::setw(14) << "S_h3(x)"
              << std::setw(12) << "|Err h3|\n";

    std::cout << std::fixed << std::setprecision(6);
    for (const double x : interior_pts) {
        const double exact = func(x);
        const double s1 = spline_h1.evaluate(x);
        const double s2 = spline_h2.evaluate(x);
        const double s3 = spline_h3.evaluate(x);

        std::cout << std::setw(10) << x
                  << std::setw(14) << exact
                  << std::setw(14) << s1
                  << std::setw(12) << std::abs(exact - s1)
                  << std::setw(14) << s2
                  << std::setw(12) << std::abs(exact - s2)
                  << std::setw(14) << s3
                  << std::setw(12) << std::abs(exact - s3) << "\n";
    }

    std::cout << "\n2. Exterior points (extrapolation outside [0; 2]):\n";
    std::cout << std::right << std::setw(10) << "x"
              << std::setw(14) << "Exact f(x)"
              << std::setw(14) << "S_h1(x)"
              << std::setw(12) << "|Err h1|"
              << std::setw(14) << "S_h2(x)"
              << std::setw(12) << "|Err h2|"
              << std::setw(14) << "S_h3(x)"
              << std::setw(12) << "|Err h3|\n";

    for (const double x : exterior_pts) {
        const double exact = func(x);
        const double s1 = spline_h1.evaluate_clamped(x);
        const double s2 = spline_h2.evaluate_clamped(x);
        const double s3 = spline_h3.evaluate_clamped(x);

        std::cout << std::setw(10) << x
                  << std::setw(14) << exact
                  << std::setw(14) << s1
                  << std::setw(12) << std::abs(exact - s1)
                  << std::setw(14) << s2
                  << std::setw(12) << std::abs(exact - s2)
                  << std::setw(14) << s3
                  << std::setw(12) << std::abs(exact - s3) << "\n";
    }

    const Plotter plotter;

    const auto exact_in = plane2d::tabulate(func, a, b, 200);
    const auto s1_in = plane2d::tabulate([&spline_h1](double x) { return spline_h1.evaluate(x); }, a, b, 200);
    const auto s2_in = plane2d::tabulate([&spline_h2](double x) { return spline_h2.evaluate(x); }, a, b, 200);
    const auto s3_in = plane2d::tabulate([&spline_h3](double x) { return spline_h3.evaluate(x); }, a, b, 200);

    Plot plot_in("Task 4: Function and Natural Cubic Splines on [0; 2]");
    plot_in.set_labels("x", "y")
           .add_line(exact_in, "Exact f(x)", 3)
           .add_line(s1_in, "Spline h1 = 0.5", 2)
           .add_line(s2_in, "Spline h2 = 0.333", 2)
           .add_line(s3_in, "Spline h3 = 0.25", 2)
           .add_points(nodes_h1, "Nodes (h1 = 0.5)");

    plotter.save(plot_in, "task4_spline_tabulation.png");

    constexpr double ext_a = -0.4;
    constexpr double ext_b = 2.7;
    const auto exact_ext = plane2d::tabulate(func, ext_a, ext_b, 250);
    const auto s1_ext = plane2d::tabulate([&spline_h1](double x) { return spline_h1.evaluate_clamped(x); }, ext_a, ext_b, 250);
    const auto s3_ext = plane2d::tabulate([&spline_h3](double x) { return spline_h3.evaluate_clamped(x); }, ext_a, ext_b, 250);

    Plot plot_ext("Task 4: Spline Extrapolation Outside [0; 2]");
    plot_ext.set_labels("x", "y")
            .add_line(exact_ext, "Exact f(x)", 3)
            .add_line(s1_ext, "Spline h1 = 0.5", 2)
            .add_line(s3_ext, "Spline h3 = 0.25", 2)
            .add_points(nodes_h3, "Tabulation nodes [0; 2]");

    plotter.save(plot_ext, "task4_spline_extrapolation.png");
    std::cout << "\nPlots saved to task4_spline_tabulation.png and task4_spline_extrapolation.png"<< std::endl;
}