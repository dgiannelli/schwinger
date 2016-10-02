/*
 * Main program for generating field configurations
 */

#include <assert.h>

#include "lattice.h"
#include "random.h"

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

