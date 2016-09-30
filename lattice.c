/*
 * In this file are defined the functions to allocate/free the lattice,
 * and all the lattice operations.
 */

#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "lattice.h"
#include "random.h"

// Function that move an angle to an equivalent value in (-pi,pi]:

#define ShiftAngle(x) (-ceil((x-M_PI)/2./M_PI) * 2.*M_PI)
#define FitInterval(x) (x + ShiftAngle(x))

// **** Lattice variables:

static double beta;
static int n;
// Global variables to compute the acceptance ratio of the updating steps:
static int total, succ;

static struct LatticeSite
{
    double rightLink;
    double topLink;
} **lattice;

// **** Implementation only visible functions declarations:

// Set the boundary conditions to torus/moebius:
static void SetBoundsTorus();
static void SetBoundsMoeb();

// Set the program to evaluate plaquette/charge:
static void SetObsPlaquette();
static void SetObsCharge();

// ** Functions and function pointers to implement one of the boundary conditions:

static double GetRightTLTorus(int nx, int ny);
static double GetRightTLMoeb(int nx, int ny);
static double (*GetRightTL)(int nx, int ny);

static double GetTopRLTorus(int nx, int ny);
static double GetTopRLMoeb(int nx, int ny);
static double (*GetTopRL)(int nx, int ny);

static double GetTopLeftRLTorus(int nx, int ny);
static double GetTopLeftRLMoeb(int nx, int ny);
static double (*GetTopLeftRL)(int nx, int ny);

static double GetLeftRLTorus(int nx, int ny);
static double GetLeftRLMoeb(int nx, int ny);
static double (*GetLeftRL)(int nx, int ny);

static double GetLeftTLTorus(int nx, int ny);
static double GetLeftTLMoeb(int nx, int ny);
static double (*GetLeftTL)(int nx, int ny);

static double GetBottomRLTorus(int nx, int ny);
static double GetBottomRLMoeb(int nx, int ny);
static double (*GetBottomRL)(int nx, int ny);

static double GetBottomTLTorus(int nx, int ny);
static double GetBottomTLMoeb(int nx, int ny);
static double (*GetBottomTL)(int nx, int ny);

static double GetBottomRightTLTorus(int nx, int ny);
static double GetBottomRightTLMoeb(int nx, int ny);
static double (*GetBottomRightTL)(int nx, int ny);

static char *boundsName;

// **

// ** Functions and functions pointers to evaluate one of observables:

static double GetPlaquetteMean();

static double GetCharge();

static double (*GetObservable)();

static char *obsName;

// **

static void SweepLattice();

// ** Updating steps:

static void SampleRightLink(int nx, int ny);

static void SampleTopLink(int nx, int ny);

// **

// **** Implementation of included functions:

void NewLattice(double _beta, int _n, int therm, const char *_boundsName, const char *_obsName)
{
    beta = _beta;
    n = _n;
    total = 0;
    succ = 0;

    if (!strcmp(_boundsName,"torus")) SetBoundsTorus();
    else if (!strcmp(_boundsName,"moebius")) SetBoundsMoeb();
    else assert(0);

    if (!strcmp(_obsName,"plaquette")) SetObsPlaquette();
    else if (!strcmp(_obsName,"charge")) SetObsCharge();
    else assert(0);

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
    for (int t=0; t<therm; t++)
    {
        SweepLattice();
    }
}

void DeleteLattice()
{
    for (int i=0; i<n; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

void GetMeasures(double *data, int iters)
{
    for (int i=0; i<iters; i++)
    {
        SweepLattice();
        data[i] = GetObservable();
    }

    //printf("\n**** Collected %i %s measures at beta = %.1f with lattice size %i and %s boundary conditions ****\n\n", iters, obsName, beta, n, boundsName);
    printf("Acceptance ratio: %f\n", (float)succ/total);
}

// **** Implementation of not included functions:

void SetBoundsTorus()
{
    GetRightTL = GetRightTLTorus;
    GetTopRL = GetTopRLTorus;
    GetTopLeftRL = GetTopLeftRLTorus;
    GetLeftRL = GetLeftRLTorus;
    GetLeftTL = GetLeftTLTorus;
    GetBottomRL = GetBottomRLTorus;
    GetBottomTL = GetBottomTLTorus;
    GetBottomRightTL = GetBottomRightTLTorus;
    boundsName = "torus";
}

void SetBoundsMoeb()
{
    GetRightTL = GetRightTLMoeb;
    GetTopRL = GetTopRLMoeb;
    GetTopLeftRL = GetTopLeftRLMoeb;
    GetLeftRL = GetLeftRLMoeb;
    GetLeftTL = GetLeftTLMoeb;
    GetBottomRL = GetBottomRLMoeb;
    GetBottomTL = GetBottomTLMoeb;
    GetBottomRightTL = GetBottomRightTLMoeb;
    boundsName = "moebius";
}

void SetObsPlaquette()
{
    GetObservable = GetPlaquetteMean;
    obsName = "plaquette";
}

void SetObsCharge()
{
    GetObservable = GetCharge;
    obsName = "charge";
}

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

double GetCharge()
{
    double charge = 0.;

    for (int nx=0; nx<n; nx++)
    {
        for (int ny=0; ny<n; ny++)
        {
            charge += FitInterval( lattice[nx][ny].rightLink - lattice[nx][ny].topLink \
                                 + GetRightTL(nx,ny)         - GetTopRL(nx,ny) );
        }
    }
    return charge/2./M_PI;
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

void SampleRightLink(int nx, int ny)
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

void SampleTopLink(int nx, int ny)
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

double GetRightTLTorus(int nx, int ny)
{
    return lattice[(nx+1)%n][ny].topLink;
}

double GetRightTLMoeb(int nx, int ny)
{
    if(nx==n-1)
    {
        return -lattice[0][(2*n-2-ny)%n].topLink;
    }
    else
    {
        return lattice[nx+1][ny].topLink;
    }
    //return -lattice[(nx+1)%n][ nx==n-1 ? (2*n-2-ny)%n : ny ].topLink;
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
    if(nx==0)
    {
        return -lattice[n-1][(2*n-2-ny)%n].topLink;
    }
    else
    {
        return lattice[nx-1][ny].topLink;
    }
    //return -lattice[(nx+n-1)%n][ nx==0 ? (2*n-2-ny)%n : ny ].topLink;
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
    if(nx==n-1)
    {
        return -lattice[0][(2*n-1-ny)%n].topLink;
    }
    else
    {
        return lattice[nx+1][(ny+n-1)%n].topLink;
    }
    //return -lattice[(nx+1)%n][ ny==n-1 ? (n-1-ny)%n : (ny+n-1)%n].topLink;
}

