/*
 * In this file are defined the functions to allocate/free the lattice,
 * and all the lattice operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <gsl/gsl_math.h>

#include "lattice.h"
#include "random.h"

// **** Lattice variables:

static double beta;
static int n;
static char bounds[10];
static int sweeps;
static int getPlaquette;
static char plaquetteHistory[40];
// Global variables to compute the acceptance ratio of the updating steps:
static int total, succ;

static struct LatticeSite
{
    double rightLink;
    double topLink;
} **lattice;

// **** Implementation only visible functions declarations:

// Set the boundary conditions to torus/klein:
static void SetBoundsTorus(void);
static void SetBoundsKlein(void);

// ** Functions and function pointers to implement one of the boundary conditions:

static double GetRightTLTorus(int nx, int ny);
static double GetRightTLKlein(int nx, int ny);
static double (*GetRightTL)(int nx, int ny);

static double GetTopRLTorus(int nx, int ny);
static double GetTopRLKlein(int nx, int ny);
static double (*GetTopRL)(int nx, int ny);

static double GetTopLeftRLTorus(int nx, int ny);
static double GetTopLeftRLKlein(int nx, int ny);
static double (*GetTopLeftRL)(int nx, int ny);

static double GetLeftRLTorus(int nx, int ny);
static double GetLeftRLKlein(int nx, int ny);
static double (*GetLeftRL)(int nx, int ny);

static double GetLeftTLTorus(int nx, int ny);
static double GetLeftTLKlein(int nx, int ny);
static double (*GetLeftTL)(int nx, int ny);

static double GetBottomRLTorus(int nx, int ny);
static double GetBottomRLKlein(int nx, int ny);
static double (*GetBottomRL)(int nx, int ny);

static double GetBottomTLTorus(int nx, int ny);
static double GetBottomTLKlein(int nx, int ny);
static double (*GetBottomTL)(int nx, int ny);

static double GetBottomRightTLTorus(int nx, int ny);
static double GetBottomRightTLKlein(int nx, int ny);
static double (*GetBottomRightTL)(int nx, int ny);

// **

// ** Functions to evaluate observables:

static double GetPlaquetteMean(void);

//static double GetChargeSq(void);

//static double GetChargeEvenOdd(void);

// **

static void SweepLattice(void);

// ** Updating steps:

static void SampleRightLink(int nx, int ny);

static void SampleTopLink(int nx, int ny);

// Function that move an angle to an equivalent value in (-pi,pi]:

static double ShiftAngle(double x)
{
    return -ceil((x-M_PI)/2.0/M_PI) * 2.0*M_PI;
}
static double FitInterval(double x)
{
    return x + ShiftAngle(x);
}

// **

// **** Implementation of included functions:

void NewLattice(char *paramFname)
{
    FILE *paramFile = fopen(paramFname, "r");
    char paramName[40], paramValue[40];
    while ( fscanf(paramFile, "%s %s", paramName, paramValue) == 2 )
    {
        if (!strcmp(paramName,"beta")) beta = atof(paramValue);
        else if (!strcmp(paramName,"n")) n = atoi(paramValue);
        else if (!strcmp(paramName,"bounds")) strcpy(bounds,paramValue);
        else if (!strcmp(paramName,"sweeps")) sweeps = atoi(paramValue);
        else if (!strcmp(paramName,"getPlaquette")) getPlaquette = atoi(paramValue);
        else if (!strcmp(paramName,"plaquetteHistory")) strcpy(plaquetteHistory,paramValue);
    }

    if (!strcmp(bounds,"torus")) SetBoundsTorus();
    else if (!strcmp(bounds,"klein")) SetBoundsKlein();
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
            lattice[nx][ny].rightLink = 2.0*M_PI*(RndUniform()-0.5);
            lattice[nx][ny].topLink = 2.0*M_PI*(RndUniform()-0.5);
        }
    }
}

void DeleteLattice(void)
{
    for (int i=0; i<n; i++)
    {
        free(lattice[i]);
    }
    free(lattice);
}

void GetMeasures(void)
{
    FILE *plaquetteFile = NULL;
    //FILE *chargeSqFile;
    //FILE *chargeEvenOddFile;
    if (getPlaquette) assert(plaquetteFile = fopen(plaquetteHistory, "w"));
    //if (getChargeSq) assert(chargeSqFile = fopen(chargeSqHistory, "w"));
    //if (getChargeEvenOdd) assert(chargeEvenOddFile = fopen(chargeEvenOddHistory, "w"));
    for (int i=0; i<sweeps; i++)
    {
        SweepLattice();
        if (getPlaquette) fprintf(plaquetteFile, "%.16e\n", GetPlaquetteMean());
        //if (getChargeSq) fprintf(chargeSqFile, "%.16e\n", GetChargeSq());
        //if (getChargeEvenOdd) fprintf(chargeEvenOdd, "%.16e\n", GetChargeEvenOdd());
    }
    if (getPlaquette)
    {
        fclose(plaquetteFile);
        printf("\n**** Collected %i plaquette  measures at beta = %.1f with lattice size %i and %s boundary conditions ****\n\n", sweeps, beta, n, bounds);
    }
    /*if (getChargeSq)
    {
        fclose(chargeSqFile);
        printf("\n**** Collected %i Q^2  measures at beta = %.1f with lattice size %i and %s boundary conditions ****\n\n", sweeps, beta, n, bounds);
    }*/
    /*if (getChargeEvenOdd)
    {
        fclose(chargeEvenOddFile);
        printf("\n**** Collected %i Q_eo measures at beta = %.1f with lattice size %i and %s boundary conditions ****\n\n", sweeps, beta, n, bounds);
    }*/

    printf("Acceptance ratio: %f\n", (float)succ/total);
}

// **** Implementation of not included functions:

void SetBoundsTorus(void)
{
    GetRightTL = GetRightTLTorus;
    GetTopRL = GetTopRLTorus;
    GetTopLeftRL = GetTopLeftRLTorus;
    GetLeftRL = GetLeftRLTorus;
    GetLeftTL = GetLeftTLTorus;
    GetBottomRL = GetBottomRLTorus;
    GetBottomTL = GetBottomTLTorus;
    GetBottomRightTL = GetBottomRightTLTorus;
}

void SetBoundsKlein(void)
{
    GetRightTL = GetRightTLKlein;
    GetTopRL = GetTopRLKlein;
    GetTopLeftRL = GetTopLeftRLKlein;
    GetLeftRL = GetLeftRLKlein;
    GetLeftTL = GetLeftTLKlein;
    GetBottomRL = GetBottomRLKlein;
    GetBottomTL = GetBottomTLKlein;
    GetBottomRightTL = GetBottomRightTLKlein;
}

double GetPlaquetteMean(void)
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
    return gsl_pow_2(charge/2.0/M_PI);
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

double GetRightTLKlein(int nx, int ny)
{
    if (nx==n-1) return -lattice[0][(2*n-2-ny)%n].topLink;
    else return lattice[nx+1][ny].topLink;
}

double GetTopRLTorus(int nx, int ny)
{
    return lattice[nx][(ny+1)%n].rightLink;
}

double GetTopRLKlein(int nx, int ny)
{
    return lattice[nx][(ny+1)%n].rightLink;
}

double GetTopLeftRLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][(ny+1)%n].rightLink;
}

double GetTopLeftRLKlein(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ nx==0 ? (2*n-2-ny)%n : (ny+1)%n ].rightLink;
}

double GetLeftRLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ny].rightLink;
}

double GetLeftRLKlein(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ nx==0 ? (n-1-ny) : ny ].rightLink;
}

double GetLeftTLTorus(int nx, int ny)
{
    return lattice[(nx+n-1)%n][ny].topLink;
}

double GetLeftTLKlein(int nx, int ny)
{
    if (nx==0) return -lattice[n-1][(2*n-2-ny)%n].topLink;
    else return lattice[nx-1][ny].topLink;
}

double GetBottomRLTorus(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].rightLink;
}

double GetBottomRLKlein(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].rightLink;
}

double GetBottomTLTorus(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].topLink;
}

double GetBottomTLKlein(int nx, int ny)
{
    return lattice[nx][(ny+n-1)%n].topLink;
}

double GetBottomRightTLTorus(int nx, int ny)
{
    return lattice[(nx+1)%n][(ny+n-1)%n].topLink;
}

double GetBottomRightTLKlein(int nx, int ny)
{
    if (nx==n-1) return -lattice[0][(2*n-1-ny)%n].topLink;
    else return lattice[nx+1][(ny+n-1)%n].topLink;
}

