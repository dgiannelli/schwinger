#include <math.h>

#include "lattice.h"
#include "random.h"

void SampleRightLink(SiteType **lattice, int nx, int ny, double beta, int n)
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
     * Metropolis-Hastings step to change phi proposing phiNew.
     * For more information, see factSheet.pdf
     */
    const double phi1 = lattice[(nx+1)%n][ny].topLink;
    const double phi2 = lattice[nx][(ny+1)%n].rightLink;
    const double phi3 = lattice[nx][ny].topLink;
    const double phi4 = lattice[nx][(ny+n-1)%n].topLink;
    const double phi5 = lattice[nx][(ny+n-1)%n].rightLink;
    const double phi6 = lattice[(nx+1)%n][(ny+n-1)%n].topLink;

    const double phiA = phi1 - phi2 - phi3;
    const double phiB = phi4 - phi5 - phi6;
    const double phiBar = 0.5*(phiA + phiB);
    const double phi = lattice[nx][ny].rightLink;

    const double a = exp(-beta*M_PI*M_PI);
    const double E = a + ( 1. - a ) * RndUniform();
    const double phiNew = sqrt(-log(E)/beta)*cos(2.*M_PI*RndUniform()) - phiBar;

    const double rate = exp(beta*( +cos(phiNew+phiA) + cos(phiNew+phiB) + pow( (phiNew+phiBar) , 2 ) \
                                   -cos(phi   +phiA) - cos(phi   +phiB) - pow( (phi   +phiBar) , 2 ) ));

    if ( RndUniform() < rate )
    {
        lattice[nx][ny].rightLink = phiNew;
    }
}

void SampleTopLink(SiteType **lattice, int nx, int ny, double beta, int n)
{
    /*
     *      phi4  phi3
     *       -     -
     * phi5 | | phi | phi2
     *       -     -
     *      phi6  phi1
     *
     * Metropolis-Hastings step to change phi proposing phiNew.
     * For more information, see factSheet.pdf
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[(nx+1)%n][ny].topLink;
    const double phi3 = lattice[nx][(ny+1)%n].rightLink;
    const double phi4 = lattice[(nx+n-1)%n][(ny+1)%n].rightLink;
    const double phi5 = lattice[(nx+n-1)%n][ny].topLink;
    const double phi6 = lattice[(nx+n-1)%n][ny].rightLink;

    const double phiA = phi1 + phi2 - phi3;
    const double phiB = phi4 + phi5 - phi6;
    const double phiBar = 0.5*(phiA + phiB);
    const double phi = lattice[nx][ny].topLink;

    const double a = exp(-beta*M_PI*M_PI);
    const double E = a + ( 1. - a ) * RndUniform();
    const double phiNew = sqrt(-log(E)/beta)*cos(2.*M_PI*RndUniform()) + phiBar;

    const double rate = exp(beta*( +cos(phiNew-phiA) + cos(phiNew-phiB) + pow( (phiNew-phiBar) , 2 ) \
                                   -cos(phi   -phiA) - cos(phi   -phiB) - pow( (phi   -phiBar) , 2 ) ));

    if ( RndUniform() < rate )
    {
        lattice[nx][ny].topLink = phiNew; 
    }
}
