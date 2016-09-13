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

    double betas[4] = {1., 1., 1., 1.};
    int ns = {5, 10, 20, 40};
    char dir[30] = "data/torus/charge/fixed/";
    char filename[40];

    for (int i=0; i<4; i++)
    {
        sprintf(filename, "b%.1fn%02i.dat", betas[i], ns[i]);
        FILE *file = fopen(strcat(dir,filename), "w");
        assert(file);

        NewLattice(betas[i], ns[i]);
        GetMeasurement(ITERS, file);
        DeleteLattice();

        fclose(file);
    }

    /*puts("Different values of beta,N such at constant physics:");

    char dir[30] = "data/torus/charge/phys/";
    char filename[40] = "";
    NewLattice(1.,5);
    FILE *file = fopen("data/torus/charge/fixed/b1.0n05.dat", "w");
    assert(file);
    GetMeasurement(ITERS, file);
    fclose(file);
    DeleteLattice();
    RndFinalize();*/

    return 0;
}
