#include <stdbool.h>

#include "miegruneisen.h"
#include "miegruneisen_params.h"
#include "test_utils.h"

#define PB_SIZE 2

/**
 * @brief Launch the test of the MieGruneisen equation of state
 *
 * @return int : success (0) or failure (1)
 * @todo : adds test for non zero values of S2, S3 and ezero
 */
int main()
{
    MieGruneisenParams_s copper_mat = {3940., 1.489, 0., 0., 8930., 2.02, 0.47, 0.};
    MieGruneisenEOS_s copper_eos = {
        &copper_mat, NULL, NULL, NULL, NULL, NULL,
        compute_pressure_and_derivative, compute_pressure_and_sound_speed,
        init, finalize};

    double density[PB_SIZE] = {8700., 9200.};
    double specific_volume[PB_SIZE] = {1. / density[0], 1. / density[1]};
    double internal_energy[PB_SIZE] = {1.e+4, 1.e+6};
    double pressure[PB_SIZE] = {0., 0.};
    double gamma_per_vol[PB_SIZE] = {0., 0.};
    double cson[PB_SIZE] = {0., 0.};

    bool success = true;
    double expected_pressure[] = {-3.391122999999982e+09, 2.248138143555919e+10};
    double expected_gamma[] = {17930.499999999996, 18165.5};
    double expected_cson[] = {3837.312029974254, 4663.450646599814};

    copper_eos.init(&copper_eos, PB_SIZE, specific_volume);

    compute_pressure_and_derivative(&copper_eos, PB_SIZE, specific_volume, internal_energy, pressure, gamma_per_vol);

    if (!assert_equal_arrays(pressure, expected_pressure, PB_SIZE, "pressure"))
        success = false;
    if (!assert_equal_arrays(gamma_per_vol, expected_gamma, PB_SIZE,
                             "gamma_per_vol"))
        success = false;

    compute_pressure_and_sound_speed(&copper_eos, PB_SIZE, specific_volume, internal_energy, pressure, cson);

    if (!assert_equal_arrays(pressure, expected_pressure, PB_SIZE, "pressure"))
        success = false;
    if (!assert_equal_arrays(gamma_per_vol, expected_gamma, PB_SIZE,
                             "gamma_per_vol"))
        success = false;
    if (!assert_equal_arrays(cson, expected_cson, PB_SIZE, "cson"))
        success = false;

    copper_eos.finalize(&copper_eos);

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}