#include <stdlib.h>
#include <math.h>

#include "lattice.h"

#define XI ( ((double) rand())/((double) RAND_MAX) )

SiteType **NewLattice(int N)
{
    SiteType **lattice;
    int i, j;
    lattice = (SiteType**) malloc(N*sizeof(SiteType*));
    for (i=0; i<N; i++)
    {
        lattice[i] = (SiteType*) malloc(N*sizeof(SiteType));
    }
    for (i=0; i<N; i++)
    {
        for (j=0; j<N; j++)
        {
            lattice[i][j].rightLink = 0.;
            lattice[i][j].topLink = 0.;
        }
    }
    return lattice;
}

void DeleteLattice(SiteType **lattice, int N)
{
    int i;
    for (i=0; i<N; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

double GetRandomClover(SiteType **lattice, int N)
{
    const int nx = rand()%N;
    const int ny = rand()%N;

    /*
     *      phi2
     *       |
     * phi3 - - phi1
     *       |
     *      phi4
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[nx][ny].topLink;
    const double phi3 = lattice[(nx+N-1)%N][ny].rightLink;
    const double phi4 = lattice[nx][(ny+N-1)%N].topLink;

    double cloverSum = 0.;

    //First petal (topright corner, then counterclockwise)
    cloverSum += cos(phi1-phi2+lattice[(nx+1)%N][ny].topLink-lattice[nx][(ny+1)%N].rightLink);

    //Second petal
    cloverSum += cos(phi3+phi2-lattice[(nx+N-1)%N][(ny+1)%N].rightLink-lattice[(nx+N-1)%N][ny].topLink);

    //Third petal
    cloverSum += cos(phi4-phi3+lattice[(nx+N-1)%N][(ny+N-1)%N].rightLink-lattice[(nx+N-1)%N][(ny+N-1)%N].topLink);

    //Fourth petal
    cloverSum += cos(-phi1-phi4+lattice[nx][(ny+N-1)%N].rightLink+lattice[(nx+1)%N][(ny+N-1)%N].topLink);

    return cloverSum/4.;
}

void SweepLattice(SiteType **lattice, double beta, int N)
{
    int nx,ny;
    for (nx=0; nx<N; nx++)
    {
        for (ny=0; ny<N; ny++)
        {
            RightMetropolis(lattice, nx, ny, beta, N);
        }
    }
    for (nx=0; nx<N; nx++)
    {
        for (ny=0; ny<N; ny++)
        {
            TopMetropolis(lattice, nx, ny, beta, N);
        }
    }
}

void RightMetropolis(SiteType **lattice, int nx, int ny, double beta, int N)
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
    const double phi1 = lattice[(nx+1)%N][ny].topLink;
    const double phi2 = lattice[nx][(ny+1)%N].rightLink;
    const double phi3 = lattice[nx][ny].topLink;
    const double phi4 = lattice[nx][(ny+N-1)%N].topLink;
    const double phi5 = lattice[nx][(ny+N-1)%N].rightLink;
    const double phi6 = lattice[(nx+1)%N][(ny+N-1)%N].topLink;

    const double phiBar = 0.5*(phi1-phi2-phi3+phi4-phi5-phi6);

    const double phi = lattice[nx][ny].rightLink;
    const double phiNew = 2.*M_PI*XI;

    if ( XI < exp(2.*beta*( cos(phiNew + phiBar) - cos(phi + phiBar) )) )
    {
        lattice[nx][ny].rightLink = phiNew;
    }
}

void TopMetropolis(SiteType **lattice, int nx, int ny, double beta, int N)
{
    /*
     *      phi4  phi3
     *       -     -
     * phi5 | | phi | phi2
     *       -     -
     *      phi6  phi1
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[(nx+1)%N][ny].topLink;
    const double phi3 = lattice[nx][(ny+1)%N].rightLink;
    const double phi4 = lattice[(nx+N-1)%N][(ny+1)%N].rightLink;
    const double phi5 = lattice[(nx+N-1)%N][ny].topLink;
    const double phi6 = lattice[(nx+N-1)%N][ny].rightLink;

    const double phiBar = 0.5*(phi1+phi2-phi3+phi4+phi5-phi6);

    const double phi = lattice[nx][ny].topLink;
    const double phiNew = 2.*M_PI*XI;

    if ( XI < exp(2.*beta*( cos(phiNew - phiBar) - cos(phi - phiBar) )) )
    {
        lattice[nx][ny].topLink = phiNew; 
    }
}
