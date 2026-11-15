#include "lagrange-app.hpp"
#include "lagrange.hpp"
#include "plane2d.hpp"
#include "plotter.hpp"
#include <iomanip>
#include <iostream>

void process_lagrange_interpolation() {
    using interpolation::lagrange::basis_polynomial;
    using interpolation::lagrange::evaluate;
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

    std::cout << "Task 1: Lagrange Polynomial Interpolation (Variant 1)\n";
    std::cout << "Target point x0 = " << x0 << "\n\n";

    std::cout << std::left << std::setw(6) << "i"
              << std::right << std::setw(12) << "x_i"
              << std::setw(12) << "y_i"
              << std::setw(16) << "l_i(x0)"
              << std::setw(16) << "y_i * l_i(x0)\n";

    std::cout << std::fixed << std::setprecision(6);
    for (std::size_t i = 0; i < nodes.size(); ++i) {
        const double li = basis_polynomial(nodes, i, x0);
        const double term = nodes[i].y * li;
        std::cout << std::left << std::setw(6) << i
                  << std::right << std::setw(12) << nodes[i].x
                  << std::setw(12) << nodes[i].y
                  << std::setw(16) << li
                  << std::setw(16) << term << "\n";
    }

    const double y0 = evaluate(nodes, x0);
    std::cout << "\nInterpolated value L_7(" << x0 << ") = " << y0 << "\n";

    const auto left_p = plane2d::left_point(nodes);
    const auto right_p = plane2d::right_point(nodes);
    const Points curve_pts = plane2d::tabulate(
        [&nodes](double x) { return evaluate(nodes, x); },
        left_p.x,
        right_p.x,
        200
    );

    Plot plot("Task 1: Lagrange Interpolation (Variant 1)");
    plot.set_labels("x", "y")
        .add_line(curve_pts, "Lagrange L_7(x)", 2)
        .add_points(nodes, "Source nodes")
        .add_points({{x0, y0}}, "x0 = 1.51");

    const Plotter plotter;
    plotter.save(plot, "task1_lagrange.png");
    std::cout << "Plot saved to task1_lagrange.png" << std::endl;
}