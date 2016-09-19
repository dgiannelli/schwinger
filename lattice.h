/*
 * In this file are declared all the lattice functions that are called in the main program.
 */

#ifndef LATTICE_H
#define LATTICE_H

#include <stdio.h>

// Allocate memory for the lattice and initialize beta, n, and correlation time tau.
// After initialising, 100*tau iterations are performed to thermalize
void NewLattice(double beta, int n, int tau);

// Free the memory allocated:
void DeleteLattice();

// Set the boundary conditions to Moebius (default: torus)
// Has to be called before SetObservableCharge:
void SetBoundaryMoeb();

// Set the program to evaluate different kind of observables (default: plaquette):
void SetObservableCharge();

// Set the program to use a basic Metropolis updating step (default: Metropolis-Hastings):
void SetMetropolis();

// Sample different field configuration and save measurements in a file:
void GetMeasurement(int iters, FILE *file);

#endif
