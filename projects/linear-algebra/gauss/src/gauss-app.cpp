#include "gauss-app.hpp"
#include <iostream>
#include <fstream>

namespace {

void run_calculations(std::ostream& os)
{
    using std::endl;
    using linear_algebra::RectangleMatrix;
    using linear_algebra::Vector;
    using linear_algebra::gauss::PartialGaussResolver;
    using linear_algebra::gauss::PartialGaussDeterminantResolver;
    using linear_algebra::gauss::PartialGaussInverseMatrixResolver;

    constexpr double raw_m[4][4] = {
        { -6.4487, -49.2800,  10.1556,   7.6665},
        {-27.7668, -49.5715, -41.7735,  35.5074},
        { 31.1055,  19.1492,   3.0142, -35.7218},
        { 22.7898, -46.8907,  14.4522, -41.4534},
    };

    const RectangleMatrix coefficients = raw_m;
    const Vector rhs { 44.7878, -22.5032, 48.1769, -20.7736 };

    os << coefficients << endl;
    os << rhs << endl;

    const PartialGaussResolver system_resolver;
    const PartialGaussDeterminantResolver determinant_resolver;
    const PartialGaussInverseMatrixResolver inverse_resolver;

    const auto solution = system_resolver.resolve(coefficients, rhs);
    os << endl << "Solution:" << solution << endl;

    os << endl << "Validate:" << coefficients * solution << endl;

    os << endl << "Deviate:" << system_resolver.deviate(coefficients, solution, rhs) << endl;

    os << endl << "Determinant:" << determinant_resolver.determinant(coefficients) << endl;

    const auto inverse_m = inverse_resolver.inverse(coefficients);
    os << "Inverse matrix:" << endl << inverse_m << endl;

    os << "Product of source and inverse matrix from left to right or right to left should produce identity matrix" << endl;
    const auto identity = coefficients * inverse_m;
    os << identity << endl;
}
}

void process_linear_by_gauss()
{
    run_calculations(std::cout);

    std::ofstream file("gauss_output.txt");
    if (file.is_open()) {
        run_calculations(file); 
        std::cout << "\nSaved to gauss_output.txt" << std::endl;
    } else {
        std::cerr << "\nError: Cannot open file" << std::endl;
    }
}