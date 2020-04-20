#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int main() {
    const unsigned int expected_size = 10;
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
}