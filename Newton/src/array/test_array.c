#include "array.h"
#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct unittest {
    const char* name;
    int (*fun_ptr)();
} s_unittest;

/**
 * @brief Test the build array function in case of success
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_build_array()
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

    DELETE_ARRAY(test_array)
    return EXIT_SUCCESS;
}

/**
 * @brief Test the build_array function in case the label is too long
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_build_array_label_too_long()
{
    char too_long[129];
    for (int i = 0; i < 128; ++i)
        too_long[i] = 'a';
    too_long[128] = 0;
    p_array failed_array = build_array(10, too_long);

    if (failed_array != NULL)
    {
        fprintf(stderr, "The build_array function should have failed because the label is too long!\n");
        DELETE_ARRAY(failed_array)
        return EXIT_FAILURE;
    }

    DELETE_ARRAY(failed_array)
    return EXIT_SUCCESS;
}

/**
 * @brief Test the build_array function in case the size is too high
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_build_array_size_too_high()
{
    // -10 in unsigned int is converted to high number
    BUILD_ARRAY(failed_array, -10)

    if (failed_array != NULL)
    {
        fprintf(stderr, "The build_array function should have failed because the array allocation should have failed!\n");
        DELETE_ARRAY(failed_array)
        return EXIT_FAILURE;
    }

    DELETE_ARRAY(failed_array)
    return EXIT_SUCCESS;
}

/**
 * @brief Test the fill_array function when everything is ok
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_fill_array()
{
    const unsigned int expected_size = 30;
    BUILD_ARRAY(array_to_fill, expected_size)

    fill_array(array_to_fill, 123.456);
    if (!check_uniform_value(array_to_fill, 123.456))
    {
        return EXIT_FAILURE;
    }

    DELETE_ARRAY(array_to_fill)
    return EXIT_SUCCESS;
}


/**
 * @brief Test the failure of fill_array function in case the pointer
 *        to the array struct is NULL
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_fill_array_null_ptr()
{
    p_array array_to_fill = NULL;

    if (fill_array(array_to_fill, 123.456) == EXIT_SUCCESS)
    {
        fprintf(stderr, "The fill_array function should have failed "
                        "because the pointer to the array struct is NULL!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/**
 * @brief Test the fill_array function in case the array size is nill
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_fill_array_null_size()
{
    BUILD_ARRAY(null_size_arr, 0)

    if (fill_array(null_size_arr, 123.456) == EXIT_SUCCESS)
    {
        fprintf(stderr, "The fill_array function should have failed "
                        "because the size of the array is nill!\n");
        DELETE_ARRAY(null_size_arr)
        return EXIT_FAILURE;
    }

    DELETE_ARRAY(null_size_arr)
    return EXIT_SUCCESS;
}

/**
 * @brief Test the fill_array_data function in case the data of the array struct is NULL
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_fill_array_data_null_ptr()
{
    BUILD_ARRAY(null_data_arr, 10)
    free(null_data_arr->data);
    null_data_arr->data = NULL;

    if (fill_array(null_data_arr, 123.456) == EXIT_SUCCESS)
    {
        fprintf(stderr, "The fill_array function should have failed "
                        "because the pointer to the data member is NULL!\n");
        DELETE_ARRAY(null_data_arr)
        return EXIT_FAILURE;
    }

    DELETE_ARRAY(null_data_arr)
    return EXIT_SUCCESS;

}

/**
 * @brief Test the clear_array function
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int test_clear_array()
{
    BUILD_ARRAY(valid_array, 10)

    clear_array(valid_array);
    if (valid_array->data != NULL || valid_array->size != 0 || strcmp(valid_array->label, "") != 0)
    {
        fprintf(stderr, "The data of array %s have not been correctly cleared!\n", valid_array->label);
        return EXIT_FAILURE;
    }

    free(valid_array);
    return EXIT_SUCCESS;
}

/**
 * @brief Print usage of this program
 * 
 * @param test_collection : the collection of unit tests
 * @param size : size of the collection
 */
void usage(const s_unittest * const test_collection, const unsigned int size)
{
    fprintf(stderr, "This program waits for the unit test to run:\n");
    for (unsigned int i = 0; i < size; ++i)
    {
        fprintf(stderr, "\t[%d] %s\n", i, test_collection[i].name);
    }
}

#define TEST_DECLARATION(name) {#name, name}

/**
 * @brief Test the array object
 * 
 * @return int EXIT_SUCCESS (0) : in case of success
               EXIT_FAILURE (1) : otherwise
 */
int main(int argc, char* argv[])
{
    s_unittest test_collection[] = {
        TEST_DECLARATION(test_build_array),
        TEST_DECLARATION(test_build_array_label_too_long),
        TEST_DECLARATION(test_build_array_size_too_high),
        TEST_DECLARATION(test_fill_array),
        TEST_DECLARATION(test_fill_array_null_ptr),
        TEST_DECLARATION(test_fill_array_null_size),
        TEST_DECLARATION(test_fill_array_data_null_ptr),
        TEST_DECLARATION(test_clear_array)
    };
    const int test_number = sizeof(test_collection) / sizeof(s_unittest);

    if (argc != 2) {
        fprintf(stderr, "Wrong number of arguments!\n");
        usage(test_collection, test_number);
        return EXIT_FAILURE;
    }

    int num_test = atoi(argv[1]);

    if (num_test >= test_number || num_test < 0) {
        fprintf(stderr, "Test doesn't exist!\n");
        usage(test_collection, test_number);
        return EXIT_FAILURE;
    }

    printf("Executing test %s\n", test_collection[num_test].name);
    return test_collection[num_test].fun_ptr();
}