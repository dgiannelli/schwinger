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
    double plaquette;
} SiteType;

SiteType **NewLattice(int n);

void DeleteLattice(SiteType **lattice, int n);

double GetPlaquetteMean(SiteType **lattice, int n);

void SweepLattice(SiteType **lattice, double beta, int n);

#endif
