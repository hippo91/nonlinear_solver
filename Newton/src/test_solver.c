#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"
#include "launch_vnr_resolution.h"
#include "test_utils.h"
#include "utils.h"

/**
 * @brief Launch the test of the nonlinear solver
 * 
 * @return int : success (0) or failure (1)
 * @todo: use MACRO to ease the build_array call
 */
int main()
{
    const size_t pb_size = 10;

    p_array old_density = build_array(pb_size, "old_density");
    p_array old_specific_volume = build_array(pb_size, "old_specific_volume");
    p_array new_density = build_array(pb_size, "new_density");
    p_array new_specific_volume = build_array(pb_size, "new_specific_volume");
    p_array pressure = build_array(pb_size, "pressure");
    p_array internal_energy = build_array(pb_size, "internal_energy");
    p_array solution = build_array(pb_size, "solution");
    p_array new_pressure = build_array(pb_size, "new_pressure");
    p_array new_cson = build_array(pb_size, "new_cson");

    fill_array(old_density, 8230.);
    fill_array(new_density, 9500.);
    fill_array(pressure, 10.e+09);
    fill_array(internal_energy, 1.325e+04);

    for (size_t i = 0; i < pb_size; ++i) {
        old_specific_volume->data[i] = 1. / old_density->data[i];
        new_specific_volume->data[i] = 1. / new_density->data[i];
    }

    launch_vnr_resolution(old_specific_volume, new_specific_volume, pressure, internal_energy,
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

    clear_array(old_density);
    free(old_density);
    clear_array(old_specific_volume);
    free(old_specific_volume);
    clear_array(new_density);
    free(new_density);
    clear_array(new_specific_volume);
    free(new_specific_volume);
    clear_array(pressure);
    free(pressure);
    clear_array(internal_energy);
    free(internal_energy);
    clear_array(solution);
    free(solution);
    clear_array(new_pressure);
    free(new_pressure);
    clear_array(new_cson);
    free(new_cson);


    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
