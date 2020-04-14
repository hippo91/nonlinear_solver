#include "launch_vnr_resolution.h"
#include "test_utils.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * @brief Launch the test of the nonlinear solver
 * 
 * @return int : success (0) or failure (1)
 */
int main()
{
    double *solution;
    size_t pb_size = 10;
    double *old_density, *old_specific_volume;
    double *new_density, *new_specific_volume;
    double *pressure;
    double *internal_energy;
    double *new_pressure;
    double *new_cson;
    allocVecForOMP(pb_size, 8230., &old_density);
    allocVecForOMP(pb_size, 0., &old_specific_volume);
    allocVecForOMP(pb_size, 9500., &new_density);
    allocVecForOMP(pb_size, 0., &new_specific_volume);
    allocVecForOMP(pb_size, 10.e+09, &pressure);
    allocVecForOMP(pb_size, 1.325e+04, &internal_energy);
    allocVecForOMP(pb_size, 0., &solution);
    allocVecForOMP(pb_size, 0., &new_pressure);
    allocVecForOMP(pb_size, 0., &new_cson);
    for (size_t i = 0; i < pb_size; ++i) {
        old_specific_volume[i] = 1. / old_density[i];
        new_specific_volume[i] = 1. / new_density[i];
    }

    launch_vnr_resolution(old_specific_volume, new_specific_volume, pressure, internal_energy, pb_size, solution, new_pressure, new_cson);

    bool success = true;
    if (!check_uniform_array_value(old_density, pb_size, "old_density", 8230.))
        success = false;
    if (!check_uniform_array_value(new_density, pb_size, "new_density", 9500.))
        success = false;
    if (!check_uniform_array_value(pressure, pb_size, "pressure", 10.e+09))
        success = false;
    if (!check_uniform_array_value(internal_energy, pb_size, "internal_energy", 1.325e+04))
        success = false;
    if (!check_uniform_array_value(solution, pb_size, "solution", 200765.8953965593))
        success = false;
    if (!check_uniform_array_value(new_pressure, pb_size, "new_pressure", 13088079183.59054))
        success = false;
    if (!check_uniform_array_value(new_cson, pb_size, "new_cson", 4503.84710590959))
        success = false;

    free(solution);
    free(old_density);
    free(new_density);
    free(pressure);
    free(internal_energy);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
