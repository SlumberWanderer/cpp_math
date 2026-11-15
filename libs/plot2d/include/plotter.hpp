#pragma once

#include <string>
#include "plot.hpp"

namespace plot2d {

class Plotter {
public:
    Plotter() noexcept = default;

    static void validate(const Plot& plot);

    void save(const Plot& plot, const std::string& filename) const;
    void show(const Plot& plot) const;
};

}