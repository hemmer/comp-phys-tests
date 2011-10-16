// Simple program to use an RK4 integrator

#include <iostream>
#include <cmath>

using namespace std;

double f(double t, double phi);
double rk4(const double &y, const double &t, const double &h);

int main (int, char **) {
    
    double h = 0.01;     // stepsize

    // integration limits
    double t_0 = 0.0, t_max = 10.0, t = t_0;
    // initial value
    double phi_0 = 1.5;     


    // start with initial value
    double phi_next = phi_0; 

    // print header
    cout << "t\tphi" << endl;
    // main integration loop
    while (t < t_max)
    {
        phi_next = rk4(phi_next, t, h);
        t += h;

        cout << t << "\t" << phi_next << endl;
    }

    return 0;
}


// Simple function (note no time dependence)
double f(double, double phi)
{
    return (2 * phi - 4 * phi * phi * phi);
}

// RK4 integrator
double rk4(const double &y, const double &t, const double &h)
{
    double hh = 0.5 * h;    // half a timestep
    double h6 = h / 6.0;    // sixth of a timestep
    double th = t + hh;   // current time advanced by half a timestep

    double k1 = h * f(t, y);
    double k2 = h * f(th, y + 0.5 * k1);
    double k3 = h * f(th, y + 0.5 * k2);
    double k4 = h * f(th, y + k3);

    //cout << k1 << " " << k2 << " " << k3 << " " << k4 << endl; 
    return y + (1/6.0) * (k1 + k2 + k3 + k4);
}
