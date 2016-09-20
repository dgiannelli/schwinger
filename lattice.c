/*
 * In this file are defined the functions to allocate/free the lattice,
 * and all the operation on the lattice that are used in the main program.
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "lattice.h"
#include "random.h"

// Function that move an angle to an equivalent value in (-pi,pi]:

#define ShiftAngle(x) (-ceil((x-M_PI)/2./M_PI) * 2.*M_PI)
#define FitInterval(x) (x + ShiftAngle(x))

// **** Lattice variables:

static double beta;
static int n;
static int tau;
// Global variables to compute the acceptance ratio of the updating steps:
static int total, succ;


static struct LatticeSite
{
    double rightLink;
    double topLink;
} **lattice;

// **** Implementation only visible functions declarations:

// ** Functions and function pointers to implement one of the boundary conditions (default: torus):

static double GetRightTLTorus(int nx, int ny);
static double GetRightTLMoeb(int nx, int ny);
static double (*GetRightTL)(int nx, int ny) = GetRightTLTorus;

static double GetTopRLTorus(int nx, int ny);
static double GetTopRLMoeb(int nx, int ny);
static double (*GetTopRL)(int nx, int ny) = GetTopRLTorus;

static double GetTopLeftRLTorus(int nx, int ny);
static double GetTopLeftRLMoeb(int nx, int ny);
static double (*GetTopLeftRL)(int nx, int ny) = GetTopLeftRLTorus;

static double GetLeftRLTorus(int nx, int ny);
static double GetLeftRLMoeb(int nx, int ny);
static double (*GetLeftRL)(int nx, int ny) = GetLeftRLTorus;

static double GetLeftTLTorus(int nx, int ny);
static double GetLeftTLMoeb(int nx, int ny);
static double (*GetLeftTL)(int nx, int ny) = GetLeftTLTorus;

static double GetBottomRLTorus(int nx, int ny);
static double GetBottomRLMoeb(int nx, int ny);
static double (*GetBottomRL)(int nx, int ny) = GetBottomRLTorus;

static double GetBottomTLTorus(int nx, int ny);
static double GetBottomTLMoeb(int nx, int ny);
static double (*GetBottomTL)(int nx, int ny) = GetBottomTLTorus;

static double GetBottomRightTLTorus(int nx, int ny);
static double GetBottomRightTLMoeb(int nx, int ny);
static double (*GetBottomRightTL)(int nx, int ny) = GetBottomRightTLTorus;

static char *boundsName = "torus";

// **

// ** Functions and functions pointers to evaluate one of observables (default: plaquette):

static double GetPlaquetteMean();

static double GetChargeTorus();

static double GetChargeMoeb();

static double (*GetObservable)() = GetPlaquetteMean;

static double (*GetCharge)() = GetChargeTorus;

static char *obsName = "plaquette";

// **

static void SweepLattice();

// ** Possible procedures to perform an updating step:

static void SampleRightLinkMetropolisHastings(int nx, int ny);

static void SampleTopLinkMetropolisHastings(int nx, int ny);

static void SampleRightLinkMetropolis(int nx, int ny);

static void SampleTopLinkMetropolis(int nx, int ny);

// Function pointers to two different implementations of updating steps:
static void (*SampleRightLink)(int nx, int ny) = SampleRightLinkMetropolisHastings;

static void (*SampleTopLink)(int nx, int ny) = SampleTopLinkMetropolisHastings;

// **

// **** Implementation of included functions:

void NewLattice(double _beta, int _n, int _tau)
{
    beta = _beta;
    n = _n;
    tau = _tau;
    total = 0;
    succ = 0;

    lattice = malloc(n*sizeof(struct LatticeSite *));
    for (int i=0; i<n; i++)
    {
        lattice[i] = malloc(n*sizeof(struct LatticeSite));
    }
    for (int nx=0; nx<n; nx++)
    {
        for (int ny=0; ny<n; ny++)
        {
            lattice[nx][ny].rightLink = 2.*M_PI*(RndUniform()-0.5);
            lattice[nx][ny].topLink = 2.*M_PI*(RndUniform()-0.5);
        }
    }
    for (int t=0; t<10*tau; t++)
    {
        SweepLattice();
    }
}

void DeleteLattice()
{
    beta = 0.;
    n = 0;
    tau = 0;
    succ = 0;
    total = 0;

    for (int i=0; i<n; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

void SetBoundaryMoeb()
{
    GetCharge = GetChargeMoeb;
    boundsName = "moebius";
    GetRightTL = GetRightTLMoeb;
    GetTopRL = GetTopRLMoeb;
    GetTopLeftRL = GetTopLeftRLMoeb;
    GetLeftRL = GetLeftRLMoeb;
    GetLeftTL = GetLeftTLMoeb;
    GetBottomRL = GetBottomRLMoeb;
    GetBottomTL = GetBottomTLMoeb;
    GetBottomRightTL = GetBottomRightTLMoeb;
}

void SetObservableCharge()
{
    GetObservable = GetCharge;
    obsName = "charge";
}

void SetMetropolisHastings()
{
    SampleRightLink = SampleRightLinkMetropolisHastings;
    SampleTopLink = SampleTopLinkMetropolisHastings;
}

void SetMetropolis()
{
    SampleRightLink = SampleRightLinkMetropolis;
    SampleTopLink = SampleTopLinkMetropolis;
}

void GetMeasurement(int iters, FILE *file)
{
    if (!strcmp(obsName,"charge")) fprintf(file, "#beta = %f\n#N = %i\n", beta, n);

    for (int i=0; i<iters; i++)
    {
        for (int j=0; j<10*tau; j++)
        {
            SweepLattice();
        }
        double observable = GetObservable();
        if (!strcmp(obsName,"plaquette")) fprintf(file, "%+.16e\n", observable);
        else fprintf(file, "%+.0f\n", observable);
    }

    printf("\n**** Saved %i %s measures at beta = %.1f with lattice size %i and %s boundary conditions ****\n\n", iters, obsName, beta, n, boundsName);
    printf("Acceptance ratio: %f\n", (float)succ/total);
}

// **** Implementation of not included functions:

double GetPlaquetteMean()
{
    double plaquetteSum = 0.;
    
    for (int nx=0; nx<n; nx++)
    {
        for (int ny=0; ny<n; ny++)
        {
            plaquetteSum += cos( lattice[nx][ny].rightLink - lattice[nx][ny].topLink \
                               + GetRightTL(nx,ny)         - GetTopRL(nx,ny) );
        }
    }
    return plaquetteSum/(n*n);
}

double GetChargeTorus()
{
    double charge = 0.;

    for (int nx=0; nx<n; nx++)
    {
        for (int ny=0; ny<n; ny++)
        {
            charge += ShiftAngle( lattice[nx][ny].rightLink - lattice[nx][ny].topLink \
                                 + GetRightTL(nx,ny)         - GetTopRL(nx,ny) );
        }
    }
    return charge/2./M_PI;
}

double GetChargeMoeb()
{
    //return fmod(round(GetChargeTorus()),2);
    return GetChargeTorus();
}

void SweepLattice()
{
    int nx, ny;
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            SampleRightLink(nx,ny);
        }
    }
    for (nx=0; nx<n; nx++)
    {
        for (ny=0; ny<n; ny++)
        {
            SampleTopLink(nx,ny);
        }
    }
}

void SampleRightLinkMetropolisHastings(int nx, int ny)
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
    const double phi1 = GetRightTL(nx,ny);
    const double phi2 = GetTopRL(nx,ny);
    const double phi3 = lattice[nx][ny].topLink;
    const double phi4 = GetBottomTL(nx,ny);
    const double phi5 = GetBottomRL(nx,ny);
    const double phi6 = GetBottomRightTL(nx,ny);

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

void SampleTopLinkMetropolisHastings(int nx, int ny)
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
    const double phi2 = GetRightTL(nx,ny);
    const double phi3 = GetTopRL(nx,ny);
    const double phi4 = GetTopLeftRL(nx,ny);
    const double phi5 = GetLeftTL(nx,ny);
    const double phi6 = GetLeftRL(nx,ny);

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

void SampleRightLinkMetropolis(int nx, int ny)
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
    const double phi1 = GetRightTL(nx,ny);
    const double phi2 = GetTopRL(nx,ny);
    const double phi3 = lattice[nx][ny].topLink;
    const double phi4 = GetBottomTL(nx,ny);
    const double phi5 = GetBottomRL(nx,ny);
    const double phi6 = GetBottomRightTL(nx,ny);

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

void SampleTopLinkMetropolis(int nx, int ny)
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
    const double phi2 = GetRightTL(nx,ny);
    const double phi3 = GetTopRL(nx,ny);
    const double phi4 = GetTopLeftRL(nx,ny);
    const double phi5 = GetLeftTL(nx,ny);
    const double phi6 = GetLeftRL(nx,ny);

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

double GetRightTLTorus(int nx, int ny)
{
    return lattice[(nx+1)%n][ny].topLink;
}

double GetRightTLMoeb(int nx, int ny)
{
    return -lattice[(nx+1)%n][ nx==n-1 ? (2*n-2-ny)%n : ny ].topLink;
}

double GetTopRLTorus(int nx, int ny)
{
    return lattice[nx][(ny+1)%n].rightLink;
}

double GetTopRLMoeb(int nx, int ny)
{
    return lattice[nx][(ny+1)%n].rightLink;
}

double GetTopLeftRLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][(ny+1)%n].rightLink;
}

double GetTopLeftRLMoeb(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ nx==0 ? (2*n-2-ny)%n : (ny+1)%n ].rightLink;
}

double GetLeftRLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ny].rightLink;
}

double GetLeftRLMoeb(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ nx==0 ? (n-1-ny) : ny ].rightLink;
}

double GetLeftTLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ny].topLink;
}

double GetLeftTLMoeb(int nx, int ny)
{
    return -lattice[(nx+n-1)%n][ nx==0 ? (2*n-2-ny)%n : ny ].topLink;
}

double GetBottomRLTorus(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].rightLink;
}

double GetBottomRLMoeb(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].rightLink;
}

double GetBottomTLTorus(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].topLink;
}

double GetBottomTLMoeb(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].topLink;
}

double GetBottomRightTLTorus(int nx, int ny)
{
    return lattice[(nx+1)%n][(ny+n-1)%n].topLink;
}

double GetBottomRightTLMoeb(int nx, int ny)
{
    return -lattice[(nx+1)%n][ ny==n-1 ? (n-1-ny)%n : (ny+n-1)%n].topLink;
}

