#include "vnr_internalenergy_evolution.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void internal_energy_evolution_VNR(void *variables, double *newton_var, size_t size_of_pb, double *func, double *dfunc)
{
    VnrVariables_t *vars = (VnrVariables_t*) variables;
    double *pression = calloc(size_of_pb, sizeof(double));
    double *dpsurde = calloc(size_of_pb, sizeof(double));
    double *dummy = calloc(size_of_pb, sizeof(double));
    double *specific_volume = calloc(size_of_pb, sizeof(double));
    for (int i=0; i<size_of_pb; ++i) {
        specific_volume[i] = 1. / vars->density_new[i];
    }
    // Appel de l'eos
    vars->miegruneisen->solve(vars->miegruneisen, size_of_pb, specific_volume, newton_var, pression, dpsurde, dummy);
    double delta_v = 0.;
    for (int i=0; i<size_of_pb; ++i) {
        delta_v = 1. / vars->density_new[i] - 1. / vars->density_old[i];
        // Fonction à annuler
        func[i] = newton_var[i] + pression[i] * delta_v / 2. + vars->pressure[i] * delta_v / 2. - vars->internal_energy_old[i];
        // Dérivée de la fonction à annuler
        dfunc[i] = 1. + dpsurde[i] * delta_v / 2.;
    }
    free(pression);
    free(dpsurde);
    free(dummy);
    free(specific_volume);
}
