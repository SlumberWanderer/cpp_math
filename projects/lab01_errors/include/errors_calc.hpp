#include <iostream>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::fabs;
using std::floor;
using std::log10;
using std::pow;
using std::round;
using std::fixed;
using std::setprecision;

double round_sig(double x, int n) 
{
    if (x == 0.0) {
        return 0.0;
    }
    int order = static_cast<int>(floor(log10(fabs(x))));
    double factor = pow(10.0, n - 1 - order);
    return round(x * factor) / factor;
}

double calc_abs_err(double A, double a) 
{
    return fabs(A - a);
}

double calc_rel_err(double A, double a) 
{
    if (A == 0.0) return 0.0;
    return calc_abs_err(A, a) / fabs(A);
}

double calc_abs_from_rel(double a, double delta_perc)
{
    return fabs(a) * (delta_perc / 100.0);
}

double calc_rel(double delta, double value) 
{
    if (value == 0.0) return 0.0;
    return delta / fabs(value);
}

int count_valid_digits(double x, double delta, double omega = 0.5) 
{
    if (x == 0.0 || delta <= 0.0) return 0;

    int m = static_cast<int>(floor(log10(fabs(x)))) + 1;
    int n = static_cast<int>(floor(m + 1.0 - log10(delta / omega)));
    return n > 0 ? n : 0;
}
