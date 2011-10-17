/* 
   Simple program to use an RK4 integrator

   The program describes the 1-dimensional motion of a particle
   under earth's gravity. The RK4 solver can take coupled equations
   and we can use this to break down the 2nd order ODE into two 
   1st order equations:

   a = F / m   ->   v = dx / dt
                    a = dv / dt

*/
#include <iostream>
#include <fstream>
#include <cmath>
#include <boost/numeric/ublas/vector.hpp>

using namespace boost::numeric::ublas;
using std::cout;
using std::endl;

double f(const double &t, const vector<double> &y, const int &i);
void rk4(vector<double> &y, const double &t, const double &h);

int main (int, char **) {


    const int N = 2;    // this is the number of equations
    double h = 0.01;    // stepsize

    // integration limits
    double t_0 = 0.0, t_max = 10.0, t = t_0;
    // initial values
    vector<double> y_0(N);
    y_0(0) = 10.0;           // initial velocity 0.0
    y_0(1) = 10.0;          // initial position 10.0

    // start with initial value
    vector<double> y(y_0);


    // print header for output file
    std::ofstream outputFile;
    outputFile.open("data/output.dat");
    outputFile << "# t\tvel\tpos" << endl;

    // main integration loop
    while (t < t_max)
    {
        rk4(y, t, h);   // apply the integrator
        t += h;         // increment timestep

        // write output to file
        outputFile << t << "\t" << y(0) << "\t" << y(1) << endl;

        // if the particle hits the ground, stop!
        if (y(1) < 0) break;
    }

    return 0;
}


// Simple function (note no time dependence)
double f(const double & /*t*/, const vector<double> &y, const int &i)
{
    // gravitational force
    double F = -9.8;
    double m = 1.0;

    if (i == 0) return (F / m);     // velocity is calculated from force
    if (i == 1) return y[0];        // position is calculated from velocity
   
    // this shouldn't happen!
    return -1.0;
}

/* RK4 integrator

   Initially, y corresponds to y_n. At the end of 
   the routine, y is updated (effectively becoming
   y_(n+1).
*/
void rk4(vector<double> &y, const double &t, const double &h)
{
    int N = y.size();

    double th = t + 0.5 * h;    // current time advanced by half a timestep
    double tf = t + h;          // current time advanced by a timestep

    vector<double> k1(N), k2(N), k3(N), k4(N);
    vector<double> temp1(N), temp2(N), temp3(N), temp4(N);

    // STEP 1
    for (int i = 0; i < N; ++i)
    {
        k1(i) = h * f(t, y, i); 
        temp1(i) = y(i) + 0.5 * k1(i);      // used in STEP 2
    }

    // STEP 2
    for (int i = 0; i < N; ++i)
    {
        k2(i) = h * f(th, temp1, i); 
        temp2(i) = y(i) + 0.5 * k2(i);      // used in STEP 3
    }

    // STEP 3
    for (int i = 0; i < N; ++i)
    {
        k3(i) = h * f(th, temp2, i);
        temp3(i) = y(i) + k3(i);            // used in STEP 4
    }

    // STEP 4
    for (int i = 0; i < N; ++i)
        k4(i) = h * f(tf, temp3, i); 


    // then sum the contributions
    for (int i = 0; i < N; ++i)
        y(i) = y(i) + (k1(i) + k2(i) + k3(i) + k4(i)) / 6.0;

}
