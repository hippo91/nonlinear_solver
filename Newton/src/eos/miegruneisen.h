#include <stdlib.h>

typedef struct MieGruneisenParameters MieGruneisenParameters_s;

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
    void (*get_pressure_and_derivative)(MieGruneisenParameters_s *, const int, const double *,
                                        const double *, double *, double *);
    void (*get_pressure_and_sound_speed)(MieGruneisenParameters_s *, const int, const double *,
                                         const double *, double *, double *);
};


void compute_pressure_and_derivative(MieGruneisenParameters_s *params, const int nb_cells,
                                     const double *specific_volume,
                                     const double *internal_energy, double *pressure,
                                     double *gamma_per_vol);

void compute_pressure_and_sound_speed(MieGruneisenParameters_s *params, const int nb_cells,
                                      const double *specific_volume,
                                      const double *internal_energy, double *pressure, double *c_son);