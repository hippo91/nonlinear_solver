#include "launch_vnr_resolution.h"
#include "utils.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

    double *solution;
    size_t pb_size = 10000;
    int nbr_of_cycles = 50000;
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
    //
    start = clock();
    for (int cycle = 0; cycle < nbr_of_cycles; ++cycle)
    {
        launch_vnr_resolution(old_specific_volume, new_specific_volume, pressure, internal_energy, pb_size, solution, new_pressure, new_cson);
        if (cycle % 1000 == 0)
        {
            srand48(time(NULL));
            int i = (int)(drand48() * pb_size);
            printf("Cycle %d ==> Energie Interne[%d] = %15.9g | Pression[%d] = %15.9g | Vitesse du son[%d] = %15.9g\n", cycle, i, solution[i], i, new_pressure[i], i, new_cson[i]);
        };
    }
    end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

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
    // At the time this test has been created, it tooks around 3 minutes to run.
    // Adds a possible variation of 10%
    const double time_limit = 95. * 1.1;
    if (cpu_time_used > time_limit)
    {
        success = false;
        printf("CPU time used (%6.4g seconds) is above the limit (%6.4g seconds)!\n", cpu_time_used, time_limit);
    }

    /*
	 * LIBERATION MEMOIRE
	 */
    free(solution);
    free(old_density);
    free(new_density);
    free(pressure);
    free(internal_energy);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
