/*
 * In this file are declared all lattice functions that are vivsible in the main program.
 */

#ifndef LATTICE_H
#define LATTICE_H

// Allocate memory for the lattice and initialize it opening the parameter file called paramFname
// The initial configuration is a hot configuration
void NewLattice(char *paramFname);

// Free the memory allocated
void DeleteLattice(void);

// Sample different field configuration and save measurements
void GetMeasures(void);

#endif
