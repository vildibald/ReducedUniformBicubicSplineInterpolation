//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_UTILS_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_UTILS_H


#include <omp.h>
#include <thread>
#include <vector>

namespace utils
{
    extern double DOUBLE_MIN;

    extern unsigned int LOGICAL_THREAD_COUNT;

    template <typename Iterator, typename Function>
    void For(Iterator from, Iterator to, Iterator incrementBy, const bool inParallel,
             Function function)
    {
        if (inParallel)
        {
#pragma omp parallel for
            for (Iterator i = from; i < to; i += incrementBy)
            {
                function(i);
            }
        }
        else
        {
            // Loop for sequential computations.
            // '#pragma omp parallel for if(inParallel)' in case of inParallel==false will execute
            // such loop in one thread, but still with overhead of OpenMP thread creation.
            for (Iterator i = from; i < to; i += incrementBy)
            {
                function(i);
            }
        }
    }

    void SolveDeboorTridiagonalSystemBuffered(double main_diagonal_value,
                                              double right_side[], size_t num_equations,
                                              double buffer[], double
                                              last_main_diagonal_value = DOUBLE_MIN);

};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_UTILS_H
