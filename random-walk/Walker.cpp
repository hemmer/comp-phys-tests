/*
 * =====================================================================================
 *
 *       Filename:  Walker.cpp
 *
 *    Description:  Model of a simple walker on a 2D lattice
 *
 *        Version:  1.0
 *        Created:  04/08/11 10:41:39
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

#include "Walker.h"
#include <cmath>    // uses sqrt() for norms
#include <ctime>    // for seeding RNG
#include <iostream>     // for debugging / output
#include <cstdlib>      // for inital move

// default constructor
Walker::Walker() : xPos(0), yPos(0), prevStep(0), dist(1, 3), randStep(gen, dist) {
    gen.seed( static_cast<unsigned int>(std::time(0)) );      // seed with system clock
    reset();
}


// re-centre the walker on the lattice
void Walker::reset() {

    xPos = yPos = 0;

    // initial move in *any* direction
    int initialMove = rand() % 4;
    applyMove(initialMove);
    
    prevStep = initialMove;
}


// perform _one_ random step, excluding the direction
// we just came from 
void Walker::moveStep() {

    // All the possible directions are stored in the enum
    // so that UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3. The 
    // previous direction is stored, so that the walker 
    // can't backtrack.

    // In fact it is the opposite direction which is dis-
    // allowed (i.e. retracing the step) so we add "d" 
    // (where "d" is the number of dimensions) to flip

    // Now as randStep() generates a number on [1,3]
    // this can be added to the previous step to choose
    // one of the 3 possible directions. The "mod 4" 
    // "wraps" the sum back round.
    int move = (prevStep + randStep() + 2) % 4;

    // apply the move to the coordinates
    applyMove(move);

    // and store the move for the next iteration
    prevStep = move;
}


// apply the correct increments for
// the specified direction
void Walker::applyMove(int &move){

    switch(move) {
        case UP:
            ++yPos; break;
        case RIGHT:
            ++xPos; break;
        case DOWN:
            --yPos; break;
        case LEFT:
            --xPos; break;
    }
}


// returns R = sqrt(x^2 + y^2)
double Walker::endToEndDist() {
    return sqrt( xPos * xPos + yPos * yPos);
}


// override output operator for easier debugging
std::ostream& operator<<(std::ostream& os, const Walker& walker) {

    os << "(" << walker.x() << ", " << walker.y() << ")";
    return os;
}
