#include <stdlib.h>
#include "array.h"

/**
 * @brief Computes the vector of increments according to the classic Newton's formula : 
 *        \f$\Delta x = -\frac{f}{df}\f$
 * 
 * @param x_k[in] : vector of unknowns (useless here)
 * @param func[in] : vector of the value of the function
 * @param dfunc[in] : vector of the value of the derivative of the function
 * @param vector_of_increments[out] : vector of the value of increments
 */
void classical_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

/**
 * @brief Compute the vector of increments according to the modified Newton's formula :
 *        \f$\Delta x = -0.5\frac{f}{df}\f$
 * 
 * @param x_k[in] : vector of unknowns (useless here)
 * @param func[in] : vector of the value of the function
 * @param dfunc[in] : vector of the value of the derivative of the function
 * @param vector_of_increments[out] : vector of the value of increments
 */
void damped_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

/**
 * @brief Compute the vector of increments in a way that ensures the vector of unknowns (x) won't change sign
 * 
 * @param x_k[in] : vector of unknowns
 * @param func[in] : vector of the value of the function
 * @param dfunc[in] : vector of the value of the derivative of the function
 * @param vector_of_increments[out] : vector of the value of increments
 */
void ensure_same_sign_incrementation(const p_array x_k, const p_array func, const p_array dfunc, p_array vector_of_increments);

/**
 * @brief A prototype for incrementation functions that are to be used in Newton algorithm
 * 
 */
typedef void (*incrementation_fct_ptr)(const p_array, const p_array, const p_array, p_array);