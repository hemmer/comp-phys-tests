/*
 * =====================================================================================
 *
 *       Filename:  Walker.h
 *
 *    Description:  Model of a simple walker on a 2D lattice
 *
 *        Version:  1.0
 *        Created:  04/08/11 10:41:41
 *       Compiler:  gcc
 *
 * =====================================================================================
 */


#ifndef  WALKER_INC
#define  WALKER_INC

#include <iostream>

// boost RNG libraries
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>
#include <boost/random/variate_generator.hpp>

class Walker {
    public:
        Walker();           // default constructor

        // restarting public methods
        void reset();

        // find <R>
        double endToEndDist();

        // perform N steps
        // (N - 1) because initial step is performed on reset
        void performWalk(int &N) { for (int step = 0; step < (N - 1); step++) moveStep(); }

        // simple getters
        const int &x() const { return xPos; }
        const int &y() const { return yPos; }

        // override output operator
        friend std::ostream& operator<<(std::ostream& os, const Walker& walker);

    private:
        int xPos, yPos;     // current position
        int prevStep;     // previous position
        enum Motions {UP, RIGHT, DOWN, LEFT};

        void moveStep();    // move one random step 
        void applyMove(int &move);  // interpret the move

        boost::mt19937 gen;
        boost::uniform_int<> dist;    // boost RNG stuff
        boost::variate_generator<boost::mt19937&, boost::uniform_int<> > randStep;
};

#endif   /* ----- #ifndef WALKER_INC  ----- */

