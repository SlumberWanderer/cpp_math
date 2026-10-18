#include "tridiagonal-app.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        process_linear_by_tridiagonal();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}