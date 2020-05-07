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
    double c_zero;  /**< Initial sound speed */
    double s1;
    double s2;
    double s3;
    double rho_zero;  /**< Initial density */
    double gamma_zero;  /**< Initial Gruneisen coefficient */
    double coeff_b;
    double e_zero;  /**< Initial internal energy */
} MieGruneisenParams_s;

#endif