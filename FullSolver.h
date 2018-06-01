//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FULLSOLVER_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FULLSOLVER_H

#include "Tridiagonal.h"

class FullSolver {
public:
    FullSolver();

    template<typename ParameterGetter, typename DerivationGetter, typename
    DerivationSetter>
    void Solve(Tridiagonals &tridiagonals,
               const ParameterGetter &p, const DerivationGetter &dget,
               DerivationSetter &dset, size_t systemIdx) {
        auto &tridiagonal = tridiagonals.Get();
        auto &rightSide = tridiagonal.RightSideBuffer();
        const auto threeDivH = 3.0 / tridiagonal.H();
        const auto derivationCount = tridiagonal.ProblemSize();
        const auto equationCount = tridiagonal.EquationCount();
        for (size_t i = 0; i < equationCount; ++i) {
            rightSide[i] = threeDivH * (p(i + 2, systemIdx) - p(i, systemIdx));
        }

        rightSide.front() -= dget(0, systemIdx);;
        rightSide[equationCount - 1] -= dget(derivationCount - 1, systemIdx);
        tridiagonal.Solve();

        for (auto i = 0; i < equationCount; ++i) {
            dset(i + 1, systemIdx, rightSide[i]);
        }
    }

    Tridiagonal CreateTridiagonal(const KnotVector &knots);
};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FULLSOLVER_H
