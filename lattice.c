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

double GetRandomClover(SiteType **lattice, int n)
{
    const int nx = rand()%n;
    const int ny = rand()%n;

    /*
     *      phi2
     *       |
     * phi3 - - phi1
     *       |
     *      phi4
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[nx][ny].topLink;
    const double phi3 = lattice[(nx+n-1)%n][ny].rightLink;
    const double phi4 = lattice[nx][(ny+n-1)%n].topLink;

    double cloverSum = 0.;

    //First petal (topright corner, then counterclockwise)
    cloverSum += cos(phi1-phi2+lattice[(nx+1)%n][ny].topLink-lattice[nx][(ny+1)%n].rightLink);

    //Second petal
    cloverSum += cos(phi3+phi2-lattice[(nx+n-1)%n][(ny+1)%n].rightLink-lattice[(nx+n-1)%n][ny].topLink);

    //Third petal
    cloverSum += cos(phi4-phi3+lattice[(nx+n-1)%n][(ny+n-1)%n].rightLink-lattice[(nx+n-1)%n][(ny+n-1)%n].topLink);

    //Fourth petal
    cloverSum += cos(-phi1-phi4+lattice[nx][(ny+n-1)%n].rightLink+lattice[(nx+1)%n][(ny+n-1)%n].topLink);

    return cloverSum/4.;
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

    const double phiBar = 0.5*(phi1-phi2-phi3+phi4-phi5-phi6);

    const double phi = lattice[nx][ny].rightLink;
    const double phiNew = phi + 2.*(XI-0.5);

    if ( XI < exp(2.*beta*( cos(phiNew + phiBar) - cos(phi + phiBar) )) )
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

    const double phiBar = 0.5*(phi1+phi2-phi3+phi4+phi5-phi6);

    const double phi = lattice[nx][ny].topLink;
    const double phiNew = phi + 2.*(XI-0.5);

    if ( XI < exp(2.*beta*( cos(phiNew - phiBar) - cos(phi - phiBar) )) )
    {
        lattice[nx][ny].topLink = phiNew; 
    }
}
