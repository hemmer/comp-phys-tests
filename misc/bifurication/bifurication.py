#!/usr/bin/env python

'''
Simple little script to create a bifurication
diagram. This can demonstrate the chaotic nature
of convergence for certain equations. For example
the equation:

    x_n+1 = a * x_n * ( 1 - x_n )

converges to different numbers of solutions depending
on both the value of a and the starting value x0 and
these can be displayed on a bifurication diagram.
'''

import numpy as np
import pylab as py
import math

def get_convergent_value(x0, a):

    tolerance = 0.0001

    x_next = a * x0 * (1 - x0)

    # only bother checking for 200 iteration (may get stuck)
    for i in xrange(200):
        x = x_next
        x_next = a * x * (1 - x)

        if abs(x_next - x) < tolerance:
            break

    return x_next


# set of a values to use
a_set = np.arange(2,4,0.005)
vs = np.zeros(len(a_set))


py.ion()
py.figure()
py.xlabel(r"parameter $a$")
py.ylabel(r"convergent value ($x$)")
py.title(r"bifurication diagram for: $x_{n+1} = x_n a (1 - x_n)$")

# try multiple starting values as may
# converge to more than one solution
for x0 in np.arange(0.001, 1, 0.01):
    for i, a in enumerate(a_set):
        vs[i] = get_convergent_value(x0, a)

    print "initial x", x0
    py.scatter(a_set, vs, marker='o', s=0.1)
    py.draw()

py.show()
raw_input("press any key to finish")
