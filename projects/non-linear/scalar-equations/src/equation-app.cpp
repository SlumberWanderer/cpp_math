#include "equation-all.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

void print_row(const std::string& method_name,
               const equation::core::Result& result,
               double residual) {
    std::cout << std::left  << std::setw(32) << method_name
              << std::right << std::setw(14) << result.iterations()
              << std::setw(18) << std::fixed << std::setprecision(4) << result.value()
              << std::setw(18) << std::fixed << std::setprecision(4) << residual
              << "\n";
}

} 

void process_nonlinear_equations() {
    using namespace equation;

    const double tolerance = 0.01;
    const core::Range range{0.2, 0.5};

    const auto func = [](double x) {
        return std::sqrt(x) - std::cos(std::cos(0.387 * x));
    };

    const auto d_func = [](double x) {
        const double inner = 0.387 * x;
        return 0.5 / std::sqrt(x) - std::sin(std::cos(inner)) * std::sin(inner) * 0.387;
    };

    const auto phi = [](double x) {
        const double c = std::cos(std::cos(0.387 * x));
        return c * c;
    };

    std::cout << "Task 1: Nonlinear Equation Root Finding\n";
    std::cout << "Equation: sqrt(x) - cos(cos(0.387x)) = 0\n";
    std::cout << "Interval: " << range << ", Tolerance: " << tolerance << "\n\n";

    std::cout << std::left  << std::setw(32) << "Method"
              << std::right << std::setw(14) << "Iterations"
              << std::setw(18) << "Root (x*)"
              << std::setw(18) << "Residual f(x*)\n";

    const bisect::BisectResolver bisect_resolver{tolerance};
    const iterative::IterativeResolver iterative_resolver{phi, tolerance};
    const newton::NewtonResolver newton_resolver{d_func, tolerance};
    const chord::ChordResolver chord_resolver{tolerance};
    const secant::SecantResolver secant_resolver{tolerance};

    const auto res_bisect = bisect_resolver.resolve(func, range);
    print_row("1. Bisection method", res_bisect, bisect_resolver.deviate(func, res_bisect.value()));

    const auto res_iter = iterative_resolver.resolve(func, range);
    print_row("2. Simple iteration method", res_iter, iterative_resolver.deviate(func, res_iter.value()));

    const auto res_newton = newton_resolver.resolve(func, range);
    print_row("3. Newton method", res_newton, newton_resolver.deviate(func, res_newton.value()));

    const auto res_chord = chord_resolver.resolve(func, range);
    print_row("4. Chord method", res_chord, chord_resolver.deviate(func, res_chord.value()));

    const auto res_secant = secant_resolver.resolve(func, range);
    print_row("5. Secant method", res_secant, secant_resolver.deviate(func, res_secant.value()));
}