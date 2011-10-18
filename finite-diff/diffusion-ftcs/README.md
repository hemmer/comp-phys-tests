Diffusion Test 
==============


Introduction
------------

This project aims to solve the basic diffusion equation in 1D using finite difference methods. 


Instructions
------------

To run, first compile using:

    $ make 

Then you can run the program using either:

    $ make exec

(which will automatically plot the data with gnuplot once finished). You can also just run the program by:

    $ ./diffusion

The program outputs data to a file in the data/ folder.

Requirements
------------

This program uses the boost numerical libraries for matrices, which can be installed on Debian/Ubuntu with:

    $ sudo apt-get install libboost-dev

or on RHEL:

    $ yum install boost boost-devel

Alternatively, you can compile for source from the [official site](http://www.boost.org).
