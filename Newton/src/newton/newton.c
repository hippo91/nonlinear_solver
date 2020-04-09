#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils.h"
#ifndef NEWTON
#include "newton.h"
#define NEWTON
#endif
#define DEBUG
#define PRINTSOL

/*
 * allConverged :
 * 	retourne true si tous les membres du tableau has_converged sont à true
 */
static bool allConverged(bool *has_converged, size_t pb_size) {
	for (int i=0 ; i<pb_size ; ++i)
	{
		if (! has_converged[i]) {
			return (false);
		}
	}
	return (true);
}


/*
 * solveNewton :
 * 	resolution de fonction non linéaires
 */
void solveNewton(NewtonParameters_t *Newton, void *func_variables, double *x_ini, size_t pb_size, double **x_sol) {
	// Contrôle des itérations
	int iter = 0;
	int NB_ITER_MAX = 40;
	// Tableau des inconnues à l'itération k
	double *x_k;
	// Tableau des inconnues à l'itération k+1
	double *x_k_pun;
	// Tableau des valeurs de la fonction à annuler
	double *F_k;
	// Tableau des valeurs de la dérivée de la fonction à annuler
	double *dF_k;
	// Tableau des valeurs d'incrémentation
	double *delta_x_k;
	// Tableau des indicateurs de convergence
	bool *has_converged;
	/*
	 * Déclaration/Allocation/Initialisation des tableaux
	 */
  allocVecForOMP(pb_size, 0., &x_k);
#pragma omp parallel for
	for (int i=0 ; i<pb_size ; ++i)
	{
		x_k[i] = x_ini[i];
	}
	allocVecForOMP(pb_size, 0., &F_k);
	allocVecForOMP(pb_size, 0., &dF_k);
	allocVecForOMP(pb_size, 0., &delta_x_k);
	allocVecForOMP(pb_size, 0., &x_k_pun);
	allocVecBoolForOMP(pb_size, &has_converged);
	/*
	 * DEBUT DU NEWTON
	 */
	while ((! allConverged(has_converged, pb_size)) && (iter++ < NB_ITER_MAX)) {
#ifdef DEBUG
		printf("<--- ITERATION : %d --->\n", iter);
#endif
		/*
		 * Evaluation DE F et dF
		 */
		Newton->evaluate_the_function(func_variables, x_k, pb_size, F_k, dF_k);
		/*
		 * Calcul des incréments
		 */
		Newton->increment_the_vector(x_k, F_k, dF_k, pb_size, delta_x_k);
		/*
		 * Vérification de la convergence + application des incréments
		 */
		Newton->check_convergence(x_k, delta_x_k, F_k, pb_size, x_k_pun, has_converged);
		//
#ifdef DEBUG
		for (int i=0 ; i<pb_size ; ++i)
		{
			printf("F_k[%d]     = %15.9g | dF_k[%d]          = %15.9g\n", i, F_k[i], i, dF_k[i]);
			printf("x_k[%d]     = %15.9g | delta_x_k[%d]     = %15.9g\n", i, x_k[i], i, delta_x_k[i]);
			printf("x_k_pun[%d] = %15.9g | has_converged[%d] = %s\n", i, x_k_pun[i], i, has_converged[i] ? "True" : "False");
		}
#endif
		//
		/*
		 * Mise à jour du tableau des inconnues
		 */
#pragma omp parallel for
		for (int i=0 ; i<pb_size ; ++i)
		{
			x_k[i] = x_k_pun[i];
		}
	}
	/*
	 * FIN DU NEWTON
	 */
	if (iter < NB_ITER_MAX) {
		for (int i=0 ; i<pb_size ; ++i) {
            (*x_sol)[i] = x_k_pun[i];
		}
#ifdef PRINTSOL
		printf("Convergence obtenue après %d itérations!\n", iter);
		for (int i=0 ; i<pb_size ; ++i) {
			printf("x[%d] = %15.9g\n", i, x_k[i]);
		}
	} else {
		printf("Non convergence du Newton!\n");
        exit(1);
#endif
	}
	/*
	 * Libération de la mémoire
	 */
	free(x_k);
	free(x_k_pun);
	free(F_k);
	free(dF_k);
	free(delta_x_k);
	free(has_converged);
}

