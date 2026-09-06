#include <iostream>
#include <iomanip>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::fabs;
using std::fixed;
using std::setprecision;
using std::scientific;

double calc_series_sum(double eps, int& count)
{
    int n = 0;
    double a_n = 1.0 / (1.0 * (n + 3));
    double sum = 0.0;
    count = 0;

    while (fabs(a_n) >= eps)
    {
        sum += a_n;
        count++;
        
        a_n *= static_cast<double>(n + 3) / ((n + 1) * (n + 4));
        n++;
    }

    return sum;
}