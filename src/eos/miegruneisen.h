#include <stdlib.h>

typedef struct MieGruneisenParameters MieGruneisenParameters_s;

struct MieGruneisenParameters
{
    const double c_zero;
    const double s1;
    const double s2;
    const double s3;
    const double rho_zero;
    const double gamma_zero;
    const double coeff_b;
    const double e_zero;
    double *phi;
    double *dphi;
    double *einth;
    double *deinth;
    double *gamma_per_vol;
    void (*get_pressure_and_derivative)(MieGruneisenParameters_s *, const int, const double *,
                                        const double *, double *, double *);
    void (*get_pressure_and_sound_speed)(MieGruneisenParameters_s *, const int, const double *,
                                         const double *, double *, double *);
    int (*init)(MieGruneisenParameters_s *, const unsigned int, const double * const);
    void (*finalize)(MieGruneisenParameters_s *);
};

/**
 * @brief Initialize the eos by computing all that depends only on density (specific volume):
 *        - phi : pressure on the hugoniot
 *        - dphi : derivative of this pressure
 *        - einth : internal energy on the hugoniot
 *        - deinth : derivative of this internal energy
 *        - gamma_per_vol : dp/de 
 * 
 * @param params : params of the eos
 * @param nb_cells : size of the pb
 * @param specific_volume : specific volume
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
int init(MieGruneisenParameters_s *params, const unsigned int nb_cells, const double * const specific_volume);


/**
 * @brief Ends up the use of the eos by freeing memory allocated at init
 * 
 * @param params : params of the eos
 * @return int EXIT_SUCCESS (0) : in case of success
 *             EXIT_FAILURE (1) : otherwise
 */
void finalize(MieGruneisenParameters_s *params);

void compute_pressure_and_derivative(MieGruneisenParameters_s *params, const int nb_cells,
                                     const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol);

void compute_pressure_and_sound_speed(MieGruneisenParameters_s *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son);