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
    //
    start = clock();
    for (int cycle = 0; cycle < nbr_of_cycles; ++cycle)
    {
        launch_vnr_resolution(old_specific_volume, new_specific_volume, pressure, internal_energy,
                              solution, new_pressure, new_cson);
        if (cycle % 1000 == 0)
        {
            srand48(time(NULL));
            int i = (int)(drand48() * pb_size);
            printf("Cycle %d ==> Energie Interne[%d] = %15.9g | Pression[%d] = %15.9g | Vitesse du son[%d] = %15.9g\n",
                   cycle, i, solution->data[i], i, new_pressure->data[i], i, new_cson->data[i]);
        };
    }
    end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

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
    // At the time this test has been created, it tooks around 3 minutes to run.
    // Adds a possible variation of 10%
    const double time_limit = 95. * 1.1;
    if (cpu_time_used > time_limit)
    {
        success = false;
        printf("CPU time used (%6.4g seconds) is above the limit (%6.4g seconds)!\n", cpu_time_used, time_limit);
    }

    DELETE_ARRAY(old_density);
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
