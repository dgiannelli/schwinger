#include <stdlib.h>

#include "lattice.h"

SiteType **NewLattice(int L)
{
    SiteType **lattice;
    int i, j;
    lattice = (SiteType**) malloc(L*sizeof(SiteType*));
    for (i=0; i<L; i++)
    {
        lattice[i] = (SiteType*) malloc(L*sizeof(SiteType));
    }
    for (i=0; i<L; i++)
    {
        for (j=0; j<L; j++)
        {
            lattice[i][j].rightLink = 0.
            lattice[i][j].topLink = 0.
        }
    }

    return lattice;
}

void DeleteLattice(SiteType **lattice, int L)
{
    int i;
    for (i=0; i<L; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

double GetAction(SiteType **lattice, int L)
{
    return S;
}

double GetPlaquetteMean(SiteType **lattice, int L)
{
    return PlaquetteSum/(L*L);
}
