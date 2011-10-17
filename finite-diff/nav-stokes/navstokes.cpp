#include <iostream>
#include <fstream>
#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

using namespace boost::numeric::ublas;
using std::cout;



int main (int /*argc*/, char ** /*argv*/) 
{

    // number of internal spacial points
    int N = 5;

    // time / spacial resolution
    double dt = 0.1, dx = 0.1, dy = 0.1;
    double t = 0.0, t_max = 10.0;

    // total number of spacial points
    // including boundaries
    int n_x, n_y;
    n_x = n_y = (N + 2);

    // grid for pressure
    matrix<double> p(n_x, n_y);
    // grids for x / y components of velocity
    matrix<double> u(n_x - 1, n_y), v(n_x, n_y - 1);

    cout << u << " " << v;
 
    while (t < t_max)
    {
        /* Some comments on the index notation, particularly due to the
           shifted grid. The pressure grid index notation just maps 
           straight onto the array notation:
           
           p(i, j) -> p(i, j)
           
           Velocity components u and v are shifted by 1/2 in one dimension
           only (i and j dimensions respectively). Clearly this notation
           can't directly map to an array notation, so instead we use:

           u(i+1/2, j) -> u(i, j)     ex.  u(1/2, 0) -> u(0, 0) etc
           v(i, j+1/2) -> v(i, j)     ex.  v(1, 1/2) -> v(1, 0) etc

        */

        // Apply boundary conditions
        // In reality this is v(i, 1/2) = 0 
        for (int i = 1; i < (n_x - 1); ++i) v(i, 0) = 0;
        // In reality this is u(1/2, j) = 0 
        for (int j = 1; j < (n_y - 1); ++j) u(i, 0) = 0;
        

        for (int i = 1; i < (n_x - 1); ++i)
        {
            for (int j = 1; j < (n_y - 1); ++j)
            {
                double viscous_term_u = (u(i-1, j) + u(i+1, j) - 2 * u(i, j)) / dx2
                                      + (u(i, j-1) + u(i, j+1) - 2 * u(i, j)) / dy2
                double viscous_term_v = (v(i-1, j) + v(i+1, j) - 2 * v(i, j)) / dx2
                                      + (v(i, j-1) + v(i, j+1) - 2 * v(i, j)) / dy2
                
                double pressure_term_u = (p(i+1, j) - p(i, j)) / dx;
                double pressure_term_v = (p(i+1, j) - p(i, j)) / dy;

                double convective_a_u = (0.25 / dx) * ((u(i+1, j) + u(i, j)) ** 2
                                                     + (u(i, j) + u(i-1, j)) ** 2);
                double convective_a_v = (0.25 / dy) * ((v(i+1, j) + v(i, j)) ** 2
                                                     + (v(i, j) + v(i-1, j)) ** 2);
                
                convective_a_u += (0.25 / dy) * ((u(i, j) + u(i, j+1)) * (v(i, j) + v(i+1, j))
                                               - (u(i, j) + u(i, j-1)) * (v(i, j) + v(i+1, j)));

                convective_a_v += (0.25 / dx) * ((u(i, j) + u(i, j+1)) * (v(i, j) + v(i+1, j))
                                               - (u(i, j) + u(i, j-1)) * (v(i, j) + v(i+1, j)));

                u(i, j) = 
                    
            }
        }

        t += dt;
    }
    return 0;
}
