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
    unsigned int chunk_size = 10;
    typedef struct pair {
        unsigned int begin;
        unsigned int end;
    } s_pair;
    s_pair iters[2];
    unsigned int iters_size = 2;
    if (arr->size <= chunk_size) {
        iters[0].begin = 0;
        iters[0].end = arr->size;
        iters_size = 1;
    } else {
        iters[0].begin = 0;
        iters[0].end = chunk_size;
        iters[1].begin = arr->size - chunk_size;
        iters[1].end = arr->size;
    }
    for (unsigned int iter_index=0; iter_index < iters_size; ++iter_index) {
        for (unsigned int i=iters[iter_index].begin; i < iters[iter_index].end; ++i) {
            printf("%s[%d] = %15.9g\n", arr->label, i, *(arr->data + i));
        }
        if (iter_index != iters_size - 1) printf("...\n");
    }
    return EXIT_SUCCESS;
}