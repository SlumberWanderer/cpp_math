#include "errors_calc.hpp"

int main()
{
    double A = 2.1514;
    int digits = 3;

    double a = round_sig(A, digits);
    double d_abs = calc_abs_err(A, a);
    double d_rel = calc_rel_err(A, a);

    cout << "Task 1" << endl;
    cout << fixed << setprecision(5);
    cout << "Exact value A:   " << A << endl;
    cout << "Approx value a:  " << a << endl;
    cout << "Absolute error:  " << d_abs << endl;
    cout << fixed << setprecision(6);
    cout << "Relative error:  " << d_rel
         << " (" << setprecision(4) << d_rel * 100.0 << "%)" << endl;

    double a2 = 132.67;
    double d_perc = 0.1;
    double d_abs2 = calc_abs_from_rel(a2, d_perc);

    cout << endl;
    cout << "Task 2" << endl;
    cout << fixed << setprecision(2);
    cout << "Approx value a:  " << a2 << endl;
    cout << "Relative error:  " << d_perc << " %" << endl;
    cout << fixed << setprecision(5);
    cout << "Absolute error:  " << d_abs2 << endl;

    double L1 = 17.52 * 1000.0;
    double d1 = 5.0;
    double L2 = 2.0;
    double d2 = 0.005;

    double rel1 = calc_rel(d1, L1);
    double rel2 = calc_rel(d2, L2);

    cout << endl;
    cout << "Task 3" << endl;
    cout << fixed << setprecision(4);
    cout << "delta1: " << rel1 * 100.0 << " %" << endl;
    cout << "delta2: " << rel2 * 100.0 << " %" << endl;
    if (rel1 < rel2) {
        cout << "Conclusion: the 1 measurement is more accurate" << endl;
    } else if (rel2 < rel1) {
        cout << "Conclusion: the 2 measurement is more accurate" << endl;
    } else {
        cout << "Conclusion: both measurements have identical accuracy" << endl;
    }

    double x = 0.3941;
    double d_x = 0.25e-2;

    int n_narrow = count_valid_digits(x, d_x, 0.5);
    int n_broad  = count_valid_digits(x, d_x, 1.0);

    cout << endl;
    cout << "Task 4" << endl;
    cout << fixed << setprecision(4);
    cout << "Value x: " << x << endl;
    cout << "Abs error dx: " << d_x << endl;
    cout << "Valid digits (narrow sense): " << n_narrow << endl;
    cout << "Valid digits (broad sense): " << n_broad << endl;

    return 0;
}