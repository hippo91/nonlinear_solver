/**
 * @file vnr_internalenergy_evolution.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief Holds the evaluation of the function govering internal energy evolution in VNR scheme 
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef VNR_INTERNALENERGY_EVOLUTION_H
#define VNR_INTERNALENERGY_EVOLUTION_H

#include <math.h>
#include <stdlib.h>
#include "array.h"
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif

/**
 * @brief This structures holds parameters of the equation 
 *        governing the evolution of internal energy in the VNR scheme
 * 
 */
typedef struct VnrParameters
{
    const p_array specific_volume_old; /**< Previous specific volume */
    const p_array specific_volume_new;  /**< Current specific volume */
    const p_array internal_energy_old;  /**< Previous internal energy */
    const p_array pressure;  /**< Previous pressure */
    MieGruneisenEOS_s *miegruneisen;  /**< Parameters of the underlying eos (here MieGruneisen) */
} VnrParameters_s;

/**
 * @brief Evaluate the fonction governing the evolution of internal energy in the VNR scheme
 * 
 * The equation is : 
 
 * \f$ e_i^{n+1} + \frac{P^n + P^{n+1}}{2} * (\frac{1}{\rho^{n+1}} - \frac{1}{\rho^n}) - e_i^n = 0\f$
 * 
 * @param[in] parameters : parameters of the function
 * @param[in] newton_var : unknown of the function (here it is internal energy)
 * @param[out] func : values of the function
 * @param[out] dfunc : values of the derivative of the function with respect to internal energy
 */
void internal_energy_evolution_VNR(void *parameters, const p_array newton_var,
                                   p_array func, p_array dfunc);

#endif