Random Walk Model 
=================


Introduction
------------

Basic 2D random walk code just to give me a bit of practice getting used to C++ / git. There are two branches, the default (named nonreversible) is written such that the walker cannot retrace the step they just took. The other branch (master) allows walking in any direction.


Instructions
------------

To run, first compile using:

    $ make 

Then you can run the program using either:

    $ make exec

(which will automatically plot the data with gnuplot once finished). You can also just run the program by:

    $ ./RandomWalk

The program outputs to a file in the data/ folder.

Requirements
------------

This program uses the boost numerical libraries for random number generation, which can be installed on Debian/Ubuntu with:

    $ sudo apt-get install libboost-dev

Alternatively, you can compile for source from the [official site](http://www.boost.org).
