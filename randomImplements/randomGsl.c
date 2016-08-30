#include <gsl/gsl_rng.h>

#include "random.h"

gsl_rng *r;

void RndInit()
{
    gsl_rng_env_setup();

    r = gsl_rng_alloc(gsl_rng_default);

    printf ("generator type: %s\n", gsl_rng_name (r));
    printf ("seed = %lu\n", gsl_rng_default_seed);
}

void RndFinalize()
{
    gsl_rng_free(r);
}

double RndUniform()
{
    return gsl_rng_uniform(r);
}
