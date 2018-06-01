//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_STOPWATCH_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_STOPWATCH_H

#include <chrono>

class StopWatch
{
public:
    StopWatch() = default;
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point stop_;

    void Start()
    {
        start_ = std::chrono::steady_clock::now();
    }

    void Stop()
    {
        stop_ =std::chrono::steady_clock::now();
    }

    long long EllapsedTime()
    {
        const auto diff = stop_ - start_;
        return std::chrono::duration_cast<std::chrono::microseconds>(diff).count();
    }
};

#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_STOPWATCH_H
