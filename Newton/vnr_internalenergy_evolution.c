#include "vnr_internalenergy_evolution.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void internal_energy_evolution_VNR(void *variables, double *newton_var, size_t size_of_pb, double *func, double *dfunc)
{
    VnrVariables_t *vars = (VnrVariables_t*) variables;
    for (int i=0; i<size_of_pb; ++i) {
        double pression_i = 0.;
        double dpsurde_i = 0.;
        double dummy = 0.;
        double specific_volume = 1. / vars->density_new[i];
        // Appel de l'eos
        vars->miegruneisen->solve(vars->miegruneisen, specific_volume, newton_var[i], &pression_i, &dpsurde_i, &dummy);
        double delta_v = 1. / vars->density_new[i] - 1. / vars->density_old[i];
        // Fonction à annuler
        func[i] = newton_var[i] + pression_i * delta_v / 2. + vars->pressure[i] * delta_v / 2. - vars->internal_energy_old[i];
        // Dérivée de la fonction à annuler
        dfunc[i] = 1. + dpsurde_i * delta_v / 2.;
    }
}
