//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TIMER_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TIMER_H


#include "StopWatch.h"

class Timer final {
    StopWatch executionWatch;
public:
    long long ExecutionTime() {
        return executionWatch.EllapsedTime();
    }

    void Start() {
        executionWatch.Start();
    }

    void Stop() {
        executionWatch.Stop();
    }

    void Reset() {
        executionWatch = StopWatch();
    }
};

#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TIMER_H
