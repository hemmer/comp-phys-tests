#!/usr/bin/env python

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

import numpy as np
import scipy as sp
import scipy.sparse

# number of spacial points, number of time points
N, Nt = 100, 50
a = 0.25

# store results as we go for plotting later
grid = np.zeros((N, Nt))

# vector for concentration
u = np.zeros(N)
u[int(N/3.0)] = 150
u[int(2*N/3.0)] = 150

# setup tridiagonal coefficient matrix
coeff = (2 + 2*a) * np.eye(N)
coeff[np.arange(N-1), np.arange(N-1)+1] = -a
coeff[np.arange(N-1) + 1, np.arange(N-1)] = -a

# main solving loop
for t in xrange(Nt):
    grid[:, t] = u

    print "mass at timestep", t, "is", np.sum(u)

    left = a * np.roll(u, 1)
    left[0] = 0

    right = a * np.roll(u, -1)
    right[-1] = 0

    # Basically this is just a quick way to do:
    # d_i = (1 - 2a) * u(i)  +  a * u(i+1)  +  a * u(i-1)
    d = (2 - 2 * a) * u
    d += left + right

    # solve the system of equations
    # (in reality would use a tridiagonal solver)
    u = np.linalg.solve(coeff, d)

    # boundary conditions
    #u[0] = 0
    #u[-1] = 0


# plot the results
plt.clf()
plt.imshow(grid)
plt.colorbar()
plt.title("Crank-Nicolson solution for a simple diffusion problem.")
plt.xlabel("time")
plt.ylabel("position")
plt.show()
