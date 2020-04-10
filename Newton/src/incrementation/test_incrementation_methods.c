#include <stdbool.h>

#include "incrementations_methods.h"
#include "test_utils.h"

#define PB_SIZE 2


/**
 * @brief Test the classical incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_classical_incrementation() {
    double x_k[PB_SIZE] = {-1., 2.};
    double f[PB_SIZE] = {123.456, -987.654};
    double df[PB_SIZE] = {-30., -50.};
    double obtained[PB_SIZE] = {0., 0.};
    double expected[PB_SIZE] = {4.1152, -19.75308};

    classical_incrementation(x_k, f, df, PB_SIZE, obtained);
    return assert_equal_arrays(obtained, expected, PB_SIZE, "obtained");
}


/**
 * @brief Test the damped incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_damped_incrementation() {
    double x_k[PB_SIZE] = {-1., 2.};
    double f[PB_SIZE] = {123.456, -987.654};
    double df[PB_SIZE] = {-30., -50.};
    double obtained[PB_SIZE] = {0., 0.};
    double expected[PB_SIZE] = {4.1152 * 0.5, -19.75308 * 0.5};

    damped_incrementation(x_k, f, df, PB_SIZE, obtained);
    return assert_equal_arrays(obtained, expected, PB_SIZE, "obtained");
}


/**
 * @brief Test the ensure positivity incrementation method
 * 
 * @return true : success
 * @return false : failure
 */
bool test_ensure_positivity_incrementation() {
    double x_k[2 * PB_SIZE] = {-1., 2., -1, 2.};
    double f[2 * PB_SIZE] = {4., 2., 4., 2.};
    double df[2 * PB_SIZE] = {-2., 0.5, -16., 2.};
    double obtained[2 * PB_SIZE] = {0., 0., 0., 0.};
    double expected[2 * PB_SIZE] = {0.5, -1., 0.25, -1.};

    ensure_same_sign_incrementation(x_k, f, df, 2 * PB_SIZE, obtained);
    return assert_equal_arrays(obtained, expected, 2 * PB_SIZE, "obtained");
}

/**
 * @brief Print the usage of the program
 * 
 * @param prog : name of the program 
 */
void usage(const char* prog) {
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
int main(int argc, char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "ERROR while parsing arguments!\n");
        usage(argv[0]);
        fprintf(stderr, "Only one argument is expected : the number of the test!\n");
        return -1;
    }
    int test_number = atoi(argv[1]);
    bool success = true;

    switch (test_number) {
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

	if (!success) return (EXIT_FAILURE);
	return EXIT_SUCCESS;
}