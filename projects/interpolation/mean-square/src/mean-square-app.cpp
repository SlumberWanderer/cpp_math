#include "mean-square-app.hpp"
#include "mean-square.hpp"
#include "plane2d.hpp"
#include "plotter.hpp"

#include <iomanip>
#include <iostream>

void process_mean_square() {
    using interpolation::mean_square::MeanSquare;
    using plane2d::Points;
    using plot2d::Plot;
    using plot2d::Plotter;

    const Points nodes{
        {-3.484,  0.696},
        {-3.310,  0.089},
        {-0.748, -0.427},
        { 0.171, -0.053},
        { 0.724, -0.551},
        { 0.990, -0.897},
        { 2.350, -1.422},
        { 2.515, -1.172}
    };

    const MeanSquare ms(nodes);
    const auto p1 = ms.approx(1);
    const auto p2 = ms.approx(2);
    const auto p3 = ms.approx(3);

    std::cout << "Task 2: Least Squares Approximation (Variant 1)\n\n";
    std::cout << "Table of polynomial coefficients and error metrics:\n";
    std::cout << std::left << std::setw(16) << "Approximation"
              << std::right << std::setw(12) << "c0"
              << std::setw(12) << "c1"
              << std::setw(12) << "c2"
              << std::setw(12) << "c3"
              << std::setw(14) << "SSE"
              << std::setw(12) << "RMSE\n";

    std::cout << std::fixed << std::setprecision(6);
    std::cout << std::left << std::setw(16) << "Linear (m=1)"
              << std::right << std::setw(12) << p1.coefficient(0)
              << std::setw(12) << p1.coefficient(1)
              << std::setw(12) << "-"
              << std::setw(12) << "-"
              << std::setw(14) << ms.squared_error(p1)
              << std::setw(12) << ms.rmse(p1) << "\n";

    std::cout << std::left << std::setw(16) << "Quadratic (m=2)"
              << std::right << std::setw(12) << p2.coefficient(0)
              << std::setw(12) << p2.coefficient(1)
              << std::setw(12) << p2.coefficient(2)
              << std::setw(12) << "-"
              << std::setw(14) << ms.squared_error(p2)
              << std::setw(12) << ms.rmse(p2) << "\n";

    std::cout << std::left << std::setw(16) << "Cubic (m=3)"
              << std::right << std::setw(12) << p3.coefficient(0)
              << std::setw(12) << p3.coefficient(1)
              << std::setw(12) << p3.coefficient(2)
              << std::setw(12) << p3.coefficient(3)
              << std::setw(14) << ms.squared_error(p3)
              << std::setw(12) << ms.rmse(p3) << "\n";

    std::cout << "\nExponential fit status: "
              << (ms.has_only_positive_y()
                      ? "Applicable"
                      : "Not applicable (dataset contains negative y values)")
              << "\n\n";

    std::cout << "Pointwise comparison at nodes:\n";
    std::cout << std::right << std::setw(10) << "x_i"
              << std::setw(12) << "y_i"
              << std::setw(14) << "P1(x_i)"
              << std::setw(14) << "P2(x_i)"
              << std::setw(14) << "P3(x_i)\n";

    for (const auto& [x, y] : nodes) {
        std::cout << std::setw(10) << x
                  << std::setw(12) << y
                  << std::setw(14) << p1(x)
                  << std::setw(14) << p2(x)
                  << std::setw(14) << p3(x) << "\n";
    }

    const auto left_p = plane2d::left_point(nodes);
    const auto right_p = plane2d::right_point(nodes);
    const double a = left_p.x - 0.5;
    const double b = right_p.x + 0.5;

    const auto pts_p1 = plane2d::tabulate([&p1](double x) { return p1(x); }, a, b, 200);
    const auto pts_p2 = plane2d::tabulate([&p2](double x) { return p2(x); }, a, b, 200);
    const auto pts_p3 = plane2d::tabulate([&p3](double x) { return p3(x); }, a, b, 200);

    Plot plot("Task 2: Least Squares Approximation (Variant 1)");
    plot.set_labels("x", "y")
        .add_points(nodes, "Source points")
        .add_line(pts_p1, "Linear P1(x)", 2)
        .add_line(pts_p2, "Quadratic P2(x)", 2)
        .add_line(pts_p3, "Cubic P3(x)", 2);

    const Plotter plotter;
    plotter.save(plot, "task2_mean_square.png");
    std::cout << "\nPlot saved to task2_mean_square.png" << std::endl;
}