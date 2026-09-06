#include <iostream>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;
using std::fabs;
using std::log;
using std::pow;
using std::fixed;
using std::setprecision;

double exact_f(double x)
{
    return pow(3.0, x);
}

double series_by_n(double x, int n_terms)
{
    double term = 1.0;
    double sum = term;
    double factor = x * log(3.0);

    for (int i = 0; i < n_terms; ++i)
    {
        term *= factor / (i + 1);
        sum += term;
    }
    return sum;
}

double series_by_eps(double x, double eps, int& terms_count)
{
    double term = 1.0;
    double sum = term;
    double factor = x * log(3.0);
    terms_count = 1;

    int i = 0;
    while (fabs(term) >= eps)
    {
        term *= factor / (i + 1);
        sum += term;
        terms_count++;
        i++;
    }
    return sum;
}