/*
 * Main program for generating field configurations and measuring the topological charge
 * Requires the values for beta and lattice size
 */

#include <stdio.h>
#include <stdlib.h>

#include "lattice.h"
#include "random.h"


#define IMAX 1000 //Number of measurement at fixed volume

extern int succ, total;

int main(int argc, char *argv[])
{
    if (argc != 3) {printf("**** ERROR: Wrong number args: is %i, should be 2\n", argc-1); return 1;}
    const double beta = atof(argv[1]);
    const int n = atoi(argv[2]);

    RndInit();
    SiteType **lattice = NewLattice(n);

    char filename[30]; 
    sprintf(filename, "./data/chargeBeta%3.1fN%2i.dat", beta, n);
    FILE *chargeFile = fopen(filename, "w");

    for (int i=0; i<n+IMAX; i++) //n is used also as number of thermaization iterations
    {
        SweepLattice(lattice, beta, n);
        if (i>=n) 
        {
            const double charge = GetCharge(lattice, n);
            fprintf(chargeFile, "%+.0f\n", charge);
        }
    }

    fclose(chargeFile);
    DeleteLattice(lattice, n);
    RndFinalize();

    printf("\n**** Saved in charge.dat %i charge measures at beta = %.2f with lattice size %i ****\n\n", \
            IMAX, beta, n);
        printf("Acceptance ratio: %f\n", (float)succ/total);

    return 0;
}
