
#define _DEBUG

#include "helpers/matrix.h"
#include <iostream>
#include <fstream>

using namespace std;
using gds::matrix;

int main (int, char **)
{

    int N_t = 1000;                    // number of timesteps
    int N_x = 50;                   // number of spacial points
    double dt = 0.01, dx = 0.01;     // timestep / spacial resolution
    double D = 0.0001;             // diffusion const

    // save calculating this at every step
    double factor = D * dt / (dx * dx);

    // track for all times (only 2 needed in reality)
    matrix<double> grid(N_x, N_t);


    // initialise system with an initial drop of material in the middle
    grid(0.4 * N_x, 0) = 100;
    grid(0.6 * N_x, 0) = 100;

    // main experiment
    for (int t = 0; t < N_t - 1; ++t)
    {
        // NOTE: here we are using Dirichlet BCs, i.e:
        // grid(0, t) = grid(N_x - 1, t) = 0 for all t
        for (int i = 1; i < N_x - 1; ++i)
        {
            grid(i, t + 1) = grid(i, t) + factor *
                             (grid(i + 1, t) + grid(i - 1, t) - 2 * grid(i, t));
        }

        // check conservation of mass
        double mass = 0.0;

        for (int i = 0; i < N_x; ++i) mass += grid(i, t);

        cout << "mass at step " << t << " is: " << mass << endl;
    }



    // write results to file, starting with a header for GNUPLOT
    ofstream dataFile;
    dataFile.open("data/output.dat", ios::trunc);
    dataFile << "# D: " << D << ", N_x: " << N_x << ", N_t: " << N_t << endl;
    dataFile << "# dx: " << dx << ", dt: " << dt << endl;
    dataFile << "# x \t t \t phi \n";

    int time_resolution = 30;        // don't need to plot every time point

    for (int t = 0; t < N_t; t += time_resolution)
    {
        for (int x = 0; x < N_x; ++x) dataFile << x << "\t" << t << "\t" << grid(x, t) << endl;

        dataFile << endl;
    }

    dataFile.close();
    return 0;
}

