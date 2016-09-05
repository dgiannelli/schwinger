/*
 * Main program for generating field configurations and measuring the topological charge
 * Requires as argvs the values for beta, lattice size and the directory in which save data
 */

#include <stdio.h>
#include <stdlib.h>

#include "lattice.h"
#include "random.h"


#define IMAX 10000 //Number of measurement at fixed volume

extern int succ, total;

int main(int argc, char *argv[])
{
    if (argc != 4) {printf("**** ERROR: Wrong number args: is %i, should be 3\n", argc-1); return 1;}
    const double beta = atof(argv[1]);
    const int n = atoi(argv[2]);

    printf("%s\n",argv[3]);

    RndInit();
    SiteType **lattice = NewLattice(n);

    char filename[30]; 
    sprintf(filename, "%schargeBeta%3.1fN%2i.dat", argv[3], beta, n);
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

    printf("\n**** Saved in %s %i charge measures at beta = %.1f with lattice size %i ****\n\n", \
            filename, IMAX, beta, n);
        printf("Acceptance ratio: %f\n", (float)succ/total);

    return 0;
}
