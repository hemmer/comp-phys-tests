#include <boost/numeric/ublas/matrix.hpp>
using boost::numeric::ublas::matrix;
#include <boost/numeric/ublas/vector.hpp>
using boost::numeric::ublas::vector;
#include <boost/numeric/ublas/io.hpp>

#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ofstream;

#include "utilities/maths.h"

int main (int, char **)
{

    int N_t = 100;                  // number of timesteps
    int N_x = 100;                  // number of spacial points
    double dt = 0.5, dx = 0.01;     // timestep / spacial resolution
    double D = 0.00005;             // diffusion const

    // save calculating this at every step
    double alpha = D * dt / (dx * dx);

    cout << "implicit finite difference method applied to the diffusion equation" << endl;
    cout << "alpha = " << alpha << endl;


    // these store the components of the tridiagonal matrix 
    // used to solve the system of linear equations
    vector<double> a(N_x), b(N_x), c(N_x), u(N_x), u_next(N_x);

    // initialise the system with a couple of "drops"
    u(int(0.4 * N_x)) = 100.0;
    u(int(0.6 * N_x)) = 100.0;

    for (int i = 0; i < N_x; ++i)
    {
        a(i) = -alpha;
        b(i) = (1 + 2*alpha);
        c(i) = -alpha;
    }


    // write results to file, starting with a header for GNUPLOT
    ofstream dataFile;
    dataFile.open("data/output.dat", std::ios::trunc);
    dataFile << "# D: " << D << ", N_x: " << N_x << ", N_t: " << N_t << endl;
    dataFile << "# dx: " << dx << ", dt: " << dt << endl;
    dataFile << "# x \t t \t phi \n";
    int time_resolution = 4;        // don't need to plot every time point


    // main experiment
    for (int t = 0; t < N_t - 1; ++t)
    {
        // solve the set of linear equations
        // (for a tridiagonal matrix)
        tridag(a, b, c, u, u_next, N_x);         

        // update the concentration
        for (int i = 0; i < N_x; ++i)
            u(i) = u_next(i);

        // NOTE: here we are using Dirichlet BCs, i.e:
        // u(0, t) = u(N_x - 1, t) = 0 for all t
        u(0) = u(N_x - 1) = 0.0;


        // check conservation of mass
        double mass = 0.0;
        for (int i = 0; i < N_x; ++i) mass += u(i);
        cout << "mass at timestep " << t << " is: " << mass << endl;

        // and write the data to file (every so often)
        if (t % time_resolution == 0)
        {
            for (int x = 0; x < N_x; ++x) dataFile << x << "\t" << t << "\t" << u(x) << endl;
            dataFile << endl;
        }
    }

    // finish writing to file
    dataFile.close();

    return 0;
}
