#ifndef MIEGRUNEISEN_H
#define MIEGRUNEISEN_H
/**
 * @file miegruneisen.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief Holds everything related to MieGruneisen equation of state
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#include <stdlib.h>
#include "miegruneisen_params.h"

typedef struct MieGruneisenEOS MieGruneisenEOS_s;

/**
 * @brief Defines a MieGruneisen equation of state
 * 
 */
struct MieGruneisenEOS
{
    MieGruneisenParams_s const *params;  /**< The equation of state parameters */
    double *phi;  /**< Array of pressures along the Hugoniot */
    double *dphi;  /**< Derivative of the pressures along the Hugoniot */
    double *einth;  /**< Internal energy along the Hugoniot */
    double *deinth;  /**< Derivative of the internal energy along the Hugoniot */
    double *gamma_per_vol;  /**< \f$dp/de\f$ */
    void (*get_pressure_and_derivative)(MieGruneisenEOS_s *, const int, const double *,
                                        const double *, double *, double *);  /**< Function that computes pressure and derivative of the pressure according to internal energy */
    void (*get_pressure_and_sound_speed)(MieGruneisenEOS_s *, const int, const double *,
                                         const double *, double *, double *);  /**< Function that computes pressure and the sound speed */
    int (*init)(MieGruneisenEOS_s *, const unsigned int, const double * const);  /**< Function that computes every parameters of the function that depend only on density */
    void (*finalize)(MieGruneisenEOS_s *);  /**< Function that release the memory allocated during init */
};

/**
 * @brief Initialize the eos by computing all that depends only on density (specific volume):
 *        - phi : pressure on the hugoniot
 *        - dphi : derivative of this pressure
 *        - einth : internal energy on the hugoniot
 *        - deinth : derivative of this internal energy
 *        - gamma_per_vol : dp/de 
 * 
 * @param[in] params : params of the eos
 * @param[in] nb_cells : size of the pb
 * @param[in] specific_volume : specific volume
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
int init(MieGruneisenEOS_s *eos, const unsigned int nb_cells, const double * const specific_volume);


/**
 * @brief Ends up the use of the eos by freeing memory allocated at init
 * 
 * @param[in] params : params of the eos
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
void finalize(MieGruneisenEOS_s *eos);

/**
 * @brief Compute the pressure and the derivative of the pressure with respect to the specific
 *        internal energy
 * 
 * @param[in] params : parameters of the equation of state
 * @param[in] nb_cells : size of the arrays
 * @param[in] specific_volume : specific volume array
 * @param[in] internal_energy : internal energy array
 * @param[out] pressure : pressure array
 * @param[out] gamma_per_vol : dp/de array
 * @note : the specific volume is unused here but remains in order to keep the signature
 *         of the function compatible with more general eos
 */
void compute_pressure_and_derivative(MieGruneisenEOS_s *eos, const int nb_cells,
                                     const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol);

/**
 * @brief Compute the pressure and the sound speed
 * 
 * @param[in] params : parameters of the equation of state
 * @param[in] nb_cells : size of the arrays
 * @param[in] specific_volume : specific volume array
 * @param[in] internal_energy : internal energy array
 * @param[out] pressure : pressure array
 * @param[out] c_son : sound speed array
 */
void compute_pressure_and_sound_speed(MieGruneisenEOS_s *eos, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son);

#endif