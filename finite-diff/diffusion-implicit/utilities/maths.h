#include <boost/numeric/ublas/vector.hpp>
using boost::numeric::ublas::vector;

/* Method for solving a tridiagonal set of linear 
   equations, as described in Numerical Recipes in C.
*/    
bool tridag(const vector<double> &a,    // lower diagonal values
            const vector<double> &b,    // diagonal values
            const vector<double> &c,    // upper diagonal values
            const vector<double> &r,
            vector<double> &u,
            const int &n)               // for n x n matrix
{
    double beta;

    vector<double> gamma(n);

    // If this happens, the set of equations should be 
    // rewritten with u_2 eliminated.
    if (b(0) == 0.0)
    {
        cout << "b(0) == 0" << endl;
        return false;
    }

    beta = b(0);
    u(0) = r(0) / beta;


    // Decomposition and forward substitution
    for (int j = 1; j < n; ++j)
    {
        gamma(j) = c(j-1) / beta;
        beta = b(j) - a(j) * gamma(j);
        
        if (beta == 0.0)
        {
            cout << "zero pivot! unable to continue" << endl;
            return false;
        }

        u(j) = (r(j) - a(j)*u(j-1)) / beta;
    }

    // Backsubstitution
    for (int j = (n - 2); j >= 0; --j) u(j) -= gamma(j+1) * u(j+1);

    return true;
}
