#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int main() {
    const unsigned int expected_size = 30;
    const char* expected_str = "An array for testing";
    s_array test_array;
    build_array(expected_size, expected_str, &test_array);

    if (test_array.size != expected_size) {
        fprintf(stderr, "Wrong value for the array's size : %d instead of %d", test_array.size, expected_size);
        return EXIT_FAILURE;
    }
    if (strcmp(test_array.label, expected_str) != 0) {
        fprintf(stderr, "Wrong value for the array's label : %s instead of %s", test_array.label, expected_str);
        return EXIT_FAILURE;
    }

    test_array.data[0] = 10.;
    test_array.data[9] = -10.;
    test_array.data[25] = 10.;
    test_array.data[29] = -10.;
    print_array(&test_array);

    s_array failed_array;
    unsigned int res = build_array(-10, "Unvalid", &failed_array);
    if (res == 0) {
        fprintf(stderr, "The build_array function should have failed because the array allocation should have failed!");
        return EXIT_FAILURE;
    }

    char too_long[129];
    for (int i=0; i<129; ++i) too_long[i] = 'a';
    res = build_array(10, too_long, &failed_array);
    if (res == 0) {
        fprintf(stderr, "The build_array function should have failed because the label is too long!");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}