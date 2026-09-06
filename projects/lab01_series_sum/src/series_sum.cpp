#include "series_sum.hpp"

int main()
{
    char cont = 'y';

    while (cont == 'y' || cont == 'Y')
    {
        double eps = 0.0;
        
        cout << "Enter accuracy eps (e.g. 0.1 to 1e-6): ";
        cin >> eps;

        if (eps <= 0.0)
        {
            cout << "Accuracy must be positive!" << endl;
            continue;
        }

        int terms_count = 0;
        double sum = calc_series_sum(eps, terms_count);

        cout << fixed << setprecision(8);
        cout << "Accuracy (eps): " << scientific << setprecision(2) << eps << endl;
        cout << fixed << setprecision(8);
        cout << "Sum of series: " << sum << endl;
        cout << "Terms computed: " << terms_count << endl;

        cout << endl;
        cout << "Continue? (y/n): ";
        cin >> cont;
        cout << endl;
    }

    return 0;
}