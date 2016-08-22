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
    int i;

    FILE *cloverFile = fopen("clover.dat", "w");

    for (i=0; i<ITHERM+IMAX; i++)
    {
        SweepLattice(lattice, BETA, N);
        if (i>=ITHERM) 
        {
            const double clover = GetRandomClover(lattice, N);
            fprintf(cloverFile, "%.15e\n", clover);
        }
    }

    fclose(cloverFile);
    DeleteLattice(lattice, N);

    printf("\n**** Saved in clover.dat %i clover measures at beta = %.2f ****\n\n", IMAX, BETA);

    return 0;
}
