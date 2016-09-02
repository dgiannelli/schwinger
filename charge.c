/*
 * Main program for generating field configurations and measuring the topological charge
 */

#include <stdio.h>

#include "lattice.h"
#include "random.h"

#define N 20 //Lattice size
#define IMAX 30000 //Number of measurement
#define ITHERM 10 //Number of thermalization iteration

#define BETA 4. //Action beta parameter

extern int succ, total;

int main(int argc, char *argv[])
{
    RndInit();
    SiteType **lattice = NewLattice(N);

    FILE *chargeFile = fopen("charge.dat", "w");

    for (int i=0; i<ITHERM+IMAX; i++)
    {
        SweepLattice(lattice, BETA, N);
        if (i>=ITHERM) 
        {
            const double charge;
            charge = GetCharge(lattice, N);
            fprintf(chargeFile, "%.16e\n", charge);
        }
    }

    fclose(chargeFile);
    DeleteLattice(lattice, N);
    RndFinalize();

    printf("\n**** Saved in charge.dat %i charge measures at beta = %.2f ****\n\n", IMAX, BETA);
    printf("Acceptance ratio: %f\n", (float)succ/total);

    return 0;
}
