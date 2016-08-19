#include <stdio.h>
#include <stdlib.h>

#include "lattice.h"

#define N 20 //Lattice size
#define IMAX 1000 //Number of measurement
#define ITHERM 10 //Number of thermalization iteration

#define BETA 1. //Action beta parameter

int main(int argc, char *argv[])
{
    srand(997);
    SiteType **lattice = NewLattice(N);
    int i;
    double plaquetteSum = 0.;

    FILE *cloverFile = fopen("clover.dat", "w");

    for (i=0; i<ITHERM+IMAX; i++)
    {
        SweepLattice(lattice, BETA, N);
        if (i>=ITHERM) 
        {
            const double clover = GetRandomClover(lattice, N);
            fprintf(cloverFile, "%.10e\n", 1. - clover);
            plaquetteSum += clover;
        }
    }

    fclose(cloverFile);
    DeleteLattice(lattice, N);

    printf("Plaquette energy mean value (1 - Re plaq): %f\n", 1. - plaquetteSum/(IMAX-ITHERM));

    return 0;
}
