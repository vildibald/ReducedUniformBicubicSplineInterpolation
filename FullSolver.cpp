//
// Created by Viliam on 16.05.2018.
//

#include "FullSolver.h"


FullSolver::FullSolver()
= default;

Tridiagonal FullSolver::CreateTridiagonal(const KnotVector& knots)
{
    return Tridiagonal::Factory::CreateFullTridiagonal(knots);
}
