/**
 * @file cubic.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief A simple cubic function example 
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef CUBIC_H
#define CUBIC_H

#include "array.h"

/**
 * @brief Evaluate the value of \f$x^3 - 2x^2 + 1\f$ and its derivative
 *        \f$3x^2 - 4x\f$
 * @param[in] x : array of unknowns
 * @param[out] fx : array of values of f
 * @param[out] dfx : array of values of df/dx
 */
void cubic_function(void *params, const p_array x, p_array fx, p_array dfx);

#endif