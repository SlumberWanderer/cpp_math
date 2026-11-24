#include "differentiation-app.hpp"
#include "lagrange-differentiator.hpp"
#include "plane2d.hpp"
#include "plotter.hpp"
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>

void process_differentiation() {
    using interpolation::differentiation::LagrangeDifferentiator;
    using plane2d::Points;
    using plot2d::Plot;
    using plot2d::Plotter;

    const Points task1_nodes{
        {0.0, 1.00},
        {0.4, 1.49},
        {1.1, 2.65},
        {1.7, 4.13},
        {2.5, 7.25}
    };
    constexpr std::array<double, 3> task1_pts{0.3, 1.3, 2.2};

    const LagrangeDifferentiator diff1(task1_nodes);

    std::cout << "Task 1: Numerical Differentiation on Non-Uniform Grid (Variant 1)\n\n";
    std::cout << std::right << std::setw(10) << "t"
              << std::setw(16) << "L'(t)"
              << std::setw(16) << "L''(t)\n";
    std::cout << std::fixed << std::setprecision(6);

    for (const double t : task1_pts) {
        const auto d = diff1.evaluate(t);
        std::cout << std::setw(10) << std::setprecision(2) << t
                  << std::setprecision(6)
                  << std::setw(16) << d.first
                  << std::setw(16) << d.second << "\n";
    }

    const auto func = [](double x) {
        return std::exp(x / 2.0) * std::sin(2.0 * x) + x * x;
    };
    const auto exact_d1 = [](double x) {
        return std::exp(x / 2.0) * (0.5 * std::sin(2.0 * x) + 2.0 * std::cos(2.0 * x)) + 2.0 * x;
    };
    const auto exact_d2 = [](double x) {
        return std::exp(x / 2.0) * (-3.75 * std::sin(2.0 * x) + 2.0 * std::cos(2.0 * x)) + 2.0;
    };

    constexpr double a = 0.0;
    constexpr double b = 2.0;
    constexpr double h = 0.25;
    constexpr std::array<double, 3> task2_pts{0.35, 1.15, 1.80};

    const Points task2_nodes = plane2d::tabulate_by_step(func, a, b, h);
    const LagrangeDifferentiator diff2(task2_nodes);

    std::cout << "\nTask 2: Accuracy of Lagrange Polynomial Derivatives (Variant 1)\n";
    std::cout << "Function: f(x) = exp(x/2)*sin(2x) + x^2 on [0; 2], h = 0.25 ("
              << task2_nodes.size() << " nodes)\n\n";

    std::cout << "Tabulated nodes:\n";
    std::cout << std::right << std::setw(6) << "i"
              << std::setw(10) << "x_i"
              << std::setw(14) << "y_i\n";
    for (std::size_t i = 0; i < task2_nodes.size(); ++i) {
        std::cout << std::setw(6) << i
                  << std::setw(10) << std::setprecision(2) << task2_nodes[i].x
                  << std::setw(14) << std::setprecision(6) << task2_nodes[i].y << "\n";
    }

    std::cout << "\nComparison with analytical derivatives:\n";
    std::cout << std::right << std::setw(8) << "t"
              << std::setw(12) << "L'(t)"
              << std::setw(12) << "f'(t)"
              << std::setw(12) << "Delta1"
              << std::setw(11) << "delta1,%"
              << std::setw(12) << "L''(t)"
              << std::setw(12) << "f''(t)"
              << std::setw(12) << "Delta2"
              << std::setw(11) << "delta2,%\n";

    for (const double t : task2_pts) {
        const auto num = diff2.evaluate(t);
        const double f1 = exact_d1(t);
        const double f2 = exact_d2(t);
        const double abs1 = std::abs(f1 - num.first);
        const double rel1 = (abs1 / std::abs(f1)) * 100.0;
        const double abs2 = std::abs(f2 - num.second);
        const double rel2 = (abs2 / std::abs(f2)) * 100.0;

        std::cout << std::setw(8) << std::setprecision(2) << t
                  << std::setprecision(6)
                  << std::setw(12) << num.first
                  << std::setw(12) << f1
                  << std::setw(12) << abs1
                  << std::setw(11) << std::setprecision(4) << rel1
                  << std::setprecision(6)
                  << std::setw(12) << num.second
                  << std::setw(12) << f2
                  << std::setw(12) << abs2
                  << std::setw(11) << std::setprecision(4) << rel2 << "\n";
    }

    const Plotter plotter;

    const auto exact_d1_curve = plane2d::tabulate(exact_d1, a, b, 200);
    const auto num_d1_curve = plane2d::tabulate(
        [&diff2](double x) { return diff2.evaluate(x).first; }, a, b, 200
    );
    Plot plot_d1("Task 2: First Derivative df/dx vs dL_8/dx (Variant 1)");
    plot_d1.set_labels("x", "dy/dx")
           .add_line(exact_d1_curve, "Exact df/dx", 3)
           .add_line(num_d1_curve, "Lagrange dL_8/dx", 2);
    plotter.save(plot_d1, "task2_first_derivative.png");

    const auto exact_d2_curve = plane2d::tabulate(exact_d2, a, b, 200);
    const auto num_d2_curve = plane2d::tabulate(
        [&diff2](double x) { return diff2.evaluate(x).second; }, a, b, 200
    );
    Plot plot_d2("Task 2: Second Derivative d2f/dx2 vs d2L_8/dx2 (Variant 1)");
    plot_d2.set_labels("x", "d2y/dx2")
           .add_line(exact_d2_curve, "Exact d2f/dx2", 3)
           .add_line(num_d2_curve, "Lagrange d2L_8/dx2", 2);
    plotter.save(plot_d2, "task2_second_derivative.png");

    std::cout << "\nPlots saved to task2_first_derivative.png and task2_second_derivative.png" << std::endl;
}