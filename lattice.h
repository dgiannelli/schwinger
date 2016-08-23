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

void RightMetropolis(SiteType **lattice, int nx, int ny, double beta, int n);

void TopMetropolis(SiteType **lattice, int nx, int ny, double beta, int n);

#endif
