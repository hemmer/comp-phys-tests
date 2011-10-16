/*
 * =====================================================================================
 *
 *       Filename:  RWMain.cpp
 *
 *    Description:  Self-avoiding Branch
 *
 *        Version:  1.0
 *        Created:  04/08/11 10:40:44
 *       Compiler:  gcc
 *
 * =====================================================================================
 */
#include "Walker.h"


#include <cmath>
#include <iostream>     // for debugging / output
#include <fstream>      // for writing data to file

using std::cout;
using std::endl;


int main (int argc, char **argv) {


    // basic experiment settings
    int experiments = 1000;   // how many random walks to test
    int N = 20;             // how many steps to take
    // quantities to average
    double endToEndSum = 0.0, endToEndMean = 0.0;
    double xSum = 0.0, xMean = 0.0;
    double ySum = 0.0, yMean = 0.0;

    // may as well resuse one random walker
    // rather than re-initialising every time
    Walker rw;


    std::ofstream dataFile;                     // output data to file
    dataFile.open("data/output.dat", std::ios::trunc);    // overwrite existing data
    dataFile << "# N - number of steps in the walk" << endl;
    dataFile << "# <R> - mean end-to-end distance" << endl;
    dataFile << "# N \t <R> \t sqrt(N)" << endl;

    // loop for a different length of walker
    for (N = 10; N < 5000; N += 40) {

        // reset averages etc
        endToEndSum = endToEndMean = 0.0;
        xSum = xMean = 0.0;
        ySum = yMean = 0.0;

        // each loop is one complete walk
        for (int n = 0; n < experiments; n++) {

            // reset the random walker to position (0, 0)
            rw.reset();

            // perform N random steps
            rw.performWalk(N);

            endToEndSum += rw.endToEndDist();
            xSum += abs(rw.x());
            ySum += abs(rw.y());
        }

        endToEndMean = endToEndSum / (double) experiments;  // calculate the mean
        xMean = xSum / (double) experiments;
        yMean = ySum / (double) experiments;

        // write to file
        dataFile << N << "\t" << endToEndMean << "\t\t" << sqrt(N) << endl;                     // <R>
        //dataFile << N << "\t" << endToEndMean*endToEndMean << "\t\t" << N << endl;            // <R>^2
        //dataFile << N << "\t" << xMean*xMean << "\t\t" << yMean*yMean << "\t\t" << endl;      // <x> and <y>
    }


    dataFile.close();       // close the file
    return 0;
}
