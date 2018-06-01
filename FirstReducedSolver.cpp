//
// Created by Viliam on 16.05.2018.
//

#include "FirstReducedSolver.h"

FirstReducedSolver::FirstReducedSolver()
= default;

Tridiagonal FirstReducedSolver::CreateTridiagonal(const KnotVector& knots)
{
    return Tridiagonal::Factory::CreateFirstReducedTridiagonal(knots);
}
