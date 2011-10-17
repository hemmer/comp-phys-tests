Percolation
===========

This experiment starts by populating sites on a lattice with probability _p_. Then by using a recursive algorithm, we can see how many of these configurations can form a path across the lattice using adjacent sites. By varying the probability, we can control what percentage of configurations can sucessfully cross. Clearly the extreme cases are simple to predict:

    p(0) = 0 (as no sites occupied)
    p(1) = 1 (as full lattice is always crossable)

Instructions
------------

To run, first compile using:

    $ make 

Then you can run the program using either:

    $ make exec

(which will automatically plot the data with gnuplot once finished). You can also just run the program by:

    $ ./percolation

The program outputs data to a file in the data/ folder.

Requirements
------------

This program uses the boost numerical libraries for random number generation, which can be installed on Debian/Ubuntu with:

    $ sudo apt-get install libboost-dev

or on RHEL:

    $ yum install boost boost-devel

Alternatively, you can compile for source from the [official site](http://www.boost.org).
