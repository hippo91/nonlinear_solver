#include "stop_criterions.h"
#include "test_utils.h"
#include "array.h"

#define PB_SIZE 2

/**
 * @brief Launch the unit tests of the stop criterions 
 * 
 * @return int : success (0) or failure (1)
 */
int main()
{
    BUILD_ARRAY(x_k, PB_SIZE)
    BUILD_ARRAY(delta_x_k, PB_SIZE)
    BUILD_ARRAY(func, PB_SIZE)
    BUILD_ARRAY(x_k_pun, PB_SIZE)
    BUILD_ARRAY(expected_x_k_un, PB_SIZE)
    p_array built_arrays[] = {x_k, delta_x_k, func, x_k_pun, expected_x_k_un};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);

    for (unsigned int i = 0; i < nb_arrays; ++i) {
        if (built_arrays[i] == NULL) {
            fprintf(stderr, "Error during building of %d th array\n", i);
            for (unsigned int j = 0; j < nb_arrays; ++j)
            {
                DELETE_ARRAY(built_arrays[i]);
            }
            return EXIT_FAILURE;
        }
    }

    x_k->data[0] = 10.;
    x_k->data[1] = 20.;

    delta_x_k->data[0] = 5.;
    delta_x_k->data[1] = -10.;

    func->data[0] = 6.e-08;
    func->data[1] = -0.9e-07;

    expected_x_k_un->data[0] = 15.;
    expected_x_k_un->data[1] = 10.;

    bool has_converged[PB_SIZE] = {false, false};
    bool expected_has_converged[PB_SIZE] = {false, true};

    relative_gap(x_k, delta_x_k, func, x_k_pun, has_converged);

    bool success = true;
    if (!assert_equal(x_k_pun, expected_x_k_un))
        success = false;
    if (!assert_equal_bool_arrays(has_converged, expected_has_converged, PB_SIZE, "has_converged"))
        success = false;

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}