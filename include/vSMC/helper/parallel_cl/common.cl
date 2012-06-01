#include <vSMC/helper/parallel_cl/rng.h>

__kernel
void copy (size_t size, __global state_struct *state, __global uint *source)
{
    size_t to = get_global_id(0);
    
    if (to >= size)
        return;

    size_t from = source[to];
    state[to] = state[from];
}
