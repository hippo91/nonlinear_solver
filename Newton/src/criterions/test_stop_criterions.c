#include "stop_criterions.h"
#include "test_utils.h"

#define PB_SIZE 2

/**
 * @brief Launch the unit tests of the stop criterions 
 * 
 * @return int : success (0) or failure (1)
 */
int main()
{
    double x_k[PB_SIZE] = {10., 20.};
    double delta_x_k[PB_SIZE] = {5., -10.};
    double func[PB_SIZE] = {6.e-08, -0.9e-07};
    double x_k_pun[PB_SIZE] = {0., 0.};
    bool has_converged[PB_SIZE] = {false, false};
    double expected_x_k_un[PB_SIZE] = {15., 10.};
    bool expected_has_converged[PB_SIZE] = {false, true};

    relative_gap(x_k, delta_x_k, func, PB_SIZE, x_k_pun, has_converged);

    bool success = true;
    if (!assert_equal_arrays(x_k_pun, expected_x_k_un, PB_SIZE, "x_k_pun"))
        success = false;
    if (!assert_equal_bool_arrays(has_converged, expected_has_converged, PB_SIZE, "has_converged"))
        success = false;

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}