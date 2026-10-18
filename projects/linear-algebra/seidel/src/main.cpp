#include "seidel-app.hpp"
#include <iostream>
#include <exception>

int main() {
    try {
        process_linear_by_seidel();
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}