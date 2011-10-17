/* =====================================================================================
 *
 *
 *       Filename:  Lattice.h
 *
 *    Description:
 *
 *        Created:  07/08/11 21:31:13
 *       Compiler:  gcc
 *
 * =====================================================================================
 */

// boost RNG libraries
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/numeric/ublas/matrix.hpp>
using boost::numeric::ublas::matrix;

#include <ctime>    // for seeding RNG

#include <iostream> // for output/debug
using std::cout;
using std::endl;

#include <deque>    // for tracking tree
using std::deque;
#include <utility>  // for pair
using std::make_pair;
using std::pair;

class Lattice {
    public:
        void initialise();
        void initialiseTest();
        void printLattice();    // print ASCII representation for debugging etc

        Lattice(int, int);  // main constructor

        // tries to find a path to the other side
        // of the lattice
        bool findPath();

        // getters / setters
        void setProb(double prob) { p = prob; }
    private:

        typedef pair<int, int> coord;   // simple pair of x/y coordinates
        typedef deque< coord >::reverse_iterator tree_iter;

        int rows, cols;     // lattice dimensions
        matrix<int> lattice;    // data structure for lattice
        double p;           // probabilty of site being occupied 


        //    ^ 0
        // 3  |
        // <--o-->
        //    |  1
        //  2 v
        enum Motions {UP, RIGHT, DOWN, LEFT, NUM_DIRECTIONS};

        // increments coordinates (xPos, yPos) in the
        // direction specified by Motion move
        void applyMove(int &move, int &xPos, int &yPos);

        // when testing, define which direction
        // we are exploring
        enum Traversal { HORIZONTAL, VERTICAL};
        int direction;

        // recursive function to find a path across the lattice
        bool searchTree(int currentX, int currentY);
        // temporary variables required by "searchTree"
        int testX, testY;

        // FILO structure to track current path
        deque<coord> tree;
        deque<coord> completePath;
        void printDeque(deque<coord> &d);


        // boost RNG stuff
        boost::mt19937 gen;
        boost::uniform_real<> dist;
        boost::variate_generator<boost::mt19937&, boost::uniform_real<> > randSite;
};




/////////////////////////
// Main Implementation //
/////////////////////////

// main constructor
Lattice::Lattice(int rows_, int cols_) : rows(rows_), cols(cols_),
    lattice(cols, rows), p(0.0), direction(-1),
    dist(0.0, 1.0), randSite(gen, dist) {

    // seed with system clock
    gen.seed( static_cast<unsigned int>(std::time(0)) );
}




bool Lattice::findPath() {

    direction = VERTICAL;

    for (int i = 0; i < cols; ++i) {
        if(lattice(i, 0)) {

            tree.clear();   // reset tree

            // if we find a successful path
            // then we can stop looking
            if(searchTree(i, 0)) return true;

        }
    }


    direction = HORIZONTAL;

    for (int j = 0; j < rows; ++j) {
        if(lattice(0, j)) {

            tree.clear();   // reset tree

            // if we find a successful path
            // then we can stop looking
            if(searchTree(0, j)) return true;

        }
    }

    // if we get the whole way without finding
    // a path then the function fails
    return false;
}



// recursive function to explore paths
// across the lattice
bool Lattice::searchTree(int currentX, int currentY) {

    // store the current position as a coord
    coord currentPos(currentX, currentY);

    /* STEP 1.
       Check if we have sucessfully traversed the lattice. This depends
       on which direction we are testing.
       NOTE: this is the terminating statement for the recursion
    */
    if (direction == HORIZONTAL) {
        if(currentX == (cols - 1)) {
            tree.push_back(currentPos);     // remember to add the final step
            completePath = tree;    // save the path used for debugging etc
            return true;
        }
    }

    if (direction == VERTICAL) {
        if(currentY == (rows - 1)) {
            tree.push_back(currentPos);     // remember to add the final step
            completePath = tree;    // save the path used for debugging etc
            return true;
        }
    }



    /* STEP 2.
       Check if the current position being explored has already been
       visited in this branch of the tree. This avoids infinite loops.
    */

    // start (reverse) iterator at end as most likely to hit most recent nodes
    tree_iter t = tree.rbegin();

    while ( t < tree.rend()) {

        // if we hit any part of the previous steps of the path, then ignore
        // this branch (by exiting)
        if (currentPos == *t) {
            tree.pop_back();
            return false;
        }

        ++t;     // otherwise keep checking rest of the tree
    }


    /* STEP 3.
       Test each of the nearest neighbour sites for a legitimate next move.
    */

    for (int move = 0; move < NUM_DIRECTIONS; ++move) {
        testX = currentX;
        testY = currentY;
        applyMove(move, testX, testY);

        // if we wander off the lattice, ignore this move
        if (testX < 0 || testY < 0 || testX >= cols || testY >= rows) continue;

        // if the current neighbour is also occupied
        if ( lattice(testX, testY) ) {

            // at this point we have a legitimate
            // new position so we add this to tree
            // to continue tracking the path
            tree.push_back(currentPos);

            // recurse deeper - if we reach
            // other side, backtrack through
            // the tree with "true" flag
            if (searchTree(testX, testY) ) {
                return true;
            }
        }
    }

    // if no branches of the tree are sucessful
    // then retrace a step, checking for empty
    if(!tree.empty()) tree.pop_back();

    return false;

}


// apply the correct increments for
// the specified direction
void Lattice::applyMove(int &move, int &xPos, int &yPos) {

    switch(move) {
        case UP:
            ++yPos;
            break;
        case RIGHT:
            ++xPos;
            break;
        case DOWN:
            --yPos;
            break;
        case LEFT:
            --xPos;
            break;
    }
}


void Lattice::printDeque(deque<coord> &d) {

    deque<coord>::iterator it;

    for (it = d.begin(); it != d.end(); ++it) {
        cout << "(" << it->first << ", " << it->second << ") -> ";
    }
}


// initialise lattice sites for a test
void Lattice::initialiseTest() {

    for (int i = 0; i < rows; i++) {
        lattice(2, i) = 1;
    }

    lattice(2, 4) = 0;
    lattice(3, 2) = 1;
    lattice(4, 2) = 1;
    lattice(4, 3) = 1;
    lattice(4, 4) = 1;

}

// initialise lattice sites with probability "p"
// and clear path exploring variables
void Lattice::initialise() {

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            lattice(i, j) = (p > randSite()) ? 1 : 0;
        }
    }

    tree.clear();
    completePath.clear();

}


// display visual representation of lattice
void Lattice::printLattice() {

    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {

            if ( lattice(i, j) ) cout << "# ";
            else cout << "0 ";

        }

        cout << endl;
    }
}
