#include <stdio.h>
#include <stdlib.h>

#include "lattice.h"

#define N 20 //Lattice size
#define IMAX 10000 //Number of measurement
#define ITHERM 10 //Number of thermalization iteration

#define BETA 4. //Action beta parameter

int main(int argc, char *argv[])
{
    srand(577);
    SiteType **lattice = NewLattice(N);
    double plaquette;
    int i;

    FILE *plaquetteFile = fopen("plaquette.dat", "w");

    for (i=0; i<ITHERM+IMAX; i++)
    {
        SweepLattice(lattice, BETA, N);
        if (i>=ITHERM) 
        {
            plaquette = GetPlaquetteMean(lattice, N);
            fprintf(plaquetteFile, "%.15e\n", plaquette);
        }
    }

    fclose(plaquetteFile);
    DeleteLattice(lattice, N);

    printf("\n**** Saved in plaquette.dat %i plaquette measures at beta = %.2f ****\n\n", IMAX, BETA);

    return 0;
}
