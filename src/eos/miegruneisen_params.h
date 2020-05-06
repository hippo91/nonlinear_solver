#ifndef MIEGRUNEISEN_PARAMS_H
#define MIEGRUNEISEN_PARAMS_H
/**
 * @file miegruneisen_params.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief Holds parameters related to MieGruneisen equation of state
 * @version 0.1
 * @date 2020-05-06
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */

/**
 * @brief Holds the MieGruneisen equation of state parameters
 * 
 */
typedef struct MieGruneisenParams {
    const double c_zero;  /**< Initial sound speed */
    const double s1;
    const double s2;
    const double s3;
    const double rho_zero;  /**< Initial density */
    const double gamma_zero;  /**< Initial Gruneisen coefficient */
    const double coeff_b;
    const double e_zero;  /**< Initial internal energy */
} MieGruneisenParams_s;

#endif