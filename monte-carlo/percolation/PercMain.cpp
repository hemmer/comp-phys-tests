/*
 * =====================================================================================
 *
 *       Filename:  PercMain.cpp
 *
 *        Created:  07/08/11 20:08:26
 *       Compiler:  gcc
 *
 * =====================================================================================
 */


#include <iostream> // for output/debug
using std::cout;
using std::endl;

#include "Lattice.h"

int main () {

    int length = 10;
    int rows, cols;
    rows = cols = length;   // square lattice for simplicity

    // statistics
    int numTests = 100;
    int result = 0;
    int totalSuccesses = 0;
    double meanSuccessRate = 0.0;

    // initialise the lattice
    Lattice lattice(rows, cols);
    lattice.initialise();

    // print header
    cout << "prob\tcross rate" << endl;

    // p - probability of site occupation
    // explore for multiple values of p
    for (double p = 0.0; p <= 1.0; p += 0.02) {

        lattice.setProb(p);
        totalSuccesses = 0;

        // perform a number of tests and
        // take averages
        for (int test = 0; test < numTests; ++test) {

            lattice.initialise();           // generate a new configuration
            result = lattice.findPath();    // find a path (returns true if path available)

            totalSuccesses += result;       // and sum sucessful results for averaging
        }

        // find average and print
        meanSuccessRate = (double) totalSuccesses / (double) numTests;
        cout << p << "\t" << meanSuccessRate << endl;
    }

    return 0;
}

