#ifndef LATTICE_H
#define LATTICE_H

typedef struct SiteStruct
{
    double rightLink;
    double topLink;
    double plaquette;
} SiteType;

SiteType **NewLattice(int N);

void DeleteLattice(SiteType **lattice, int N);

double GetRandomClover(SiteType **lattice, int N);

void SweepLattice(SiteType **lattice, double beta, int N);

void RightMetropolis(SiteType **lattice, int nx, int ny, double beta, int N);

void TopMetropolis(SiteType **lattice, int nx, int ny, double beta, int N);

#endif
