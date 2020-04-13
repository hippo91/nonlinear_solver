#include <omp.h>
#include <stdbool.h>
#include <stdlib.h>

void allocVecForOMP(size_t pb_size, double value, double **vec)
{
    /* -----------------------------------------------------------------------
     * Retourne un tableau de double de taille pb_size initialise a value
     * utilisation de malloc + initialisation a la main pour eviter des effets
     * numa en cas d'utilisation de openmp
     * */
    *vec = (double *)calloc(pb_size, sizeof(double));
    if (value != 0.) {
        for (size_t i = 0; i < pb_size; ++i)
        {
            (*vec)[i] = value;
        }
    }
}

void allocVecBoolForOMP(size_t pb_size, bool **vec)
{
    /* -----------------------------------------------------------------------
     * Retourne un tableau de booleen de taille pb_size initialise a false
     * utilisation de malloc + initialisation a la main pour eviter des effets
     * numa en cas d'utilisation de openmp
     * */
    *vec = (bool *)malloc(pb_size * sizeof(bool));
    for (size_t i = 0; i < pb_size; ++i)
    {
        (*vec)[i] = false;
    }
}
