#include <math.h>

#include "lattice.h"
#include "random.h"

int succ = 0;
int total = 0;

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

    const double reS = cos(phiA) + cos(phiB);
    const double imS = sin(phiA) + sin(phiB);
    const double argS = atan2(imS,reS);
    const double k = hypot(reS,imS);

    const double E = (1. - exp(-0.5*beta*k*M_PI*M_PI)) * RndUniform() + exp(-0.5*beta*k*M_PI*M_PI);
    const double phi0New = sqrt(-2./beta/k*log(E))*cos(2.*M_PI*RndUniform());

    double phi0 = lattice[nx][ny].rightLink + argS;
    phi0 = atan2(sin(phi0),cos(phi0)); //For fitting inside (-pi,pi]

    const double rate = exp(beta*k*( cos(phi0New) + 0.5*pow(phi0New,2) \
                                    -cos(phi0   ) - 0.5*pow(phi0   ,2) ));

    if (RndUniform() < rate)
    {
        const double phiNew = phi0New - argS;
        lattice[nx][ny].rightLink = atan2(sin(phiNew),cos(phiNew));
        succ++;
    }
    total++;
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

    const double reS = cos(-phiA) + cos(-phiB);
    const double imS = sin(-phiA) + sin(-phiB);
    const double argS = atan2(imS,reS);
    const double k = hypot(reS,imS);

    const double E = (1. - exp(-0.5*beta*k*M_PI*M_PI)) * RndUniform() + exp(-0.5*beta*k*M_PI*M_PI);
    const double phi0New = sqrt(-2./beta/k*log(E))*cos(2.*M_PI*RndUniform());

    double phi0 = lattice[nx][ny].topLink + argS;
    phi0 = atan2(sin(phi0),cos(phi0));
    const double rate = exp(beta*k*( cos(phi0New) + 0.5*pow(phi0New,2) \
                                    -cos(phi0   ) - 0.5*pow(phi0   ,2) ));

    if (RndUniform() < rate)
    {
        const double phiNew = phi0New - argS;
        lattice[nx][ny].topLink = atan2(sin(phiNew),cos(phiNew));
        succ++;
    }
    total++;
}

