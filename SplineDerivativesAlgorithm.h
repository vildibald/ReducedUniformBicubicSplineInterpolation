//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_SPLINEDERIVATIVESALGORITHM_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_SPLINEDERIVATIVESALGORITHM_H

#include "MathFunction.h"
#include "Spline.h"
#include <vector>
#include "Tridiagonal.h"
#include "utils.h"


template <typename Solver>
class SplineDerivativesAlgorithm
{
    InterpolativeMathFunction function_;
    Tridiagonals xTridiagonals_;
    Tridiagonals yTridiagonals_;
    Timer timer_;
    bool isParallel_;
    Solver solver_;

public:
    SplineDerivativesAlgorithm(const InterpolativeMathFunction f, Solver solver)
            :function_(f), xTridiagonals_(),
             yTridiagonals_(), isParallel_(false), solver_(solver)
    {
    }

    Spline Calculate(const KnotVector xVector,
                     const KnotVector yVector)
    {
        Spline spline{ std::move(xVector), std::move(yVector) };
        Initialize(spline);
        timer_.Reset();
        timer_.Start();
        FillDx(spline);
        FillDy(spline);
        FillDxy(spline);
        FillDyx(spline);
        timer_.Stop();

        return spline;
    }

    long long ExecutionTime()
    {
        return timer_.ExecutionTime();
    }

    void InParallel(bool value)
    {
        isParallel_ = value;
    }

    bool IsParallel() const
    {
        return isParallel_;
    }

private:
    void Initialize(Spline& spline)
    {
        InitializeTridiagonals(spline);
        spline.Initialize(function_);
    }

    void FillDx(Spline& spline)
    {
        const auto parameterGetter = [&](size_t i, size_t j)
        {
            return spline.Z(i, j);
        };
        const auto derivationGetter = [&](size_t i, size_t j)
        {
            return spline.Dx(i, j);
        };
        auto derivationSetter = [&](size_t i, size_t j, double value)
        {
            spline.SetDx(i, j, value);
        };
        FillD(spline.ColumnsCount(), xTridiagonals_, parameterGetter,
              derivationGetter, derivationSetter);
    }

    void FillDy(Spline& spline)
    {
        const auto parameterGetter = [&](size_t i, size_t j)
        {
            return spline.Z(j, i);
        };
        const auto derivationGetter = [&](size_t i, size_t j)
        {
            return spline.Dy(j, i);
        };
        auto derivationSetter = [&](size_t i, size_t j, double value)
        {
            spline.SetDy(j, i, value);
        };
        FillD(spline.RowsCount(), yTridiagonals_, parameterGetter, derivationGetter, derivationSetter);
    }

    void FillDxy(Spline& spline)
    {
        const auto parameterGetter = [&](size_t i, size_t j)
        {
            return spline.Dy(i, j);
        };
        const auto derivationGetter = [&](size_t i, size_t j)
        {
            return spline.Dxy(i, j);
        };
        auto derivationSetter = [&](size_t i, size_t j, double value)
        {
            spline.SetDxy(i, j, value);
        };

        size_t loop[] = { 0, spline.ColumnsCount() - 1 };
        for (auto j : loop)
        {
            solver_.Solve(xTridiagonals_, parameterGetter, derivationGetter, derivationSetter, j);
        }
    }

    void FillDyx(Spline& spline)
    {
        const auto parameterGetter = [&](size_t i, size_t j)
        {
            return spline.Dx(j, i);
        };
        const auto derivationGetter = [&](size_t i, size_t j)
        {
            return spline.Dxy(j, i);
        };
        auto derivationSetter = [&](size_t i, size_t j, double value)
        {
            spline.SetDxy(j, i, value);
        };
        FillD(spline.RowsCount(), yTridiagonals_, parameterGetter, derivationGetter, derivationSetter);
    }

    void Parallelize(bool inParallel)
    {
        xTridiagonals_.Parallelize(inParallel);
        yTridiagonals_.Parallelize(inParallel);
    }

    void InitializeTridiagonals(Spline& spline)
    {
        xTridiagonals_.GetAll().clear();
        xTridiagonals_.GetAll().clear();

        xTridiagonals_.GetAll().emplace_back(solver_.CreateTridiagonal(spline.X()));
        yTridiagonals_.GetAll().emplace_back(solver_.CreateTridiagonal(spline.Y()));

        Parallelize(isParallel_);
    }

    template <typename ParameterGetter, typename DerivationGetter, typename
    DerivationSetter>
    void FillD(const size_t systemCount, Tridiagonals& tridiagonals,
               const ParameterGetter& p, const DerivationGetter& dget,
               DerivationSetter& dset)
    {
        utils::For(0, static_cast<int>(systemCount), 1, isParallel_, [&](int j)
        {
            solver_.Solve(tridiagonals, p, dget, dset, j);
        });
    }

};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_SPLINEDERIVATIVESALGORITHM_H
