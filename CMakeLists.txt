cmake_minimum_required(VERSION 3.10)
project(ReducedUniformBicubicSplineInterpolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native")

#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=core2")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=nehalem")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=sandybridge")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=haswell")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=skylake")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=barcelona")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=bdver1")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=bdver4")
#set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=znver1")



add_executable(SecondReducedUniformBicubicSplineInterpolation main.cpp ComparisonBenchmarkResult.cpp ComparisonBenchmarkResult.h utils.cpp utils.h Tridiagonal.cpp Tridiagonal.h Spline.cpp Spline.h MathFunction.cpp MathFunction.h KnotVector.h KnotMatrix.h Timer.h StopWatch.h MultiThreadPreparator.h SplineDerivativesAlgorithm.h FullSolver.cpp FullSolver.h FirstReducedSolver.cpp FirstReducedSolver.h)