/*
 * In this file are declared all the lattice functions that are called in the main program.
 */

#ifndef LATTICE_H
#define LATTICE_H

#include <stdio.h>

// Allocate memory for the lattice and initialize beta and n.
// After initialising, 1000 sweeps are performed to thermalize.
// boundsName = "torus" or "moebius"
// obsName = "plaquette" of "charge"
void NewLattice(double beta, int n, int therm, const char *boundsName, const char *obsName);

// Free the memory allocated:
void DeleteLattice();

// Sample different field configuration and save measurements in a file:
void GetMeasures(double *data, int sweeps);

#endif
