#include "cubic.h"
#include <assert.h>

#include "array.h"


void cubic_function(__attribute__((unused)) void *params, const p_array x, p_array fx, p_array dfx)
{
    assert(is_valid_array(x));
    assert(is_valid_array(fx));
    assert(is_valid_array(dfx));
    assert(x->size == fx->size);
    assert(x->size == dfx->size);

    for (unsigned int i = 0; i < x->size; ++i)
    {
        fx->data[i] = x->data[i] * x->data[i] * x->data[i] - 2. * x->data[i] * x->data[i] + 1;
        dfx->data[i] = 3. * x->data[i] * x->data[i] - 4. * x->data[i];
    }
}