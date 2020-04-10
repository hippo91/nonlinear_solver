#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include "vnr_internalenergy_evolution.h"
#include "incrementations_methods.h"
#include "stop_criterions.h"
#include "newton.h"
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif
#include "utils.h"
//#define MEASURE_TIME

void launch_vnr_resolution(double *old_density, double *new_density, double *pressure, double *internal_energy,
                           int pb_size, double *solution, double *new_p, double *new_vson) {
#ifdef MEASURE_TIME
	/*
	 * POUR LA MESURE DU TEMPS
	 */
	double begin = 0.;
	double end = 0.;
	double delta = 0.;
#endif
    /*
     * DEFINITION DE L EOS
     * */
	double czero = 3940.0, S1 = 1.489, S2 = 0., S3 = 0., rhozero = 8930.0, grunzero = 2.02, b = 0.47, ezero = 0.;
	MieGruneisenParameters_t MieGruneisenParams= {czero, S1, S2, S3, rhozero, grunzero, b, ezero, solveVolumeEnergyVec};
	/*
	 * DEFINITION DE LA FONCTION A RESOUDRE (EQUATION D EVOLUTION DE L ENERGIE
	 * INTERNE DANS LE SCHEMA VNR)
     * FIXATION DE LA METHODE D'INCREMENTATION ET DU CRITERE D'ARRET
	 * */
  double * l_old_density = NULL;
  double * l_new_density = NULL;
  double * l_pressure = NULL;
  double * l_internal_energy = NULL;
  allocVecForOMP(pb_size, 0., &l_old_density);
  allocVecForOMP(pb_size, 0., &l_new_density);
  allocVecForOMP(pb_size, 0., &l_pressure);
  allocVecForOMP(pb_size, 0., &l_internal_energy);
  memmove(l_old_density, old_density, pb_size * sizeof(double));
  memmove(l_new_density, new_density, pb_size * sizeof(double));
  memmove(l_pressure, pressure, pb_size * sizeof(double));
  memmove(l_internal_energy, internal_energy, pb_size * sizeof(double));
  VnrVariables_t VnrVars = {l_old_density, l_new_density, l_internal_energy, l_pressure, &MieGruneisenParams};
	NewtonParameters_t TheNewton = {internal_energy_evolution_VNR, classical_incrementation, relative_gap};
#ifdef MEASURE_TIME
	begin = omp_get_wtime();
#endif
	solveNewton(&TheNewton, &VnrVars, internal_energy, pb_size, &solution);
    // Appel de l'eos avec la solution du newton pour calculer la nouvelle
    // pression et vitesse du son
  double *specific_volume = NULL;
  allocVecForOMP(pb_size, 0., &specific_volume);
  for (int i = 0; i < pb_size; ++i) {
    specific_volume[i] = 1. / new_density[i];
  }
  double *dummy = NULL;
  allocVecForOMP(pb_size, 0., &dummy);
  VnrVars.miegruneisen->solve(VnrVars.miegruneisen, pb_size, specific_volume, solution, new_p, dummy, new_vson);
  free(l_old_density);
  free(l_new_density);
  free(l_pressure);
  free(l_internal_energy);
  free(specific_volume);
  free(dummy);
#ifdef MEASURE_TIME
	end = omp_get_wtime();
	delta = end - begin;
	printf("Temps consommé : %lf secondes\n", delta);
#endif
}
