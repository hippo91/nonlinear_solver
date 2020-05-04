#include "vnr_internalenergy_evolution.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"

void internal_energy_evolution_VNR(void *variables, const p_array newton_var, p_array func, p_array dfunc)
{
    assert(is_valid_array(newton_var));
    assert(is_valid_array(func));
    assert(is_valid_array(dfunc));
    assert(newton_var->size == func->size);
    assert(newton_var->size == dfunc->size);

    VnrParameters_s *vars = (VnrParameters_s *)variables;
    assert(is_valid_array(vars->specific_volume_old));
    assert(is_valid_array(vars->specific_volume_new));
    assert(is_valid_array(vars->internal_energy_old));
    assert(is_valid_array(vars->pressure));
    assert(vars->specific_volume_old->size == vars->specific_volume_new->size);
    assert(vars->specific_volume_old->size == vars->internal_energy_old->size);
    assert(vars->specific_volume_old->size == vars->pressure->size);

    const unsigned int pb_size = newton_var->size;

    BUILD_ARRAY(pression, pb_size)
    BUILD_ARRAY(dpsurde, pb_size)

    p_array built_arrays[] = {pression, dpsurde};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE)
    {
        cleanup_memory(built_arrays, nb_arrays);
        exit(1);
    }

    // Call of EOS
    vars->miegruneisen->get_pressure_and_derivative(vars->miegruneisen, pb_size, vars->specific_volume_new->data,
                                                    newton_var->data, pression->data, dpsurde->data);
    for (size_t i = 0; i < pb_size; ++i)
    {
        const double delta_v = vars->specific_volume_new->data[i] - vars->specific_volume_old->data[i];
        // Function to vanish
        func->data[i] = newton_var->data[i] + (pression->data[i] + vars->pressure->data[i]) * delta_v * 0.5 - vars->internal_energy_old->data[i];
        // Derivative of the function to vanish
        dfunc->data[i] = 1. + dpsurde->data[i] * delta_v * 0.5;
    }

    cleanup_memory(built_arrays, nb_arrays);
}
