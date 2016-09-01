/*
 * In this file is defined the struct of the lattice element and are declared 
 * all the lattice operations that are called in the main program.
 */

#ifndef LATTICE_H
#define LATTICE_H

typedef struct SiteStruct
{
    double rightLink;
    double topLink;
} SiteType;

// Allocate the memory for the lattice and initialize every link angle to 0:
SiteType **NewLattice(int n);

// Free the mamory allocated:
void DeleteLattice(SiteType **lattice, int n);

// Compute the mean value of all plaquettes in the present configuration:
double GetPlaquetteMean(SiteType **lattice, int n);

// Compute the topological charge in the present configuration:
double GetCharge(SiteType **lattice, int n);

// Perform a sweep of the lattice resampling every link (first all right links, then all top links):
void SweepLattice(SiteType **lattice, double beta, int n);

// Shift the value of x in order to fit into the interval (-pi,pi]:
double FitInterval(double x);

#endif
