#include <stdlib.h>

typedef struct MieGruneisenParameters MieGruneisenParameters_t;

struct MieGruneisenParameters
{
    double c_zero;
    double s1;
    double s2;
    double s3;
    double rho_zero;
    double gamma_zero;
    double coeff_b;
    double e_zero;
    void (*get_pressure_and_derivative)(MieGruneisenParameters_t *, const int, const double *,
                                        const double *, double *, double *);
    void (*get_pressure_and_sound_speed)(MieGruneisenParameters_t *, const int, const double *,
                                         const double *, double *, double *);
};

void solveVolumeEnergyVec(MieGruneisenParameters_t *, const int nb_cells,
                          const double *specific_volume,
                          const double *internal_energy, double *pressure,
                          double *gamma_per_vol, double *c_son);

void compute_pressure_and_derivative(MieGruneisenParameters_t *params, const int nb_cells,
                                     const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol);

void compute_pressure_and_sound_speed(MieGruneisenParameters_t *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son);