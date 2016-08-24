/*
 * In this file are defined the functions to allocate/free the lattice,
 * and all the operation on the lattice that are used in the main program.
 */

#include <stdlib.h>
#include <math.h>

#include "lattice.h"
#include "sampling.h"

SiteType **NewLattice(int n)
{
    SiteType **lattice;
    int i, j;
    lattice = (SiteType**) malloc(n*sizeof(SiteType*));
    for (i=0; i<n; i++)
    {
        lattice[i] = (SiteType*) malloc(n*sizeof(SiteType));
    }
    for (i=0; i<n; i++)
    {
        for (j=0; j<n; j++)
        {
            lattice[i][j].rightLink = 0.;
            lattice[i][j].topLink = 0.;
        }
    }
    return lattice;
}

void DeleteLattice(SiteType **lattice, int n)
{
    int i;
    for (i=0; i<n; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

double GetPlaquetteMean(SiteType **lattice, int n)
{
    int nx, ny;
    double plaquetteSum = 0.;
    
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            plaquetteSum += cos(lattice[nx][ny].rightLink - lattice[nx][ny].topLink +\

                            lattice[(nx+1)%n][ny].topLink - lattice[nx][(ny+1)%n].rightLink);
        }
    }
    return plaquetteSum/(n*n);
}

void SweepLattice(SiteType **lattice, double beta, int n)
{
    int nx,ny;
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            SampleRightLink(lattice, nx, ny, beta, n);
        }
    }
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            SampleTopLink(lattice, nx, ny, beta, n);
        }
    }
}

