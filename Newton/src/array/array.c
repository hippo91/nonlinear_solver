#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"

int build_array(const unsigned int size, const char * label, p_array arr) {
    arr->data = (double *)calloc(size, sizeof(double));
    if (arr->data == NULL) {
        fprintf(stderr, "An error has occured when building the array %s\n", label);
        fprintf(stderr, "The allocation has failed!\n");
        return EXIT_FAILURE;
    }
    if (strlen(label) >= MAX_LABEL_SIZE) {
        fprintf(stderr, "An error has occured when building the array %s\n", label);
        fprintf(stderr, "The label size is above the limit : %d!\n", MAX_LABEL_SIZE);
        return EXIT_FAILURE;
    }
    strcpy(arr->label, label);
    arr->size = size;
    return EXIT_SUCCESS;
};