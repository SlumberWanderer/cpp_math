#include "non-linear-all.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

void print_system_row(const std::string& method_name,
                      const nonlinear::core::IterationResult& res) {
    std::cout << std::left  << std::setw(30) << method_name
              << std::right << std::setw(12) << res.iterations
              << std::setw(14) << std::fixed << std::setprecision(4) << res.solution[0]
              << std::setw(14) << std::fixed << std::setprecision(4) << res.solution[1]
              << std::setw(18) << std::fixed << std::setprecision(4) << res.residual_norm
              << "\n";
}

} 

void process_nonlinear_system() {
    using nonlinear::core::IterationOptions;
    using nonlinear::core::Matrix;
    using nonlinear::core::NonlinearSystem;
    using nonlinear::core::Vector;

    NonlinearSystem system;
    system.dimension = 2;

    system.function = [](const Vector& v) {
        const double x = v[0];
        const double y = v[1];
        return Vector{
            2.0 * x + std::cos(y) - 2.0,
            std::sin(x + y) - y - 1.2
        };
    };

    system.custom_jacobian = [](const Vector& v) {
        const double x = v[0];
        const double y = v[1];
        Matrix J(2, 2);
        J(0, 0) = 2.0;
        J(0, 1) = -std::sin(y);
        J(1, 0) = std::cos(x + y);
        J(1, 1) = std::cos(x + y) - 1.0;
        return J;
    };

    const auto phi = [](const Vector& v) {
        const double x = v[0];
        const double y = v[1];
        return Vector{
            1.0 - 0.5 * std::cos(y),
            std::sin(x + y) - 1.2
        };
    };

    const Vector init{1.1, -1.8};
    IterationOptions options;
    options.tolerance = 0.01;

    std::cout << "Task 2: System of Nonlinear Equations\n";
    std::cout << "System:\n";
    std::cout << "  2x + cos(y) - 2 = 0\n";
    std::cout << "  sin(x + y) - y - 1.2 = 0\n";
    std::cout << "Initial: [1.1, -1.8], Tolerance: " << options.tolerance << "\n\n";

    std::cout << std::left  << std::setw(30) << "Method"
              << std::right << std::setw(12) << "Iterations"
              << std::setw(14) << "Root (x*)"
              << std::setw(14) << "Root (y*)"
              << std::setw(18) << "Residual\n";

    const auto res_iter = nonlinear::iterative::resolve(system, phi, init, options);
    print_system_row("1. Simple iteration method", res_iter);

    const auto res_newton = nonlinear::newton::resolve(system, init, options);
    print_system_row("2. Newton method", res_newton);
}