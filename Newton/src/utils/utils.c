#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

void allocVecForOMP(size_t pb_size, double value, double **vec) {
    /* -----------------------------------------------------------------------
     * Retourne un tableau de double de taille pb_size initialise a value
     * utilisation de malloc + initialisation a la main pour eviter des effets
     * numa en cas d'utilisation de openmp
     * */
    *vec = (double*)malloc(pb_size * sizeof(double));
#pragma omp parallel for
    for (int i=0; i < pb_size; ++i) {
        (*vec)[i] = value;
    }
}


void allocVecBoolForOMP(size_t pb_size, bool **vec) {
    /* -----------------------------------------------------------------------
     * Retourne un tableau de booleen de taille pb_size initialise a false
     * utilisation de malloc + initialisation a la main pour eviter des effets
     * numa en cas d'utilisation de openmp
     * */
    *vec = (bool*)malloc(pb_size * sizeof(bool));
#pragma omp parallel for
    for (int i=0; i < pb_size; ++i) {
        (*vec)[i] = false;
    }
}
