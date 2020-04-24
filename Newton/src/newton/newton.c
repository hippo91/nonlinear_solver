#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "array.h"
#include "utils.h"
#ifndef NEWTON
#include "newton.h"
#define NEWTON
#endif
// #define DEBUG
// #define PRINTSOL

/*
 * allConverged :
 * 	retourne true si tous les membres du tableau has_converged sont à true
 */
static bool allConverged(bool *has_converged, size_t pb_size)
{
    for (size_t i = 0; i < pb_size; ++i)
    {
        if (!has_converged[i])
        {
            return (false);
        }
    }
    return (true);
}

/*
 * solveNewton :
 * 	resolution de fonction non linéaires
 * 
 * todo : put a return code in order to catch problem and delete correctly the arrays in the caller
 */
void solveNewton(NewtonParameters_s *Newton, void *func_variables, p_array x_ini, p_array x_sol)
{
    if (x_ini->size != x_sol->size) {
        fprintf(stderr, "Size mismatch between array x_ini (%s with size %u) and x_sol (%s with size %u)\n",
                x_ini->label, x_ini->size, x_sol->label, x_sol->size);
        DELETE_ARRAY(x_ini)
        DELETE_ARRAY(x_sol)
        exit(1);
    }
    // Contrôle des itérations
    int iter = 0;
    int NB_ITER_MAX = 40;
    unsigned int pb_size = x_ini->size;
    // Tableau des inconnues à l'itération k
    BUILD_ARRAY(x_k, pb_size)
    // Tableau des inconnues à l'itération k+1
    BUILD_ARRAY(x_k_pun, pb_size)
    // Tableau des valeurs de la fonction à annuler
    BUILD_ARRAY(F_k, pb_size)
    // Tableau des valeurs de la dérivée de la fonction à annuler
    BUILD_ARRAY(dF_k, pb_size)
    // Tableau des valeurs d'incrémentation
    BUILD_ARRAY(delta_x_k, pb_size)
    if (!is_valid_array(delta_x_k)) {
        fprintf(stderr, "Error when building the array delta_x_k!\n");
        exit(1);
    }
    // Tableau des indicateurs de convergence
    bool *has_converged;
    /*
	 * Déclaration/Allocation/Initialisation des tableaux
	 */
    copy_array(x_ini, x_k);
    allocVecBoolForOMP(pb_size, &has_converged);
    /*
	 * DEBUT DU NEWTON
	 */
    while ((!allConverged(has_converged, pb_size)) && (iter++ < NB_ITER_MAX))
    {
#ifdef DEBUG
        printf("<--- ITERATION : %d --->\n", iter);
#endif
        /*
		 * Evaluation DE F et dF
		 */
        Newton->evaluate_the_function(func_variables, x_k->data, pb_size, F_k->data, dF_k->data);
        /*
		 * Calcul des incréments
		 */
        Newton->increment_the_vector(x_k->data, F_k->data, dF_k->data, pb_size, delta_x_k->data);
        /*
		 * Vérification de la convergence + application des incréments
		 */
        Newton->check_convergence(x_k->data, delta_x_k->data, F_k->data, pb_size, x_k_pun->data, has_converged);
        //
#ifdef DEBUG
        for (size_t i = 0; i < pb_size; ++i)
        {
            printf("F_k[%zu]     = %15.9g | dF_k[%zu]          = %15.9g\n", i, F_k[i], i, dF_k[i]);
            printf("x_k[%zu]     = %15.9g | delta_x_k[%zu]     = %15.9g\n", i, x_k[i], i, delta_x_k[i]);
            printf("x_k_pun[%zu] = %15.9g | has_converged[%zu] = %s\n", i, x_k_pun[i], i, has_converged[i] ? "True" : "False");
        }
#endif
        //
        /*
		 * Mise à jour du tableau des inconnues
		 */
        for (size_t i = 0; i < pb_size; ++i)
        {
            x_k->data[i] = x_k_pun->data[i];
        }
    }
    /*
	 * FIN DU NEWTON
	 */
    if (iter < NB_ITER_MAX)
    {
        copy_array(x_k_pun, x_sol);
#ifdef PRINTSOL
        printf("Convergence obtenue après %d itérations!\n", iter);
        print_array(x_k);
#endif
    }
    else
    {
        printf("Non convergence du Newton!\n");
        exit(1);
    }
    /*
	 * Libération de la mémoire
	 */
    DELETE_ARRAY(x_k)
    DELETE_ARRAY(x_k_pun)
    DELETE_ARRAY(F_k)
    DELETE_ARRAY(dF_k)
    DELETE_ARRAY(delta_x_k)
    free(has_converged);
}
