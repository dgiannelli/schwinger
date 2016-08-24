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
    const double phiNew = 2.*M_PI*RndUniform();

    const double DeltaS = beta*( -cos(phiNew+phi1-phi2-phi3) - cos(phi5+phi6-phiNew-phi4) \
                                 +cos(phi   +phi1-phi2-phi3) + cos(phi5+phi6-phi   -phi4) );

    if ( RndUniform() < exp(-DeltaS) )
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
     * Metropolis step to change phi proposing phiNew.
     * For more information, see factSheet.pdf
     */
    const double phi1 = lattice[nx][ny].rightLink;
    const double phi2 = lattice[(nx+1)%n][ny].topLink;
    const double phi3 = lattice[nx][(ny+1)%n].rightLink;
    const double phi4 = lattice[(nx+n-1)%n][(ny+1)%n].rightLink;
    const double phi5 = lattice[(nx+n-1)%n][ny].topLink;
    const double phi6 = lattice[(nx+n-1)%n][ny].rightLink;

    const double phi = lattice[nx][ny].topLink;
    const double phiNew = 2.*M_PI*RndUniform();

    const double DeltaS = beta*( -cos(phi1+phi2-phi3-phiNew) - cos(phi6+phiNew-phi4-phi5) \
                                 +cos(phi1+phi2-phi3-phi   ) + cos(phi6+phi   -phi4-phi5) );

    if ( RndUniform() < exp(-DeltaS) )
    {
        lattice[nx][ny].topLink = phiNew; 
    }
}