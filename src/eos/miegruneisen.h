/**
 * @file miegruneisen.h
 * @author Guillaume PEILLEX (guillaume.peillex@gmail.com)
 * @brief Holds everything related ot MieGruneisen equation of state
 * @version 0.1
 * @date 2020-05-04
 * 
 * @copyright Copyright (c) 2020 Guillaume Peillex. Subject to GNU GPL V2.
 * 
 */
#include <stdlib.h>

typedef struct MieGruneisenParameters MieGruneisenParameters_s;

/**
 * @brief Holds the MieGruneisen equation of state parameters
 * 
 */
struct MieGruneisenParameters
{
    const double c_zero;  /**< Initial sound speed */
    const double s1;
    const double s2;
    const double s3;
    const double rho_zero;  /**< Initial density */
    const double gamma_zero;  /**< Initial Gruneisen coefficient */
    const double coeff_b;
    const double e_zero;  /**< Initial internal energy */
    double *phi;  /**< Array of pressures along the Hugoniot */
    double *dphi;  /**< Derivative of the pressures along the Hugoniot */
    double *einth;  /**< Internal energy along the Hugoniot */
    double *deinth;  /**< Derivative of the internal energy along the Hugoniot */
    double *gamma_per_vol;  /**< \f$dp/de\f$ */
    void (*get_pressure_and_derivative)(MieGruneisenParameters_s *, const int, const double *,
                                        const double *, double *, double *);  /**< Function that computes pressure and derivative of the pressure according to internal energy */
    void (*get_pressure_and_sound_speed)(MieGruneisenParameters_s *, const int, const double *,
                                         const double *, double *, double *);  /**< Function that computes pressure and the sound speed */
    int (*init)(MieGruneisenParameters_s *, const unsigned int, const double * const);  /**< Function that computes every parameters of the function that depend only on density */
    void (*finalize)(MieGruneisenParameters_s *);  /**< Function that release the memory allocated during init */
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
int init(MieGruneisenParameters_s *params, const unsigned int nb_cells, const double * const specific_volume);


/**
 * @brief Ends up the use of the eos by freeing memory allocated at init
 * 
 * @param[in] params : params of the eos
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
void finalize(MieGruneisenParameters_s *params);

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
void compute_pressure_and_derivative(MieGruneisenParameters_s *params, const int nb_cells,
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
void compute_pressure_and_sound_speed(MieGruneisenParameters_s *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son);