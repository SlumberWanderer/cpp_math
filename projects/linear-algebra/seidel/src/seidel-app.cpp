#include "seidel-app.hpp"
#include "rectangle-matrix.hpp"
#include "vector.hpp"
#include "seidel.hpp"

#include <iostream>
#include <fstream>

namespace {

void run_calculations(std::ostream& os) {
    using std::endl;
    using linear_algebra::RectangleMatrix;
    using linear_algebra::Vector;
    using linear_algebra::seidel::SimpleIterationResolver;
    using linear_algebra::seidel::GaussSeidelResolver;

    const double raw_m[4][4] = {
        {  -6.4487, -49.28,    10.1556,   7.6665 },
        { -27.7668, -49.5715, -41.7735,  35.5074 },
        {  31.1055,  19.1492,   3.0142, -35.7218 },
        {  22.7898, -46.8907,  14.4522, -41.4534 }
    };
    const RectangleMatrix matrix = raw_m;
    const Vector rhs { 44.7878, -22.5032, 48.1769, -20.7736 };

    os << "Matrix:" << endl << matrix << endl;
    os << "Vector:" << endl << rhs << endl;

    const SimpleIterationResolver simple_resolver(1e-7, 200000);
    const Vector x_simple = simple_resolver.resolve(matrix, rhs);

    os << endl << "Simple iteration solution:" << endl << x_simple << endl;
    os << endl << "Simple iteration validate:" << endl << (matrix * x_simple) << endl;
    os << endl << "Simple iteration deviate:" << endl << simple_resolver.deviate(matrix, x_simple, rhs) << endl;

    const GaussSeidelResolver seidel_resolver(1e-7, 200000);
    const Vector x_seidel = seidel_resolver.resolve(matrix, rhs);

    os << endl << "Seidel solution:" << endl << x_seidel << endl;
    os << endl << "Seidel validate:" << endl << (matrix * x_seidel) << endl;
    os << endl << "Seidel deviate:" << endl << seidel_resolver.deviate(matrix, x_seidel, rhs) << endl;
}

}

void process_linear_by_seidel() {
    run_calculations(std::cout);

    std::ofstream file("seidel_output.txt");
    if (file.is_open()) {
        run_calculations(file);
        std::cout << "\nSaved to seidel_output.txt\n";
    } else {
        std::cerr << "\nError: Cannot open file\n";
    }
}