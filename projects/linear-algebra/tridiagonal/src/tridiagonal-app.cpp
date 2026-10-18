#include "tridiagonal-app.hpp"
#include "band-matrix.hpp"
#include "vector.hpp"
#include "tridiagonal-resolver.hpp"

#include <iostream>
#include <fstream>

namespace {

void run_calculations(std::ostream& os) {
    using std::endl;
    using linear_algebra::BandMatrix;
    using linear_algebra::Vector;
    using linear_algebra::band::TridiagonalResolver;

    const Vector a {   0.0,   -14.969,  21.05,  -35.269, -49.108 };
    const Vector b { -30.668,  32.284, -19.601,  48.851,   3.166 };
    const Vector c {   8.501, -32.587, -40.859, -38.092,   0.0   };
    const Vector d {  -3.793,  36.222,  27.966,  49.68,   11.149 };

    const BandMatrix matrix(a, b, c);

    os << "Tridiagonal Matrix A:" << endl << matrix << endl;
    os << "Right-hand side vector d:" << endl << d << endl;

    const TridiagonalResolver resolver;

    const Vector solution = resolver.resolve(matrix, d);
    os << endl << "Solution:" << endl << solution << endl;

    os << endl << "Validate:" << endl << (matrix * solution) << endl;

    os << endl << "Deviate:" << endl << resolver.deviate(matrix, solution, d) << endl;
}

}

void process_linear_by_tridiagonal() {
    run_calculations(std::cout);

    std::ofstream file("tridiagonal_output.txt");
    if (file.is_open()) {
        run_calculations(file);
        std::cout << "\nSaved to tridiagonal_output.txt" << std::endl;
    } else {
        std::cerr << "\nError: Cannot open file" << std::endl;
    }
}