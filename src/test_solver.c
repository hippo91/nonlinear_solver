#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"
#include "miegruneisen_params.h"
#include "launch_vnr_resolution.h"
#include "test_utils.h"

/**
 * @brief Launch the test of the nonlinear solver
 * 
 * @return int : success (0) or failure (1)
 * @todo: use MACRO to ease the build_array call
 */
int main()
{
    const size_t pb_size = 10;

    BUILD_ARRAY(old_density, pb_size)
    BUILD_ARRAY(old_specific_volume, pb_size)
    BUILD_ARRAY(new_density, pb_size)
    BUILD_ARRAY(new_specific_volume, pb_size)
    BUILD_ARRAY(pressure, pb_size)
    BUILD_ARRAY(internal_energy, pb_size)
    BUILD_ARRAY(solution, pb_size)
    BUILD_ARRAY(new_pressure, pb_size)
    BUILD_ARRAY(new_cson, pb_size)

    fill_array(old_density, 8230.);
    fill_array(new_density, 9500.);
    fill_array(pressure, 10.e+09);
    fill_array(internal_energy, 1.325e+04);

    for (size_t i = 0; i < pb_size; ++i)
    {
        old_specific_volume->data[i] = 1. / old_density->data[i];
        new_specific_volume->data[i] = 1. / new_density->data[i];
    }

    MieGruneisenParams_s const copper_mat = {3940., 1.489, 0., 0., 8930., 2.02, 0.47, 0.};
    launch_vnr_resolution(&copper_mat, old_specific_volume, new_specific_volume, pressure, internal_energy,
                          solution, new_pressure, new_cson);

    bool success = true;
    if (!check_uniform_value(old_density, 8230.))
        success = false;
    if (!check_uniform_value(new_density, 9500.))
        success = false;
    if (!check_uniform_value(pressure, 10.e+09))
        success = false;
    if (!check_uniform_value(internal_energy, 1.325e+04))
        success = false;
    if (!check_uniform_value(solution, 200765.8953965593))
        success = false;
    if (!check_uniform_value(new_pressure, 13088079183.59054))
        success = false;
    if (!check_uniform_value(new_cson, 4503.84710590959))
        success = false;

    DELETE_ARRAY(old_density)
    DELETE_ARRAY(old_specific_volume);
    DELETE_ARRAY(new_density);
    DELETE_ARRAY(new_specific_volume);
    DELETE_ARRAY(pressure);
    DELETE_ARRAY(internal_energy);
    DELETE_ARRAY(solution);
    DELETE_ARRAY(new_pressure);
    DELETE_ARRAY(new_cson);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
