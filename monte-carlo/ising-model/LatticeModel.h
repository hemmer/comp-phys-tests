/*
 * ====================================================================================
 *
 *       Filename:  LatticeModel.h
 *    Description:
 *
 *        Version:  1.0
 *        Created:  12/07/11 21:27:32
 *
 *         Author:  Ewan Hemingway, ewan.hemingway@gmail.com
 *
 * =====================================================================================
 */

#ifndef  LATTICEMODEL_INC
#define  LATTICEMODEL_INC

#include <vector>

// library for dealing with matrices
#include "helpers/matrix.h"
using namespace gds;

const double J = 1.0;   // strength of spin

class LatticeModel {

    public:
        // update grid using Metropolis Aglorithm
        void advanceMetropolis();

        // reset system
        void initialiseSystem();

        // try to flip a random site once for cell that exists
        void monteCarloStep();

        // constructors
        LatticeModel(int &rows, int &cols);
        LatticeModel(int length);

        // getters/setters
        int rows() const { return rows_; }
        int cols() const { return cols_; }

        double currentEnergy() const { return J * energy / size_; }
        double currentMagnetisation() const { return (double) magnetisation / (double) size_; }

        void setTemp(double &temp);
        inline int spin (int &xPos, int &yPos) const { return grid(xPos, yPos); };      // get spin (with BCs)


    private:
        const int rows_, cols_, size_;

        // use integer counts for energy / magnetisation
        // for accuracy, can multiply by -J for actual value
        int energy, magnetisation;

        matrix<int> grid;   // spins are stored in a matrix object
        double T;     // temperature

        std::vector<double> expCache;    // calculate cached exponential terms
        void cacheExponentials();   // function to regenerate (for a new temperature etc.):w
        inline double &expdBeta(int & nnSum);  // lookup cached exponential terms

        inline int &spin (int xPos, int yPos);      // get spin (with BCs)
        int flipSpin(int &xPos, int &yPos);      // flips spin
        int getNearestNeighbours(int &xPos, int &yPos);      // get nearest neighbour counts

        // override << operator for better output
        friend std::ostream& operator<<(std::ostream& os, const LatticeModel& model);
};


#endif   /* ----- #ifndef LATTICEMODEL_INC  ----- */
