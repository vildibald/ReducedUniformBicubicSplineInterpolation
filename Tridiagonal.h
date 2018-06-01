//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TRIDIAGONAL_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TRIDIAGONAL_H


#include <vector>
#include "Spline.h"
#include "KnotVector.h"
#include "MultithreadPreparator.h"
#include <numeric>
#include "Timer.h"


class Tridiagonal;

class Tridiagonals;

class Tridiagonals final
{
    std::vector<Tridiagonal> tridiagonals_;

public:
    std::vector<Tridiagonal>& GetAll();

    Tridiagonal& Get();

    void Parallelize(const bool inParallel)
    {
        MultithreadPreparator multithreadPreparator;
        multithreadPreparator.PrepareVector(inParallel, GetAll());
    }
};

class Tridiagonal final
{
    std::vector<double> luBuffer_{};
    std::vector<double> rightSideBuffer_{};
    double mainDiagonal_;
    double mainDiagonalLast_;
    double h_;
    size_t equationCount_;
    size_t problemSize_;
    Timer timer_;

    Tridiagonal(double mainDiagonal,
                double mainDiagonalLast,
                double h,
                size_t equationCount,
                size_t problemSize
    );

public:

    std::vector<double>& Solve();

    std::vector<double>& ResetBufferAndGet();

    std::vector<double>& Buffer();

    double H() const;

    std::vector<double>& RightSideBuffer();

    size_t EquationCount() const;

    size_t ProblemSize() const;

    long long ExecutionTime() {
        return timer_.ExecutionTime();
    }

    class Factory final
    {
    public:

        static Tridiagonal
        CreateEmptyTridiagonal();

        static Tridiagonal
        CreateFullTridiagonal(const KnotVector& knotVector);

        static Tridiagonal
        CreateFirstReducedTridiagonal(const KnotVector& knotVector);

    };

};



#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_TRIDIAGONAL_H
