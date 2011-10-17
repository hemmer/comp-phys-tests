#include "LatticeModel.h"
#include "helpers/misc_maths.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

LatticeModel::LatticeModel(int &rows, int &cols) :
    rows_(rows), cols_(cols), size_(rows*cols),
    grid(rows, cols), T(0.0), expCache(2, 0.0) {

    initialiseSystem();
}

void LatticeModel::initialiseSystem() {

    // seed the RNG using the clock
    srand ( time(NULL) );

    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            grid(i, j) = (rand() % 2 == 1) ? 1 : -1;
            // grid(i,j) = 1;   // uncomment for warm start
        }
    }

    // reset energy / magnetisation counters
    energy = 0;
    magnetisation = 0;

    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if (spin(i, j) == 1) {
                magnetisation++;
                energy -= getNearestNeighbours(i, j);
            } else {
                magnetisation--;
                energy += getNearestNeighbours(i, j);
            }
        }
    }

    energy /= 2;    // correct for double counting

}


// update model temperature
void LatticeModel::setTemp(double &temp) {
    T = temp;
    cacheExponentials();    // update stored exp(-dBeta)
}


// calculate the exponential term for a specific temperature
void LatticeModel::cacheExponentials() {
    double dBeta = J * 4.0 / T;
    expCache[0] = exp(-dBeta);
    expCache[1] = exp(-2.0 * dBeta);
}


// abstract the lookup method for cache
inline double &LatticeModel::expdBeta(int &nnSum) { return expCache[abs(nnSum) / 2 - 1]; }


// returns reference to a spin (modifiable) including
// periodic boundary conditions
inline int &LatticeModel::spin (int xPos, int yPos) {

    // apply period boundary conditions
    xPos = (xPos >= rows_) ? xPos - rows_ : xPos;
    yPos = (yPos >= cols_) ? yPos - cols_ : yPos;
    xPos = (xPos < 0) ? rows_ - 1 : xPos;
    yPos = (yPos < 0) ? cols_ - 1 : yPos;

    return grid(xPos, yPos);
}

// flip the spin of a site and return the new value
int LatticeModel::flipSpin(int &xPos, int &yPos) {

    grid(xPos, yPos) = -grid(xPos, yPos);   // flip spin
    return grid(xPos, yPos);
}


// returns the energy change associated with a flip
int LatticeModel::getNearestNeighbours(int &xPos, int &yPos) {
    return spin(xPos + 1, yPos) + spin(xPos - 1, yPos) + spin(xPos, yPos + 1) + spin(xPos, yPos - 1);
}


// update grid using Metropolis Aglorithm
void LatticeModel::advanceMetropolis() {

    // pick a random position on the lattice
    int xPos = (rand() % rows_);
    int yPos = (rand() % cols_);


    // get the sum of nearest neighbour spins
    int nnSum = getNearestNeighbours(xPos, yPos);

    // sum nearest neighbours, multiply by sign of
    // the flipped state, and by 2 for the difference
    //
    //    +          +       here dE = E2 - E1
    //  + + +  ->  + - +             = -4 - 4
    //    +          +               = -8
    // E1 = 4     E2 = -4            = 2 * nnSum * sign of flipped state
    //                             ( = 2 * 4     * -1
    //

    double dE = -J * nnSum * 2.0 * -spin(xPos, yPos);

    // flip if the switch lowers overall energy
    // or with Boltzmann probability exp(-dBeta)
    // if it doesn't (where dBeta = dE / T )
    if (dE <= 0.0 || expdBeta(nnSum) > randDouble() ) {
        //      cout << "\nflip";
        // flip the spin, and count energy / magnetisation changes
        magnetisation += (2 * flipSpin(xPos, yPos) );
        energy += dE;
    }
}

void LatticeModel::monteCarloStep() {

    // update random positions on the grid once
    // for each cell
    for (int i = 0; i < (rows_ * cols_); ++i ) {
        advanceMetropolis();
    }
}





// override output operator for easier debugging
std::ostream& operator<<(std::ostream& os, const LatticeModel& model) {

    os << std::endl;    // clear with newline

    for (int i = 0; i < model.rows(); i++) {
        for (int j = 0; j < model.cols(); j++) {
            if (model.spin(i, j) == 1) {
                os << "#";
            } else {
                os << "0";
            }
        }

        os << std::endl;    // reset row
    }

    return os;
}
