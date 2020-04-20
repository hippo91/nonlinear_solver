#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "incrementations_methods.h"
#include "newton.h"
#include "stop_criterions.h"
#include "vnr_internalenergy_evolution.h"
#ifndef MIEGRUNEISEN
#include "miegruneisen.h"
#define MIEGRUNEISEN
#endif
#include "utils.h"

void launch_vnr_resolution(p_array old_specific_volume, p_array new_specific_volume,
                           p_array pressure, p_array internal_energy,
                           p_array solution, p_array new_p,
                           p_array new_vson)
{
    // EOS definition
    double czero = 3940.0, S1 = 1.489, S2 = 0., S3 = 0., rhozero = 8930.0,
           grunzero = 2.02, b = 0.47, ezero = 0.;
    MieGruneisenParameters_t MieGruneisenParams = {
        czero, S1, S2, S3, rhozero, grunzero, b, ezero, compute_pressure_and_derivative, compute_pressure_and_sound_speed};
    // Function to solve (internal energy evolution in the vNR scheme)
#pragma omp parallel
    {
        /* Obtain and print thread id */
        int tid = omp_get_thread_num();
        int n_threads = omp_get_num_threads();

        const int pb_size = old_specific_volume->size;
        int chunk_size = pb_size / n_threads;
        int offset = tid * chunk_size;
        int remain_chunk_size = pb_size % n_threads;
        if (tid == n_threads - 1) chunk_size += remain_chunk_size;

        VnrVariables_t VnrVars = {old_specific_volume->data + offset,
                                  new_specific_volume->data + offset,
                                  internal_energy->data + offset,
                                  pressure->data + offset,
                                  &MieGruneisenParams};
        NewtonParameters_t TheNewton = {internal_energy_evolution_VNR,
                                        classical_incrementation, relative_gap};
        solveNewton(&TheNewton, &VnrVars, internal_energy->data + offset, chunk_size, solution->data + offset);
        // Appel de l'eos avec la solution du newton pour calculer la nouvelle
        // pression et vitesse du son
        VnrVars.miegruneisen->get_pressure_and_sound_speed(VnrVars.miegruneisen, chunk_size, new_specific_volume->data + offset,
                                                           solution->data + offset, new_p->data + offset, new_vson->data + offset);
    }
}
