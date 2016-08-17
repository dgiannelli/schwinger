#include "lattice.h"

#define L 10
#define N 100

int main(int argc, char *argv[])
{
    SiteType **lattice = NewLattice(L);
    double S;
    int i;

    S = GetAction(lattice, L);

    for (i=0; i<N; i++)
    {
        HeatBathRight(lattice, L);
        HeatBathTop(lattice, L);
        plaquetteSum += GetPlaquetteMean(lattice, L);
    }

    DeleteLattice(lattice, L);

    printf("Plaquette mean value: %lf\n", plaquetteSum/N);

    return 0;
}
