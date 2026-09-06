#include "series_taylor.hpp"

int main()
{
    double a = 0.0;
    double b = 1.0;
    int k = 11;
    int n_fixed = 5;
    double eps = 0.0001;

    double h = (b - a) / (k - 1);

    cout << fixed << setprecision(6);
    cout << "x\tExact\t\tBy N (n=5)\tBy eps\t\tTerms" << endl;

    for (int step = 0; step < k; ++step)
    {
        double x = a + step * h;
        if (x > b) x = b;

        int terms_used = 0;
        double y_exact = exact_f(x);
        double y_n     = series_by_n(x, n_fixed);
        double y_eps   = series_by_eps(x, eps, terms_used);

        cout << setprecision(2) << x << "\t"
             << setprecision(6) << y_exact << "\t"
             << y_n << "\t"
             << y_eps << "\t"
             << terms_used << endl;
    }

    return 0;
}