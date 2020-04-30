#include "array.h"

/**
 * @brief Evaluate the value of \f$x^3 - 2x^2 + 1\f$ and its derivative
 *        \f$3x^2 - 4x\f$
 * @param[in] x : array of unknowns
 * @param[out] fx : array of values of f
 * @param[out] dfx : array of values of df/dx
 */
void cubic_function(void *params, const p_array x, p_array fx, p_array dfx);