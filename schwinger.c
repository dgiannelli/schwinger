#include <stdio.h>
#include <stdlib.h>

#include "lattice.h"

#define N 10 //Lattice size
#define IMAX 100 //Total iterations
#define ITHERM 20 //Number of iters needed to thermalize

#define BETA 1. //Action beta parameter

int main(int argc, char *argv[])
{
    srand(997);
    SiteType **lattice = NewLattice(N);
    int i;
    double plaquetteSum = 0.;

    FILE *cloverFile = fopen("clover.dat", "w");

    for (i=0; i<IMAX; i++)
    {
        SweepLattice(lattice, BETA, N);
        const double clover = GetRandomClover(lattice, N);
        fprintf(cloverFile, "%e\n", 1. - clover);
        if (i>=ITHERM) plaquetteSum += clover;
    }

    fclose(cloverFile);
    DeleteLattice(lattice, N);

    printf("Plaquette energy mean value (1 - Re plaq): %f\n", 1. - plaquetteSum/(IMAX-ITHERM));

    return 0;
}
