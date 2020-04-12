#include "launch_vnr_resolution.h"
#include "utils.h"
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
    double *old_density;
    double *new_density;
    double *pressure;
    double *internal_energy;
    double *new_pressure;
    double *new_cson;
    allocVecForOMP(pb_size, 8230., &old_density);
    allocVecForOMP(pb_size, 9500., &new_density);
    allocVecForOMP(pb_size, 10.e+09, &pressure);
    allocVecForOMP(pb_size, 1.325e+04, &internal_energy);
    allocVecForOMP(pb_size, 0., &solution);
    allocVecForOMP(pb_size, 0., &new_pressure);
    allocVecForOMP(pb_size, 0., &new_cson);
    //
    start = clock();
    for (int cycle = 0; cycle < nbr_of_cycles; ++cycle)
    {
        launch_vnr_resolution(old_density, new_density, pressure, internal_energy, pb_size, solution, new_pressure, new_cson);
        if (cycle % 1000 == 0)
        {
            srand48(time(NULL));
            int i = (int)(drand48() * pb_size);
            printf("Cycle %d ==> Energie Interne[%d] = %15.9g | Pression[%d] = %15.9g | Vitesse du son[%d] = %15.9g\n", cycle, i, solution[i], i, new_pressure[i], i, new_cson[i]);
        };
    }
    end = clock();
    double cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    /*
	 * LIBERATION MEMOIRE
	 */
    free(solution);
    free(old_density);
    free(new_density);
    free(pressure);
    free(internal_energy);

    bool success = true;
    // At the time this test has been created, it tooks around 3 minutes to run.
    // Adds a possible variation of 10%
    const double time_limit = 180. * 1.1;
    if (cpu_time_used > time_limit)
    {
        success = false;
        printf("CPU time used (%6.4g seconds) is above the limit (%6.4g seconds)!\n", cpu_time_used, time_limit);
    }

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}
