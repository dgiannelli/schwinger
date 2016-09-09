/*
 * In this file are defined the functions to allocate/free the lattice,
 * and all the operation on the lattice that are used in the main program.
 */

#include <stdlib.h>
#include <math.h>

#include "lattice.h"

double FitInterval(double x)
{
    return x - floor( (x+M_PI) / (2.*M_PI) ) * 2.*M_PI;
}

void GetMeasurement(Lattice lattice, const char *observable, int measures);

void SweepLattice(Lattice lattice);

double GetPlaquetteMean(Lattice lattice);

double GetCharge(Lattice lattice);

double (*GetObservable)(Lattice lattice);

void SampleRightLinkMetropolisHastings(Lattice lattice, int nx, int ny);

void SampleTopLinkMetropolisHastings(Lattice lattice, int nx, int ny);

void SampleRightLinkMetropolis(Lattice lattice, int nx, int ny);

void SampleTopLinkMetropolis(Lattice lattice, int nx, int ny);

void (*SampleRightLink)(Lattice lattice, int nx, int ny) = SampleRightLinkMetropolisHastings;

void (*SampleTopLink)(Lattice lattice, int nx, int ny) = SampleTopLinkMetropolisHastings;


Lattice NewLattice(double beta, int n)
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

void DeleteLattice(Lattice lattice)
{
    int i;
    for (i=0; i<n; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

void GetMeasurement(Lattice lattice, const char *observable, int measures)
{
    if (argc != 4) {printf("**** ERROR: Wrong number args: is %i, should be 3\n", argc-1); return 1;}
    const double beta = atof(argv[1]);
    const int n = atoi(argv[2]);

    RndInit();
    SiteType **lattice = NewLattice(n);

    FILE *chargeFile = fopen(argv[3], "w");
    fprintf(chargeFile, "#beta = %f\n#N = %i\n", beta, n);

    for (int i=0; i<n+IMAX; i++) //n is used also as number of thermaization iterations
    {
        SweepLattice(lattice, beta, n);
        if (i>=n) 
        {
            const double charge = GetCharge(lattice, n);
            fprintf(chargeFile, "%+.0f\n", charge);
        }
    }

    fclose(chargeFile);
    DeleteLattice(lattice, n);
    RndFinalize();

    printf("\n**** Saved in %s %i charge measures at beta = %.1f with lattice size %i ****\n\n", \
            argv[3], IMAX, beta, n);
        printf("Acceptance ratio: %f\n", (float)succ/total);

    return 0;
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

double GetPlaquetteMean(SiteType **lattice, int n)
{
    int nx, ny;
    double plaquetteSum = 0.;
    
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            plaquetteSum += cos( lattice[nx][ny].rightLink - lattice[nx][ny].topLink \

                               + lattice[(nx+1)%n][ny].topLink - lattice[nx][(ny+1)%n].rightLink);
        }
    }
    return plaquetteSum/(n*n);
}

double GetCharge(SiteType **lattice, int n)
{
    int nx, ny;
    double charge = 0.;

    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            charge += FitInterval( lattice[nx][ny].rightLink - lattice[nx][ny].topLink \
                                 + lattice[(nx+1)%n][ny].topLink - lattice[nx][(ny+1)%n].rightLink);
        }
    }
    return charge/2./M_PI;
}

void SampleRightLinkMetropolisHastings(SiteType **lattice, int nx, int ny, double beta, int n)
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

    const double phi0 = FitInterval( lattice[nx][ny].rightLink + argS );

    const double rate = exp(beta*k*( cos(phi0New) + 0.5*pow(phi0New,2) \
                                    -cos(phi0   ) - 0.5*pow(phi0   ,2) ));

    if (RndUniform() < rate)
    {
        lattice[nx][ny].rightLink = FitInterval( phi0New - argS );
        succ++;
    }
    total++;
}

void SampleTopLinkMetropolisHastings(SiteType **lattice, int nx, int ny, double beta, int n)
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

    const double phi0 = FitInterval( lattice[nx][ny].topLink + argS );

    const double rate = exp(beta*k*( cos(phi0New) + 0.5*pow(phi0New,2) \
                                    -cos(phi0   ) - 0.5*pow(phi0   ,2) ));

    if (RndUniform() < rate)
    {
        lattice[nx][ny].topLink = FitInterval( phi0New - argS );
        succ++;
    }
    total++;
}

void SampleRightLinkMetropolis(SiteType **lattice, int nx, int ny, double beta, int n)
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

    const double phiA = phi1 - phi2 - phi3;
    const double phiB = phi4 - phi5 - phi6;
    const double phi = lattice[nx][ny].rightLink;
    const double phiNew = 2.*M_PI*( RndUniform() - 0.5 );

    const double rate = exp(beta*( +cos(phiNew+phiA) + cos(phiNew+phiB) \
                                   -cos(phi   +phiA) - cos(phi   +phiB) ));

    if ( RndUniform() < rate )
    {
        lattice[nx][ny].rightLink = phiNew;
        succ++;
    }
    total++;
}

void SampleTopLinkMetropolis(SiteType **lattice, int nx, int ny, double beta, int n)
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

    const double phiA = phi1 + phi2 - phi3;
    const double phiB = phi4 + phi5 - phi6;
    const double phi = lattice[nx][ny].topLink;
    const double phiNew = 2.*M_PI*( RndUniform() - 0.5 );

    const double rate = exp(beta*( +cos(phiNew-phiA) + cos(phiNew-phiB) \
                                   -cos(phi   -phiA) - cos(phi   -phiB) ));

    if ( RndUniform() < rate )
    {
        lattice[nx][ny].topLink = phiNew; 
        succ++;
    }
    total++;
}

double GetAcceptance(Lattice lattice)
{



