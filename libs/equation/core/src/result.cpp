#include "result.hpp"

namespace equation::core {

std::ostream& operator<<(std::ostream& os, const Result& result) {
    os << "{status: " << (result.is_defined() ? "DEFINED" : "UNDEFINED")
       << ", root: " << result.value()
       << ", iterations: " << result.iterations() << "}";
    return os;
}

}