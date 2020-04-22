#include "array.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @todo: split in different tests
 * 
 */

/**
 * @brief Test the array object
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int main()
{
    const unsigned int expected_size = 30;
    const char *expected_str = "An array for testing";
    p_array test_array = build_array(expected_size, expected_str);

    if (test_array->size != expected_size)
    {
        fprintf(stderr, "Wrong value for the array's size : %u instead of %u\n", test_array->size, expected_size);
        return EXIT_FAILURE;
    }
    if (strcmp(test_array->label, expected_str) != 0)
    {
        fprintf(stderr, "Wrong value for the array's label : %s instead of %s\n", test_array->label, expected_str);
        return EXIT_FAILURE;
    }

    test_array->data[0] = 10.;
    test_array->data[9] = -10.;
    test_array->data[10] = 20.;
    test_array->data[12] = -20.;
    test_array->data[25] = 30.;
    test_array->data[29] = -30.;
    print_array(test_array); // No test here. Just for beauty...

    fill_array(test_array, 123.456);
    if (!check_uniform_value(test_array, 123.456))
    {
        return EXIT_FAILURE;
    }

    p_array failed_array = build_array(-10, "Unvalid");
    if (failed_array != NULL)
    {
        fprintf(stderr, "The build_array function should have failed because the array allocation should have failed!\n");
        return EXIT_FAILURE;
    }

    char too_long[129];
    for (int i = 0; i < 128; ++i)
        too_long[i] = 'a';
    too_long[128] = 0;
    failed_array = build_array(10, too_long);
    if (failed_array != NULL)
    {
        fprintf(stderr, "The build_array function should have failed because the label is too long!\n");
        return EXIT_FAILURE;
    }

    clear_array(test_array);
    if (test_array->data != NULL || test_array->size != 0 || strcmp(test_array->label, "") != 0)
    {
        fprintf(stderr, "The data of array %s have not been correctly cleared!\n", test_array->label);
        return EXIT_FAILURE;
    }

    free(test_array);
    clear_array(failed_array);
    free(failed_array);

    return EXIT_SUCCESS;
}