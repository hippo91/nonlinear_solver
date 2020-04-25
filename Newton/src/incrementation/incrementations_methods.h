#include <stdlib.h>
#include "array.h"

void classical_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

void damped_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

void ensure_same_sign_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

typedef void (*incrementation_fct_ptr)(const p_array, const p_array, const p_array, p_array);