#include <iostream>
#include <algorithm>
#include "ComparisonBenchmarkResult.h"
#include <numeric>
#include "StopWatch.h"
#include "MathFunction.h"
#include "SplineDerivativesAlgorithm.h"
#include <limits>
#include <cmath>
#include "FirstReducedSolver.h"
#include "FullSolver.h"


KnotVector TestVector(const double from, const double to, const size_t size) {
    KnotVector vector(size);
    const auto offset = (to - from) / (size - 1);
    auto x = from;
    for (size_t i = 0; i < size; ++i) {
        vector[i] = x;
        x += offset;
    }
    return vector;
}

ComparisonBenchmarkResult SurfaceBenchmark(unsigned int numIterations, unsigned int numKnots,
                                           const bool inParallel = false) {
    const MathFunction function = [](double x, double y) {
        return sin(sqrt(x * x + y * y));
    };

    SplineDerivativesAlgorithm<FullSolver> full(function, FullSolver());
    SplineDerivativesAlgorithm<FirstReducedSolver> firstReduced(function, FirstReducedSolver());

    full.InParallel(inParallel);
    firstReduced.InParallel(inParallel);

    numKnots = numKnots % 2 == 0 ? numKnots + 1 : numKnots;

    std::vector<double> calculatedResults;
    std::vector<long long> fullTimes;
    fullTimes.reserve(numIterations);
    std::vector<long long> firstReducedTimes;
    firstReducedTimes.reserve(numIterations);
    const auto vector = TestVector(-2, 2, numKnots);

    for (size_t i = 0; i < numIterations; i++) {
        auto xKnots = vector;
        auto yKnots = vector;
        auto result = full.Calculate(std::move(xKnots), std::move(yKnots));
        calculatedResults.push_back(result.Dxy(1, 1));
        fullTimes.push_back(full.ExecutionTime());
    }

    for (size_t i = 0; i < numIterations; i++) {
        auto xKnots = vector;
        auto yKnots = vector;
        auto result = firstReduced.Calculate(std::move(xKnots), std::move(yKnots));
        calculatedResults.push_back(result.Dxy(1, 1));
        firstReducedTimes.push_back(firstReduced.ExecutionTime());
    }

    const auto fullTime = static_cast<unsigned long long>(static_cast<double>(std::accumulate(fullTimes.begin(),
                                                                                              fullTimes.end(), 0))
                                                          / static_cast<double>(numIterations));
    const auto firstReducedTime = static_cast<unsigned long long>(static_cast<double>(std::accumulate(
            firstReducedTimes.begin(), firstReducedTimes.end(), 0))
                                                                  / static_cast<double>(numIterations));
    // Just to avoid accidental compiler "throw away unused code" optimizations
    std::cout << "Ignore " << calculatedResults[0] << std::endl;
    ComparisonBenchmarkResult benchmarkResult;
    benchmarkResult.Add(fullTime).Add(firstReducedTime);
    return benchmarkResult;
}

void PrintSurfaceDeboorResult(ComparisonBenchmarkResult &result) {
    std::cout << "Full : " << result[0] << std::endl;
    std::cout << "Reduced : " << result[1] << std::endl;
    std::cout << "Difference F/R: " << result.Ratio(0, 1) << std::endl;
}

void PerformSurfaceBenchmark(ComparisonBenchmarkResult &result, bool inParallel = false) {
    unsigned int numIterations;
    unsigned int numKnots;
    std::cout << "Spline surface benchmark" << std::endl << std::endl;
    std::cout << "Enter number of iterations: " << std::endl;
    std::cin >> numIterations;
    std::cout << "Enter number of knots: " << std::endl;
    std::cin >> numKnots;
    std::cin.get();

    result = SurfaceBenchmark(numIterations, numKnots, inParallel);
    PrintSurfaceDeboorResult(result);
}

void PerformSurfaceBenchmarkInParallel(ComparisonBenchmarkResult &result) {
    std::cout << "Spline surface benchmark (in parallel)" << std::endl <<
              std::endl;
    PerformSurfaceBenchmark(result, true);
}

void PerformSurfaceBenchmarkInSerial(ComparisonBenchmarkResult &result) {
    std::cout << "Spline surface benchmark" << std::endl << std::endl;
    PerformSurfaceBenchmark(result);
}



void EqualityComparison() {
    const MathFunction function = [](double x, double y) {
        return sin(sqrt(x * x + y * y));
    };
    SplineDerivativesAlgorithm<FullSolver> full(function, FullSolver());
    SplineDerivativesAlgorithm<FirstReducedSolver> firstReduced(function, FirstReducedSolver());
    const unsigned int numKnots = 13;

    auto vector = TestVector(-20, 20, numKnots);

    auto xKnots = vector;
    auto yKnots = vector;
    auto resultFull = full.Calculate(std::move(xKnots), std::move(yKnots));

    xKnots = vector;
    yKnots = vector;
    auto resultFirstReduced = firstReduced.Calculate(std::move(xKnots), std::move(yKnots));

    auto minDiffDx = (std::numeric_limits<double>::min)(), maxDiffDx = (std::numeric_limits<double>::
    min)();
    auto minDiffDy = (std::numeric_limits<double>::max)(), maxDiffDy = (std::numeric_limits<double>::
    min)();
    auto minDiffDxy = (std::numeric_limits<double>::max)(), maxDiffDxy = (std::numeric_limits<double>::
    min)();

    InterpolativeMathFunction interpolativeMathFunction = function;
    std::cout << "---------- Knot matrix ----------" << std::endl;
    for (unsigned int i = 0; i < resultFull.RowsCount(); ++i) {
        std::cout << "Row " << i << " :\n";
        for (unsigned int j = 0; j < resultFull.ColumnsCount(); ++j) {
            minDiffDx = (std::min)(minDiffDx, std::abs(
                    resultFull.Dx(i, j) - resultFirstReduced.Dx(i, j)));
            minDiffDy = (std::min)(minDiffDy, std::abs(
                    resultFull.Dy(i, j) - resultFirstReduced.Dy(i, j)));
            minDiffDxy = (std::min)(minDiffDxy, std::abs(
                    resultFull.Dxy(i, j) - resultFirstReduced.Dxy(i, j)));

            maxDiffDx = (std::max)(maxDiffDx, std::abs(
                    resultFull.Dx(i, j) - resultFirstReduced.Dx(i, j)));
            maxDiffDy = (std::max)(maxDiffDy, std::abs(
                    resultFull.Dy(i, j) - resultFirstReduced.Dy(i, j)));
            maxDiffDxy = (std::max)(maxDiffDxy, std::abs(
                    resultFull.Dxy(i, j) - resultFirstReduced.Dxy(i, j)));

            std::cout << "\nColumn " << j << ":\n"
                      << "M. z: " << interpolativeMathFunction.Z()(vector[i], vector[j]) << '\n'
                      << "M. dx: " << interpolativeMathFunction.Dx()(vector[i], vector[j]) << '\n'
                      << "M. dy: " << interpolativeMathFunction.Dy()(vector[i], vector[j]) << '\n'
                      << "M. dxy: " << interpolativeMathFunction.Dxy()(vector[i], vector[j]) << "\n\n";
            std::cout << "F. z: " << resultFull.Z(i, j) << '\n'
                      << "F. dx: " << resultFull.Dx(i, j) << '\n'
                      << "F. dy: " << resultFull.Dy(i, j) << '\n'
                      << "F. dxy: " << resultFull.Dxy(i, j) << "\n\n";
            std::cout << "1. R. z: " << resultFirstReduced.Z(i, j) << '\n'
                      << "1. R. dx: " << resultFirstReduced.Dx(i, j) << '\n'
                      << "1. R. dy: " << resultFirstReduced.Dy(i, j) << '\n'
                      << "1. R. dxy: " << resultFirstReduced.Dxy(i, j) << "\n\n";
        }
        std::cout << std::endl;
    }

    std::cout << "-------------------------------" << std::endl;

    std::cout << "Min diff Dx: " << minDiffDx << std::endl;
    std::cout << "Max diff Dx: " << maxDiffDx << std::endl;
    std::cout << "Min diff Dy: " << minDiffDy << std::endl;
    std::cout << "Max diff Dy: " << maxDiffDy << std::endl;
    std::cout << "Min diff Dxy: " << minDiffDxy << std::endl;
    std::cout << "Max diff Dxy: " << maxDiffDxy << std::endl;
}

void EqualityComparisonDebug() {
    const MathFunction function = [](double x, double y) {
        return sin(sqrt(x * x));
    };

    SplineDerivativesAlgorithm<FullSolver> full(function, FullSolver());
    SplineDerivativesAlgorithm<FirstReducedSolver> firstReduced(function, FirstReducedSolver());
    std::vector<double> vector = {-6, -5, -4, -3, -2, -1, 0, 1, 2, 3, 4, 5, 6};
    const auto numKnots = static_cast<const unsigned int>(vector.size());

    auto xKnots = vector;
    auto yKnots = vector;
    auto resultFull = full.Calculate(std::move(xKnots), std::move(yKnots));

    xKnots = vector;
    yKnots = vector;
    auto resultFirstReduced = firstReduced.Calculate(std::move(xKnots), std::move(yKnots));

    auto minDiffDx = (std::numeric_limits<double>::min)(), maxDiffDx = (std::numeric_limits<double>::
    min)();

    InterpolativeMathFunction interpolativeMathFunction = function;
    std::cout << std::fixed << "---------- Knot vector ----------" << std::endl;
    std::cout.precision(std::numeric_limits<double>::max_digits10);
    for (unsigned int i = 0; i < resultFull.ColumnsCount(); ++i) {
        minDiffDx = (std::min)(minDiffDx, std::abs(
                resultFull.Dx(i, 0) - resultFirstReduced.Dx(i, 0)));
        maxDiffDx = (std::max)(maxDiffDx, std::abs(
                resultFull.Dx(i, 0) - resultFirstReduced.Dx(i, 0)));
        maxDiffDx = (std::max)(maxDiffDx, std::abs(
                resultFull.Dx(i, 0) - resultFirstReduced.Dx(i, 0)));
        std::cout << "\nColumn " << i << ":\n"
                  << "M. dx: " << interpolativeMathFunction.Dx()(vector[i], 0) << std::endl;
        std::cout << "F. dx: " << resultFull.Dx(i, 0) << std::endl;
        std::cout << "1. R. dx: " << resultFirstReduced.Dx(i, 0) << std::endl;
        minDiffDx = (std::min)(minDiffDx, std::abs(resultFull.Dx(i, 0) - resultFirstReduced.Dx(i, 0)));
        std::cout << std::endl;
    }

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Min diff Dx: " << minDiffDx << std::endl;
    std::cout << "Max diff Dx: " << maxDiffDx << std::endl;
}


int main() {
    while (true) {
        // Console clear ...
        // ... for Windows,
        system("cls");
        // ... for Linux/Unix.
        //system("clear");
        std::cout << "1: Spline surface benchmark." << std::endl;
        std::cout << "2: Spline surface benchmark (options)." << std::endl;
        std::cout << "3: Equality comparison." << std::endl;
        std::cout << "4: Debug equality comparison." << std::endl;
        std::cout << "Q: End program" << std::endl;
        char input;
        std::cin >> input;
        std::cin.get();
        std::cout << std::endl << "---------------" << std::endl;

        ComparisonBenchmarkResult result;
        switch (input) {
            case '1':
                PerformSurfaceBenchmarkInSerial(result);
                break;
            case '2':
                PerformSurfaceBenchmarkInParallel(result);
                break;
            case '3':
                EqualityComparison();
                break;
            case '4':
                EqualityComparisonDebug();
                break;
            case 'q':
            case 'Q':
                return 0;
            default:;
        }

        std::cout << "===================" << std::endl;
        system("pause");
    }
}