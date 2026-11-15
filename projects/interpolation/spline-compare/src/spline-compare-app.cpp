#include "spline-compare-app.hpp"
#include "cubic-spline.hpp"
#include "lagrange.hpp"
#include "plane2d.hpp"
#include "plotter.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

void process_spline_comparison() {
    using interpolation::lagrange::evaluate;
    using interpolation::spline::cubic::CubicSpline;
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
    const double x0 = 1.51;

    const CubicSpline spline(nodes);
    const double y_spline = spline.evaluate(x0);
    const double y_lagrange = evaluate(nodes, x0);

    std::cout << "Task 3: Cubic Spline vs Lagrange Polynomial (Variant 1)\n\n";
    std::cout << "Natural Cubic Spline segments S_i(x) = a_i + b_i(x - x_i) + c_i(x - x_i)^2 + d_i(x - x_i)^3:\n";
    std::cout << std::left << std::setw(6) << "Seg"
              << std::setw(22) << "Interval [x_i, x_{i+1}]"
              << std::right << std::setw(12) << "a_i"
              << std::setw(12) << "b_i"
              << std::setw(12) << "c_i"
              << std::setw(12) << "d_i\n";

    std::cout << std::fixed << std::setprecision(6);
    const auto& segments = spline.segments();
    for (std::size_t i = 0; i < segments.size(); ++i) {
        const auto& seg = segments[i];
        std::cout << std::left << std::setw(6) << i
                  << "[" << std::setw(7) << std::setprecision(3) << seg.start().point().x
                  << "; " << std::setw(7) << seg.end().point().x << "]   "
                  << std::right << std::setprecision(6)
                  << std::setw(12) << seg.get_coefficient(0)
                  << std::setw(12) << seg.get_coefficient(1)
                  << std::setw(12) << seg.get_coefficient(2)
                  << std::setw(12) << seg.get_coefficient(3) << "\n";
    }

    std::cout << "\nComparison at x0 = " << std::setprecision(2) << x0 << ":\n";
    std::cout << std::setprecision(6);
    std::cout << "  Cubic Spline S(1.51)      = " << y_spline << "\n";
    std::cout << "  Lagrange L_7(1.51)        = " << y_lagrange << "\n";
    std::cout << "  Absolute difference |S-L| = " << std::abs(y_spline - y_lagrange) << "\n";

    const auto left_p = plane2d::left_point(nodes);
    const auto right_p = plane2d::right_point(nodes);

    const auto spline_pts = plane2d::tabulate(
        [&spline](double x) { return spline.evaluate(x); },
        left_p.x, right_p.x, 250
    );
    const auto lagrange_pts = plane2d::tabulate(
        [&nodes](double x) { return evaluate(nodes, x); },
        left_p.x, right_p.x, 250
    );

    Plot plot("Task 3: Cubic Spline vs Lagrange L_7(x) (Variant 1)");
    plot.set_labels("x", "y")
        .add_points(nodes, "Nodes")
        .add_line(spline_pts, "Cubic Spline S(x)", 2)
        .add_line(lagrange_pts, "Lagrange L_7(x)", 2);

    const Plotter plotter;
    plotter.save(plot, "task3_spline_vs_lagrange.png");
    std::cout << "Plot saved to task3_spline_vs_lagrange.png"<< std::endl;
}