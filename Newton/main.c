#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "launch_vnr_resolution.h"

int main() {
	/*
	 * DECLARATION DES VARIABLES ET ALLOCATION MEMOIRE
	 */
	double* solution;
	size_t pb_size = 50;
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
    launch_vnr_resolution(old_density, new_density, pressure, internal_energy, pb_size, solution, new_pressure, new_cson);
    for (int i=0; i<pb_size; ++i) {
        printf("Energie Interne[%d] = %15.9g | Pression[%d] = %15.9g | Vitesse du son[%d] = %15.9g\n", i, solution[i], i, new_pressure[i], i, new_cson[i]);
    }
	/*
	 * LIBERATION MEMOIRE
	 */
	free(solution);
	free(old_density);
	free(new_density);
	free(pressure);
	free(internal_energy);
	return (EXIT_SUCCESS);
}
