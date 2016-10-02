/*
 * In this file are declared all the lattice functions that are called in the main program.
 */

#ifndef LATTICE_H
#define LATTICE_H

#include <stdio.h>

// Allocate memory for the lattice and initialize all parameters
// reading from params/paramName
void NewLattice(const char *paramName);

// Free the memory allocated:
void DeleteLattice();

// Sample different field configuration and save measurements in a file:
void GetMeasures();

#endif
