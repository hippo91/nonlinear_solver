/**
 * @file launch_vnr_resolution.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef LAUNCH_VNR_RESOLUTION_H
#define LAUNCH_VNR_RESOLUTION_H

#include <stdlib.h>
#include "array.h"
#include "miegruneisen_params.h"

/**
 * @brief Use the Newton-Raphson algorithm to solve the equation governing the evolution of the 
 *        internal energy in the Von Neumann Richtmyer scheme.
 * 
 * The equation to solve is :
 * 
 * \f$ e_i^{n+1} = e_i^{n} - \frac{P^n + P^{n+1}}{2} * (\frac{1}{\rho^{n+1}} - \frac{1}{\rho^n})\f$
 * 
 * where \f$P^{n+1}\f$, \f$e_i^{n+1}\f$ and \f$\rho^{n+1}\f$ are linked through the equation of state \f$h\f$ :
 * 
 * \f$P^{n+1} = h(\rho^{n+1}, e_i^{n+1})\f$
 * 
 * @param[in] eos_params : parameters of the equation of state
 * @param[in] old_density : current density \f$\rho^n\f$
 * @param[in] new_density : next time step density \f$\rho^{n+1}\f$
 * @param[in] pressure : current pressure \f$P^n\f$
 * @param[in] internal_energy : current internal energy \f$e_i^n\f$
 * @param[out] solution : solution of the equation i.e the internal energy at next time step \f$e_i^{n+1}\f$
 * @param[out] new_p : pressure at next time step \f$P^{n+1}\f$
 * @param[out] new_vson : sound speed at next time step \f$C_s^{n+1}\f$
 */
void launch_vnr_resolution(MieGruneisenParams_s const *eos_params, p_array old_density, p_array new_density, p_array pressure, p_array internal_energy,
                           p_array solution, p_array new_p, p_array new_vson);

#endif