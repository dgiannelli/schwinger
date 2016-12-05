/*
 * Main program for generating field configurations and measuring observables
 * Reads the parameters in the file specified as argv
 */

#include <assert.h>

#include "random.h"
#include "lattice.h"

int main(int argc, char *argv[])
{
    assert(argc == 2);
    RndInit();

    NewLattice(argv[1]);
    GetMeasures();
    DeleteLattice();

    RndFinalize();

    return 0;
}
