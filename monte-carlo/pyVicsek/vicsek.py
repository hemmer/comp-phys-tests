#!/usr/bin/env python

import numpy as np
from pylab import *
import matplotlib.pyplot as plt
import sys

class VicsekModel(object):

    def __init__(self, L=7, eta=2.0, N=300, num_steps=10, r=1.0, dt=1, v=0.03):

        self.L = L              # length of box
        self.halfL = L * 0.5    # used for periodic BCs
        self.N = N          # number of particles
        self.eta = eta      # noise
        self.r2 = r * r     # interaction radius squared
        self.dt = dt        # timestep size
        self.v = v          # magnitude of velocity
        self.num_steps = num_steps

        self.velocities = np.zeros((N, 2))
        self.angles = np.zeros(N)
        self.positions = np.zeros((N, 2))

        # first index is for each particle,
        # second for which of its neighbours are in range
        self.particles_in_range = np.eye(N, dtype=np.bool)

    # initialises positions randomly in a box length L,
    # initialises velocites in random directions with mag v
    def initialise_experiment(self):

        # initialise positions
        self.positions = np.random.rand(N, 2) * L

        # generate random directions on [-pi, pi]
        self.angles = (np.random.rand(self.N) * 2 - 1) * np.pi
        # and use this to find velocity components in x/y directions
        self.velocities[:, 0] = np.sin(self.angles) * self.v
        self.velocities[:, 1] = np.cos(self.angles) * self.v

    # this is where the main experiment is carried out
    def main(self, visual_mode):


        # set up plotting stuff
        if visual_mode:

            plt.ion()
            fig = plt.figure()
            ax = fig.add_subplot(111)
            wframe = None

            # reset positions
            self.initialise_experiment()

            for step in xrange(self.num_steps):

                oldcol = wframe
                wframe = self.plot_grid(ax)
                if oldcol:
                    ax.collections.remove(oldcol)
                plt.draw()

                self.perform_step()

            print "for eta =", self.eta, " v_a =", self.find_avg_norm_vel()

        # otherwise we are in data intensive mode
        else:

            # print header
            print "eta\tv_a"

            for self.eta in arange(0.0, 4.0, 0.2):

                # reset positions
                self.initialise_experiment()

                for step in xrange(self.num_steps):
                    self.perform_step()

                print self.eta, "\t", self.find_avg_norm_vel()

        print "experiment finished"

    # perform one timestep
    def perform_step(self):

        # generate the set of random movements dTheta from [-eta/2, eta/2]
        noise_increments = (np.random.rand(self.N) - 0.5) * self.eta

        # find which particles are within distance r
        self.find_particles_in_range()

        # setup array for saving average directions
        mean_directions = np.zeros(self.N)

        for p in xrange(self.N):
            # get the array of angles for particles in range of particle p
            near_angles = self.angles[self.particles_in_range[p, :]]

            # and average over these
            mean_directions[p] = np.arctan2(np.mean(np.sin(near_angles)),
                                            np.mean(np.cos(near_angles)))

        # new direction is average of surrounding particles + noise
        self.angles = mean_directions + noise_increments

        # using these new directions, we can find the velocity
        # vectors such that all have magnitude v
        self.velocities[:, 0] = np.sin(self.angles) * self.v
        self.velocities[:, 1] = np.cos(self.angles) * self.v

        # finally we can use these to Euler-update the positions
        self.positions = self.positions + self.velocities * self.dt
        # apply periodic boundary conditions
        self.positions = np.mod(self.positions, self.L)

    # Updates the NxN boolean grid of whether particles are in range: first
    # index is for each particle, second for which of its neighbours are in
    # range. As the resulting matrix is symmetric, it doesn't really matter...
    def find_particles_in_range(self):

        # each particle is within its own range, which corresponds
        # to the identity matrix
        self.particles_in_range = np.eye(self.N, dtype=np.bool)

        for p in xrange(self.N):
            for q in xrange(p + 1, self.N):

                # difference vector of the 2 points
                diff = self.positions[p, :] - self.positions[q, :]

                # apply minimum image criteria, i.e. use mirror
                # image if closer
                for dim in xrange(2):
                    while diff[dim] > self.halfL:
                        diff[dim] = diff[dim] - self.L
                    while diff[dim] < -self.halfL:
                        diff[dim] = diff[dim] + self.L


                # get the distance squared to (avoid sqrts)
                dist2 = np.sum(np.power(diff, 2))

                # and see if this is within the allowed range
                in_range = dist2 < (self.r2)
                self.particles_in_range[p, q] = in_range
                self.particles_in_range[q, p] = in_range

    # find the average normalised velocity, v_a
    def find_avg_norm_vel(self):

        mean_v = np.sum(self.velocities, axis=0)
        mag_mean_v = np.power(np.sum(np.power(mean_v, 2)), 0.5)

        return mag_mean_v / (self.N * self.v)

    # plot the swarm using matplotlib
    def plot_grid(self, ax):
        plotx = self.positions[:, 0]
        ploty = self.positions[:, 1]
        plotu = self.velocities[:, 0]
        plotv = self.velocities[:, 1]

        return ax.quiver(plotx, ploty, plotu, plotv)

# this is the main experiment
N, L = 150, 18

rho = N / float(L * L)
eta = 0.1
num_steps = 100
print "num particles:", N
print "system size:", L
print "density:", rho
print "num steps:", num_steps

sim = VicsekModel(L, eta, N, num_steps)


if len(sys.argv) > 1:
    print "using visual mode"
    sim.main(True)
else:
    print "using data intensive mode"
    sim.main(False)

