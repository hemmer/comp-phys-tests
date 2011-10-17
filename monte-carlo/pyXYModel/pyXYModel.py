#!/usr/bin/env python

from math import pi
from time import time

import numpy as np
from pylab import *


# float version of xrange
def drange(start, stop, step):
    r = start
    while r < stop:
        yield r
        r += step


class Mean(object):

    def __init__(self):
        self.reset()

    def add_val(self, val):
        self.total += val
        self.totalSquared += val * val
        self.N += 1

    def get_mean(self):
        mean = self.total / float(self.N)
        sd = np.sqrt(self.totalSquared / float(self.N) - mean ** 2)
        return (mean, sd)

    def reset(self):
        self.total = 0
        self.N = 0
        self.totalSquared = 0.0


class XYModel(object):

    def __init__(self, L=50):

        self.TWOPI = 2 * pi                     # 2*pi constant
        self.J0 = 1.0                           # J_0 coupling constant
        self.L = L                              # length of grid
        self.num_spins = L * L                  # number of spins

        # initialise grid to spins in [0, 2pi]
        #self.grid = np.random.rand(L, L) * self.TWOPI
        self.grid = np.zeros((L, L))

        np.random.seed(42)       # seed for debugging
        self.delta = 0.5                        # angle increment

        # boolean "mask" index arrays for checkerboard pattern
        coords = np.ogrid[0:L, 0:L]
        self.BLACK = (coords[0] + coords[1]) % 2 == 1
        self.WHITE = ~self.BLACK

        # instructions for moving arrays in nearest neighbour detection
        self.NNMOVES = ((1, 1), (-1, 1), (1, 0), (-1, 0))

        # Print informative header
        print "XY Model with lattice size", L

    def update_grid(self, T):
        num_accepted = self.update_checkerboard(T, True)
        num_accepted += self.update_checkerboard(T, False)

        # return the total number of sucessful flips
        return num_accepted

    def update_checkerboard(self, T, use_black_squares):

        # update scheme requires updating
        # only a checkerboard pattern at a time
        checkboardmask = self.BLACK if use_black_squares else self.WHITE

        # we need to generate random increments for every 2nd square
        num_squares = self.num_spins / 2

        # generate angle increments for the grid on [-delta, delta]
        randangle = self.delta * (np.random.rand(num_squares) * 2 - 1)

        # and add these to the existing grid
        proposed_grid = np.zeros((self.L, self.L))
        proposed_grid[checkboardmask] = self.grid[checkboardmask] + randangle

        # find the cosine of the difference between the current site
        # and each of the nearest neighbours, and sum those contributions
        before = np.zeros((self.L, self.L))
        after = np.zeros((self.L, self.L))
        for dir, ax in self.NNMOVES:
            # roll the grid (wrapping round) to move neighbouring
            # sites onto the current index
            shift = np.roll(self.grid, dir, ax)
            before += np.cos(self.grid - shift)
            after += np.cos(proposed_grid - shift)

        # dE is the energy change after making the proposed
        # change to each angle
        delta_E = (after - before) * -self.J0

        # get boolean index of sites for which an angle change
        # is energetically favourable (i.e dE < 0) and update them
        favourable = delta_E < 0.0
        unfavourable = ~favourable
        # make sure that from these we are only
        # selecting the checkerboard squares
        favourable &= checkboardmask
        unfavourable &= checkboardmask

        # update those checkerboard squares for which dE < 0
        self.grid[favourable] = proposed_grid[favourable]

        # the remaining unfavourable sites can still make
        # the change with boltzmann probability
        num_unfav_sites = np.sum(unfavourable)

        # probs is an array of random numbers on [0, 1)
        # against which the Boltzmann probabilities are tested
        probs = np.random.rand(num_unfav_sites)
        boltz_mask = np.zeros((self.L, self.L), dtype=np.bool)
        boltz_mask[unfavourable] = np.exp(-delta_E[unfavourable] / T) > probs
        # update sites that pass the boltzmann probability test
        self.grid[boltz_mask] = proposed_grid[boltz_mask]

        # return the number of cells changed so we
        # can track the acceptance rate
        return np.sum(favourable) + np.sum(boltz_mask)

    def get_magnetisation(self):

        # get a and y componants of magnetisation
        # remembering to normalise wrt number of spins
        mx = np.sum(np.cos(self.grid)) / self.num_spins
        my = np.sum(np.sin(self.grid)) / self.num_spins
        # and return the norm of these
        return np.sqrt(mx ** 2 + my ** 2)

    def get_energy(self):
        # shift the whole lattice right/down (wrapping round)
        right_shift = np.cos(self.grid - np.roll(self.grid, 1, 0))
        down_shift = np.cos(self.grid - np.roll(self.grid, 1, 1))

        energies = -self.J0 * (right_shift + down_shift)
        return np.sum(energies) / float(self.num_spins)

    # main logic loop
    def main(self):

        warmup = 1000       # number to reach equilibrium
        mag_stats = Mean()      # track average magnetisation
        energy_stats = Mean()      # track average energy
        start = time()      # start timer


        temperature_range = np.arange(0.1, 1.5, 0.1)
        m_values = []
        e_values = []

        # loop over possible temperatures
        for T in temperature_range:

            self.delta = 0.5        # reset angle increment to default
            mag_stats.reset()           # reset mean magnetisation tracker
            energy_stats.reset()           # reset mean energy tracker

            # warmup the system
            for i in xrange(warmup):
                self.update_grid(T)

            # main loop for measurements
            for j in xrange(1000):

                # update grid and automatically correct delta
                # to keep acceptance rate around 50%
                accept_rate = self.update_grid(T) / float(self.num_spins)
                if accept_rate > 0.5:
                    self.delta += 0.05
                else:
                    self.delta -= 0.05

                if j % 100:
                    mag_stats.add_val(self.get_magnetisation())
                    energy_stats.add_val(self.get_energy())

            # output results
            meanM, errorM = mag_stats.get_mean()
            meanE, errorE = energy_stats.get_mean()
            m_values.append(meanM)
            e_values.append(meanE)

            print T, meanM, errorM, meanE, errorE

        # finish timing
        end = time()
        print "Ran in", (end - start), "secs."

        # plot results
        plot(temperature_range, m_values, label="$\langle M\\rangle$")
        plot(temperature_range, e_values, label="$\langle E\\rangle$")
        legend()
        title("Plots of energy and magnetisation against T")
        show()

    # plot using motplotlib
    def plot_grid(self):
        plotx = np.sin(self.grid)
        ploty = np.cos(self.grid)

        quiver(plotx, ploty)
        draw()

sim = XYModel(32)
sim.main()
