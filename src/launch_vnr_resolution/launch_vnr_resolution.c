#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "array.h"
#include "incrementations_methods.h"
#include "newton.h"
#include "stop_criterions.h"
#include "vnr_internalenergy_evolution.h"
#include "miegruneisen.h"
#include "miegruneisen_params.h"

void launch_vnr_resolution(MieGruneisenParams_s const * eos_params,
                           p_array old_specific_volume, p_array new_specific_volume,
                           p_array pressure, p_array internal_energy,
                           p_array solution, p_array new_p,
                           p_array new_vson)
{
    assert(is_valid_array(old_specific_volume));
    assert(is_valid_array(new_specific_volume));
    assert(is_valid_array(pressure));
    assert(is_valid_array(internal_energy));
    assert(is_valid_array(solution));
    assert(is_valid_array(new_p));
    assert(is_valid_array(new_vson));

    const unsigned int pb_size = old_specific_volume->size;

    assert(pb_size == new_specific_volume->size);
    assert(pb_size == pressure->size);
    assert(pb_size == internal_energy->size);
    assert(pb_size == solution->size);
    assert(pb_size == new_p->size);
    assert(pb_size == new_vson->size);

    // Function to solve (internal energy evolution in the vNR scheme)
#pragma omp parallel
    {
        /* Obtain and print thread id */
        int tid = omp_get_thread_num();
        int n_threads = omp_get_num_threads();

        int chunk_size = pb_size / n_threads;
        int offset = tid * chunk_size;
        int remain_chunk_size = pb_size % n_threads;
        if (tid == n_threads - 1) chunk_size += remain_chunk_size;

        // EOS definition
        MieGruneisenEOS_s mie_gruneisen_eos = {
            eos_params, NULL, NULL, NULL, NULL, NULL, 
            compute_pressure_and_derivative, compute_pressure_and_sound_speed,
            init, finalize};

        // Compute all terms that are parameters of the eos (i.e all that depends on specific_volume)
        int ret_code = mie_gruneisen_eos.init(&mie_gruneisen_eos, chunk_size, new_specific_volume->data + offset);
        if (ret_code == EXIT_FAILURE) {
            fprintf(stderr, "An error occured during MieGruneisen initialization!\n");
            fprintf(stderr, "Thread id : %d(/%d)\n", tid, n_threads);
            fprintf(stderr, "Chunk size : %d\n", chunk_size);
            exit(1);
        }

        s_array thread_old_spec_vol = {chunk_size, "Thread old specific volume", old_specific_volume->data + offset};
        s_array thread_new_spec_vol = {chunk_size, "Thread new specific volume", new_specific_volume->data + offset};
        s_array thread_internal_energy = {chunk_size, "Thread internal energy", internal_energy->data + offset};
        s_array thread_pressure = {chunk_size, "Thread pressure", pressure->data + offset};
        s_array thread_solution = {chunk_size, "Thread solution", solution->data + offset};

        VnrParameters_s VnrVars = {&thread_old_spec_vol,
                                   &thread_new_spec_vol,
                                   &thread_internal_energy,
                                   &thread_pressure,
                                   &mie_gruneisen_eos};
        NewtonParameters_s TheNewton = {internal_energy_evolution_VNR,
                                        classical_incrementation, relative_gap};
        ret_code = solveNewton(&TheNewton, &VnrVars, &thread_internal_energy, &thread_solution);

        if (ret_code == EXIT_FAILURE) {
            fprintf(stderr, "Unable to solve the equation! Aborting!\n");
            exit(1); // A bit weird to exit inside a thread.
        }
        // Appel de l'eos avec la solution du newton pour calculer la nouvelle
        // pression et vitesse du son
        VnrVars.miegruneisen->get_pressure_and_sound_speed(VnrVars.miegruneisen, chunk_size, new_specific_volume->data + offset,
                                                           solution->data + offset, new_p->data + offset, new_vson->data + offset);

        mie_gruneisen_eos.finalize(&mie_gruneisen_eos);
    }

}
