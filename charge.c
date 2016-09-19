/*
 * Main program for generating field configurations and measuring the topological charge
 * with torus boundary conditions.
 * The program will run a simulation with different initial values in order to study the finite volume
 * behaviour and the scaling study at constant physics.
 * This program will save data in the folders data/torus/charge/fixed and (...)/phys. They must exist
 * before running the program.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "lattice.h"
#include "random.h"

#define ITERS 10000


int main()
{
    SetObservableCharge();
    RndInit();

    puts("Fixed beta = 1 and different values of N:");
    char filename[40];

    {
        const double betas[] = {1., 1., 1., 1.};
        const int ns[] = {5, 10, 20, 40};

        for (int i=0; i<4; i++)
        {
            sprintf(filename, "data/torus/charge/fixed/b%.1fn%02i.dat", betas[i], ns[i]);
            FILE *file = fopen(filename, "w"); assert(file);
            NewLattice(betas[i], ns[i], 1);

            GetMeasurement(ITERS,file);

            DeleteLattice();
            fclose(file);
        }
    }

    puts("Different values of beta,N such at constant physics:");

    {
        const double betas[] = {0.8, 1.8, 3.2, 5.0, 7.2, 9.8};
        const int ns[] = {8, 12, 16, 20, 24, 28};

        for (int i=0; i<6; i++)
        {
            sprintf(filename, "data/torus/charge/phys/b%.1fn%02i.dat", betas[i], ns[i]);
            FILE *file = fopen(filename, "w"); assert(file);
            NewLattice(betas[i], ns[i], 1);

            GetMeasurement(ITERS,file);

            DeleteLattice();
            fclose(file);
        }
    }

    RndFinalize();

    return 0;
}
