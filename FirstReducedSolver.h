//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FIRSTREDUCEDSOLVER_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FIRSTREDUCEDSOLVER_H

#include "Tridiagonal.h"

class FirstReducedSolver {
public:
    FirstReducedSolver();

    template<typename ParameterGetter, typename DerivationGetter, typename
    DerivationSetter>
    void Solve(Tridiagonals &tridiagonals,
               const ParameterGetter &p, const DerivationGetter &dget,
               DerivationSetter &dset, size_t systemIdx) {
        auto &tridiagonal = tridiagonals.Get();
        auto &rightSide = tridiagonal.RightSideBuffer();

        const auto derivationCount = tridiagonal.ProblemSize();
        const auto even = derivationCount % 2 == 0;
        const auto equationsCount = tridiagonal.EquationCount();

        const auto h = tridiagonal.H();
        const auto threeDivH = 3.0 / h;
        const auto twelveDivH = 4.0 * threeDivH;

        for (size_t i = 0; i < equationsCount - 1; ++i) {
            const auto i21 = 2 * (i + 1);
            rightSide[i] = threeDivH * (p(i21 + 2, systemIdx) - p(i21 - 2, systemIdx))
                           - twelveDivH * (p(i21 + 1, systemIdx) - p(i21 - 1, systemIdx));
        }
        rightSide.front() -= dget(0, systemIdx);
        {
            const size_t i = equationsCount - 1;
            const auto i21 = 2 * (i + 1);
            if (even) {
                // TODO: Needs support for even count.
                rightSide[i] = 0;
            } else {
                rightSide[i] = threeDivH * (p(i21 + 2, systemIdx) - p(i21 - 2, systemIdx))
                               - twelveDivH * (p(i21 + 1, systemIdx) - p(i21 - 1, systemIdx));
            }
            rightSide[i] -= dget(derivationCount - 1, systemIdx);
        }
        tridiagonal.Solve();

        const auto quarter = 1.0 / 4;
        for (size_t i = 0; i < equationsCount; ++i) {
            const auto evenI = 2 * (i + 1);
            dset(evenI, systemIdx, rightSide[i]);

            const auto oddI = 2 * i + 1;
            dset(oddI, systemIdx,
                 quarter * (threeDivH * (p(oddI + 1, systemIdx) - p(oddI - 1, systemIdx))
                            - dget(oddI + 1, systemIdx) - dget(oddI - 1, systemIdx)
                 )
            );
        }
        {
            const size_t i = equationsCount;
            const auto oddI = 2 * i + 1;
            dset(oddI, systemIdx,
                 quarter * (threeDivH * (p(oddI + 1, systemIdx) - p(oddI - 1, systemIdx))
                            - dget(oddI + 1, systemIdx) - dget(oddI - 1, systemIdx)
                 )
            );
        }
    }

    Tridiagonal CreateTridiagonal(const KnotVector &knots);
};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_FIRSTREDUCEDSOLVER_H
