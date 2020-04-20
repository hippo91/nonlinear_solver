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


int print_array(const p_array arr) {
    unsigned int chunk_size = PRINT_ARRAY_CHUNK_SIZE;
    for (unsigned int i=0; i < arr->size; ++i) {
        if ((i == chunk_size) && (arr->size > chunk_size * 2)) {
            i = arr->size - chunk_size;
            printf("...\n");
        }
        int ret = printf("%s[%d] = %15.9g\n", arr->label, i, *(arr->data + i));
        if (ret < 0) {
            perror("An error occured during the print of the array!");
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}