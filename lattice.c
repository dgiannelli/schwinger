#include <stdlib.h>
#include <math.h>

#include "lattice.h"

#define XI ( ((double) rand())/((double) RAND_MAX) )

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

                            lattice[(nx+1)%n][ny].topLink - lattice[nx][(ny+1)].rightLink);
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
            RightMetropolis(lattice, nx, ny, beta, n);
        }
    }
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            TopMetropolis(lattice, nx, ny, beta, n);
        }
    }
}

void RightMetropolis(SiteType **lattice, int nx, int ny, double beta, int n)
{
    /*
     *       phi2
     *        -
     *  phi3 | | phi1
     *       phi
     *        -
     *  phi4 | | phi6
     *       phi5
     *        -
     *
     * Metropolis step to change phi proposing phiNew.
     * For more information, see factSheet.pdf
     */
    const double phi1 = lattice[(nx+1)%n][ny].topLink;
    const double phi2 = lattice[nx][(ny+1)%n].rightLink;
    const double phi3 = lattice[nx][ny].topLink;
    const double phi4 = lattice[nx][(ny+n-1)%n].topLink;
    const double phi5 = lattice[nx][(ny+n-1)%n].rightLink;
    const double phi6 = lattice[(nx+1)%n][(ny+n-1)%n].topLink;

    const double phi = lattice[nx][ny].rightLink;
    const double phiNew = 2.*M_PI*XI;

    const double DeltaS = 2.*beta*( -cos(phiNew+phi1-phi2-phi3) - cos(phi5+phi6-phiNew-phi4) \
                                    +cos(phi   +phi1-phi2-phi3) + cos(phi5+phi6-phi   -phi4) );

    if ( XI < exp(-DeltaS) )
    {
        lattice[nx][ny].rightLink = phiNew;
    }
}

void TopMetropolis(SiteType **lattice, int nx, int ny, double beta, int n)
{
    /*
     *      phi4  phi3
     *       -     -
     * phi5 | | phi | phi2
     *       -     -
     *      phi6  phi1
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[(nx+1)%n][ny].topLink;
    const double phi3 = lattice[nx][(ny+1)%n].rightLink;
    const double phi4 = lattice[(nx+n-1)%n][(ny+1)%n].rightLink;
    const double phi5 = lattice[(nx+n-1)%n][ny].topLink;
    const double phi6 = lattice[(nx+n-1)%n][ny].rightLink;

    const double phi = lattice[nx][ny].topLink;
    const double phiNew = 2.*M_PI*XI;

    const double DeltaS = 2.*beta*( -cos(phi1+phi2-phi3-phiNew) - cos(phi6+phiNew-phi4-phi5) \
                                    +cos(phi1+phi2-phi3-phi   ) + cos(phi6+phi   -phi4-phi5) );

    if ( XI < exp(-DeltaS) )
    {
        lattice[nx][ny].topLink = phiNew; 
    }
}
