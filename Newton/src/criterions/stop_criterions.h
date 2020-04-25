#include <stdbool.h>
#include <stdlib.h>
#include "array.h"

int relative_gap(p_array delta_x_k, p_array func, bool *has_converged);

typedef int (*criterion_fct_ptr)(p_array delta_x_k, p_array func, bool *has_converged);
