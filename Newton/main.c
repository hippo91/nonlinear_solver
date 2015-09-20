#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "newton.h"
#include "utils.h"
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif
#include "vnr_internalenergy_evolution.h"
#include "quadratic_function.h"
#include "incrementations_methods.h"
#include "stop_criterions.h"

int main() {
	/*
	 * POUR LA MESURE DU TEMPS
	 */
	double begin = 0.;
	double end = 0.;
	double delta = 0.;
	/*
	 * DECLARATION DES VARIABLES ET ALLOCATION MEMOIRE
	 */
    double* x_ini;
    double* solution;
    size_t pb_size = 1000000;
    allocVecForOMP(pb_size, -0.001, &x_ini);
    allocVecForOMP(pb_size, 0., &solution);
    /*
     * DEFINITION DE LA FONCTION A RESOUDRE (POLYNOME D'ORDRE 2)
     * FIXATION DE LA METHODE D'INCREMENTATION ET DU CRITERE D'ARRET
     * */
    NewtonParameters_t TheNewton = {quadratic_function, classical_incrementation, relative_gap};
    /*
     * APPEL DU SOLVEUR
     */
    printf("SOLVEUR DE TYPE : NEWTON!\n");
	begin = omp_get_wtime();
    for (int i=0; i<1000; i++) {
    	solveNewton(&TheNewton, NULL, x_ini, pb_size, solution);
    }
	end = omp_get_wtime();
	delta = end - begin;
    printf("Temps consommé : %lf secondes\n", delta);
    /*
     * LIBERATION MEMOIRE
     */
    free(x_ini);
    free(solution);
    return (EXIT_SUCCESS);
}

//void main() {
//    double* x_ini;
//    double* solution;
//    size_t pb_size = 50;
//    /*
//     * DEFINITION DE L EOS DE TYPE MIE-GRUNEISEN
//     * */
//    double czero = 3980.0, S1 = 1.58, S2 = 0., S3 = 0., rhozero = 8129.0, grunzero = 1.6, b = 0.5, ezero = 0.;
//    MieGruneisenParameters_t MieGruneisenParams= {czero, S1, S2, S3, rhozero, grunzero, b, ezero, solveVolumeEnergy};
//    /*
//     * DEFINITION DE LA FONCTION A RESOUDRE (EQUATION D EVOLUTION DE L ENERGIE
//     * INTERNE DANS LE SCHEMA VNR)
//     * */
//    double *old_density;
//    double *new_density;
//    double *pressure;
//    double *internal_energy;
//    allocVecForOMP(pb_size, 8230., &old_density);
//    allocVecForOMP(pb_size, 9500., &new_density);
//    allocVecForOMP(pb_size, 10.e+09, &pressure);
//    allocVecForOMP(pb_size, 1.325e+04, &internal_energy);
//    VnrVariables_t VnrVars = {old_density, new_density, internal_energy, pressure, &MieGruneisenParams};
//    NewtonParameters_t TheNewton = {InternalEnergyEvolutionVNR, ClassicalIncrementation, RelativeGap};
//    //
//    x_ini = (double*)malloc(pb_size * sizeof(double));
//    allocVecForOMP(pb_size, 0., &solution);
//    for (int i=0 ; i<pb_size ; ++i)
//    {
//            x_ini[i] = 0.25 * i;
//    }
//    printf("SOLVEUR DE TYPE : NEWTON!\n");
//    solveNewton(&TheNewton, &VnrVars, x_ini, pb_size, solution);
//    free(x_ini);
//    free(solution);
//    free(old_density);
//    free(new_density);
//    free(pressure);
//    free(internal_energy);
//}
