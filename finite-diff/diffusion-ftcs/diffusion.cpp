#include <boost/numeric/ublas/matrix.hpp>
using boost::numeric::ublas::matrix;
#include <iostream>
#include <fstream>

using namespace std;

int main (int, char **)
{

    int Nt = 1000;                    // number of timesteps
    int Nx = 50;                   // number of spacial points
    double dt = 0.01, dx = 0.01;     // timestep / spacial resolution
    double D = 0.0001;             // diffusion const

    // save calculating this at every step
    double factor = D * dt / (dx * dx);

    // track for all times (only 2 rows needed in reality)
    matrix<double> grid(Nx, Nt);


    // initialise system with an initial drop of material in the middle
    grid(int(0.4 * Nx), 0) = 100;
    grid(int(0.6 * Nx), 0) = 100;
    grid(int(0.1 * Nx), 0) = 100;

    // main experiment
    for (int t = 0; t < Nt - 1; ++t)
    {

        // NOTE: here we are using Neumann BCs, i.e:
        // du(0, t)/dx = du(Nx - 1, t)/dx = 0 for all t
        grid(0, t) = grid(1, t);
        grid(Nx - 1, t) = grid(Nx - 2, t);

        for (int i = 1; i < Nx - 1; ++i)
        {
            grid(i, t + 1) = grid(i, t) + factor *
                             (grid(i + 1, t) + grid(i - 1, t) - 2 * grid(i, t));
        }

        
        // check conservation of mass, remembering to ignore the 
        // boundary points!
        double mass = 0.0;
        for (int i = 1; i < Nx - 1; ++i) mass += grid(i, t);
        cout << "mass at step " << t << " is: " << mass << endl;
    }



    // write results to file, starting with a header for GNUPLOT
    ofstream dataFile;
    dataFile.open("data/output.dat", ios::trunc);
    dataFile << "# D: " << D << ", Nx: " << Nx << ", Nt: " << Nt << endl;
    dataFile << "# dx: " << dx << ", dt: " << dt << endl;
    dataFile << "# x \t t \t phi \n";

    int time_resolution = 30;        // don't need to plot every time point

    for (int t = 0; t < Nt; t += time_resolution)
    {
        for (int x = 0; x < Nx; ++x) dataFile << x << "\t" << t << "\t" << grid(x, t) << endl;

        dataFile << endl;
    }

    dataFile.close();
    return 0;
}

