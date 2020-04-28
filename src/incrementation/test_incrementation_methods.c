#include <stdbool.h>

#include "incrementations_methods.h"
#include "test_utils.h"
#include "array.h"

#define PB_SIZE 2

/**
 * @brief Test the classical incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_classical_incrementation()
{
    BUILD_ARRAY(x_k, PB_SIZE)
    BUILD_ARRAY(f, PB_SIZE)
    BUILD_ARRAY(df, PB_SIZE)
    BUILD_ARRAY(obtained, PB_SIZE)
    BUILD_ARRAY(expected, PB_SIZE)

    p_array built_arrays[] = {x_k, f, df, obtained, expected};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE)
    {
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    x_k->data[0] = -1.;
    x_k->data[1] = 2.;

    f->data[0] = 123.456;
    f->data[1] = -987.654;

    df->data[0] = -30.;
    df->data[1] = -50.;

    expected->data[0] = 4.1152;
    expected->data[1] = -19.75308;

    classical_incrementation(x_k, f, df, obtained);
    int status = assert_equal(obtained, expected);

    cleanup_memory(built_arrays, nb_arrays);
    return status;
}

/**
 * @brief Test the damped incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_damped_incrementation()
{
    BUILD_ARRAY(x_k, PB_SIZE)
    BUILD_ARRAY(f, PB_SIZE)
    BUILD_ARRAY(df, PB_SIZE)
    BUILD_ARRAY(obtained, PB_SIZE)
    BUILD_ARRAY(expected, PB_SIZE)

    p_array built_arrays[] = {x_k, f, df, obtained, expected};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE)
    {
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    x_k->data[0] = -1.;
    x_k->data[1] = 2.;

    f->data[0] = 123.456;
    f->data[1] = -987.654;

    df->data[0] = -30.;
    df->data[1] = -50.;

    expected->data[0] = 4.1152 * 0.5;
    expected->data[1] = -19.75308 * 0.5;

    damped_incrementation(x_k, f, df, obtained);
    int status = assert_equal(obtained, expected);

    cleanup_memory(built_arrays, nb_arrays);

    return status;
}

/**
 * @brief Test the ensure positivity incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_ensure_positivity_incrementation()
{
    BUILD_ARRAY(x_k, 2 * PB_SIZE)
    BUILD_ARRAY(f, 2 * PB_SIZE)
    BUILD_ARRAY(df, 2 * PB_SIZE)
    BUILD_ARRAY(obtained, 2 * PB_SIZE)
    BUILD_ARRAY(expected, 2 * PB_SIZE)

    p_array built_arrays[] = {x_k, f, df, obtained, expected};
    const unsigned int nb_arrays = sizeof(built_arrays) / sizeof(p_array);
    if (check_arrays_building(built_arrays, nb_arrays) == EXIT_FAILURE)
    {
        cleanup_memory(built_arrays, nb_arrays);
        return EXIT_FAILURE;
    }

    x_k->data[0] = -1.;
    x_k->data[1] = 2.;
    x_k->data[2] = -1;
    x_k->data[3] = 2.;

    f->data[0] = 4.;
    f->data[1] = 2.;
    f->data[2] = 4.;
    f->data[3] = 2.;

    df->data[0] = -2.;
    df->data[1] = 0.5;
    df->data[2] = -16.;
    df->data[3] = 2.;

    expected->data[0] = 0.5;
    expected->data[1] = -1.;
    expected->data[2] = 0.25;
    expected->data[3] = -1.;

    ensure_same_sign_incrementation(x_k, f, df, obtained);
    int status = assert_equal(obtained, expected);

    cleanup_memory(built_arrays, nb_arrays);

    return status;
}

/**
 * @brief Print the usage of the program
 * 
 * @param prog : name of the program 
 */
void usage(const char *prog)
{
    fprintf(stderr, "%s number_of_test\n", prog);
    fprintf(stderr, "   number_of_test=0 : test the classical incrementation method\n");
    fprintf(stderr, "   number_of_test=1 : test the damped incrementation method\n");
    fprintf(stderr, "   number_of_test=2 : test the ensure positivity incrementation method\n");
}

/**
 * @brief Launch the test of the incrementation methods
 * 
 * @return int : success (0) or failure (1)
 */
int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        fprintf(stderr, "ERROR while parsing arguments!\n");
        usage(argv[0]);
        fprintf(stderr, "Only one argument is expected : the number of the test!\n");
        return -1;
    }
    int test_number = atoi(argv[1]);
    bool success = true;

    switch (test_number)
    {
    case 0:
        success = test_classical_incrementation();
        break;
    case 1:
        success = test_damped_incrementation();
        break;
    case 2:
        success = test_ensure_positivity_incrementation();
        break;
    default:
        fprintf(stderr, "ERROR while parsing arguments!\n");
        usage(argv[0]);
        fprintf(stderr, "Only 3 methods may be tested: please enter a number in [0-2] not %d!\n", test_number);
        return -2;
    }

    if (!success)
        return (EXIT_FAILURE);
    return EXIT_SUCCESS;
}