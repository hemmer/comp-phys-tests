#!/usr/bin/env python

from mpl_toolkits.mplot3d import axes3d
import matplotlib.pyplot as plt

import numpy as np
import scipy as sp
import scipy.sparse

# number of spacial points, number of time points
N, Nt = 100, 50
a = 0.5

# store results as we go for plotting later
grid = np.zeros((N, Nt))

# vector for concentration
u = np.zeros(N)
u[int(N/3.0)] = 150
u[int(2*N/3.0)] = 150

coeff = (1 + 2*a) * np.eye(N)
coeff[np.arange(N-1), np.arange(N-1)+1] = -a
coeff[np.arange(N-1) + 1, np.arange(N-1)] = -a

# main solving loop
for t in xrange(Nt):
    grid[:, t] = u
    u = np.linalg.solve(coeff, u)

# plot the results
plt.clf()
plt.imshow(grid)
plt.colorbar()
plt.show()

