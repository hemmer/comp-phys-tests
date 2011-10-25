#!/usr/bin/env python

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

import numpy as np
import scipy as sp
import scipy.sparse

# number of spacial points, number of time points
N, Nt = 100, 100
a = 0.25

# store results as we go for plotting later
grid = np.zeros((N, Nt))

# vector for concentration
u = np.zeros(N)
u[int(N * 0.4)] = 100
u[int(N * 0.6)] = 100
u[int(N * 0.1)] = 100

coeff = (1 + 2*a) * np.eye(N)
coeff[np.arange(N-1), np.arange(N-1)+1] = -a
coeff[np.arange(N-1) + 1, np.arange(N-1)] = -a
print coeff

# main solving loop
for t in xrange(Nt):
    u[0] = u[1]
    u[-1] = u[-2]
    grid[:, t] = u
    u = np.linalg.solve(coeff, u)

    print "mass at timestep", t, "is:", np.sum(u[1:-1])

# plot the results
plt.clf()
plt.imshow(grid)
plt.colorbar()
plt.title("Implicit finite difference method for a basic diffusion problem.")
plt.xlabel("time")
plt.ylabel("position")
plt.show()

