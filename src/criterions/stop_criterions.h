/**
 * @file stop_criterions.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief This file holds functions that check convergence of the Newton algorithm
 * @version 1.0
 * @date 2020-04-30
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#ifndef STOP_CRITERIONS_H
#define STOP_CRITERIONS_H

#include <stdbool.h>
#include <stdlib.h>
#include "array.h"

/**
 * @brief Check the convergence of the function
 * 		  The convergence is obtained if the value of the function is sufficiently close to zero.
 * 
 * @param[in] delta_x_k : array of the Newton's incrementation values 
 * @param[in] func : array of the function values
 * @param[out] has_converged : array of boolean indicating the convergence of each item
 * @return true : if convergence of all items is achieved 
 * @return false : otherwise
 */
bool relative_gap(p_array delta_x_k, p_array func, bool *has_converged);

/**
 * @brief The prototype of criterion checking function to be used with the Newton algorithm
 * 
 */
typedef bool (*criterion_fct_ptr)(p_array delta_x_k, p_array func, bool *has_converged);

#endif