#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "array.h"
#include "launch_vnr_resolution.h"
#include "test_utils.h"
#include "utils.h"

/**
 * @brief Test the performance of the solveur
 * 
 * @return int : 0 (SUCCESS) 1 (FAILURE)
 */
int main()
{
    /*
	 * DECLARATION DES VARIABLES ET ALLOCATION MEMOIRE
	 */
    clock_t start, end;
    const size_t pb_size = 10000;
    const int nbr_of_cycles = 50000;

    s_array solution, old_density, old_specific_volume;
    s_array new_density, new_specific_volume, pressure;
    s_array internal_energy, new_pressure, new_cson;

    build_array(pb_size, "old_density", &old_density);
    build_array(pb_size, "old_specific_volume", &old_specific_volume);
    build_array(pb_size, "new_density", &new_density);
    build_array(pb_size, "new_specific_volume", &new_specific_volume);
    build_array(pb_size, "pressure", &pressure);
    build_array(pb_size, "internal_energy", &internal_energy);
    build_array(pb_size, "solution", &solution);
    build_array(pb_size, "new_pressure", &new_pressure);
    build_array(pb_size, "new_cson", &new_cson);

    fill_array(&old_density, 8230.);
    fill_array(&new_density, 9500.);
    fill_array(&pressure, 10.e+09);
    fill_array(&internal_energy, 1.325e+04);

    for (size_t i = 0; i < pb_size; ++i) {
        old_specific_volume.data[i] = 1. / old_density.data[i];
        new_specific_volume.data[i] = 1. / new_density.data[i];
    }
    //
    start = clock();
    for (int cycle = 0; cycle < nbr_of_cycles; ++cycle)
    {
        launch_vnr_resolution(&old_specific_volume, &new_specific_volume, &pressure, &internal_energy, &solution, &new_pressure, &new_cson);
        if (cycle % 1000 == 0)
        {
            srand48(time(NULL));
            int i = (int)(drand48() * pb_size);
            printf("Cycle %d ==> Energie Interne[%d] = %15.9g | Pression[%d] = %15.9g | Vitesse du son[%d] = %15.9g\n", cycle, i, solution.data[i], i, new_pressure.data[i], i, new_cson.data[i]);
        };
    }
    end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    bool success = true;
    if (!check_uniform_array_value(old_density.data, old_density.size, old_density.label, 8230.))
        success = false;
    if (!check_uniform_array_value(new_density.data, new_density.size, new_density.label, 9500.))
        success = false;
    if (!check_uniform_array_value(pressure.data, pressure.size, pressure.label, 10.e+09))
        success = false;
    if (!check_uniform_array_value(internal_energy.data, internal_energy.size, internal_energy.label, 1.325e+04))
        success = false;
    if (!check_uniform_array_value(solution.data, solution.size, solution.label, 200765.8953965593))
        success = false;
    if (!check_uniform_array_value(new_pressure.data, new_pressure.size, new_pressure.label, 13088079183.59054))
        success = false;
    if (!check_uniform_array_value(new_cson.data, new_cson.size, new_cson.label, 4503.84710590959))
        success = false;
    // At the time this test has been created, it tooks around 3 minutes to run.
    // Adds a possible variation of 10%
    const double time_limit = 95. * 1.1;
    if (cpu_time_used > time_limit)
    {
        success = false;
        printf("CPU time used (%6.4g seconds) is above the limit (%6.4g seconds)!\n", cpu_time_used, time_limit);
    }

    clear_array(&old_density);
    clear_array(&old_specific_volume);
    clear_array(&new_density);
    clear_array(&new_specific_volume);
    clear_array(&pressure);
    clear_array(&internal_energy);
    clear_array(&solution);
    clear_array(&new_pressure);
    clear_array(&new_cson);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
