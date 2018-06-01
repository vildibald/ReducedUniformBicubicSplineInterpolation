//
// Created by Viliam on 16.05.2018.
//

#include "Tridiagonal.h"
#include "utils.h"
#include <algorithm>

std::vector<Tridiagonal> &Tridiagonals::GetAll() {
    return tridiagonals_;
}

Tridiagonal &Tridiagonals::Get() {
    if (tridiagonals_.size() == 1) {
        return tridiagonals_[0];
    }
    return tridiagonals_[omp_get_thread_num()];
}

std::vector<double> &
Tridiagonal::ResetBufferAndGet() {
    auto &buffer = luBuffer_;
    std::fill(buffer.begin(), buffer.end(), 1);
    return buffer;
}

std::vector<double> &
Tridiagonal::Buffer() {
    return luBuffer_;
}

std::vector<double> &
Tridiagonal::Solve() {
    auto &buffer = Buffer();
    utils::SolveDeboorTridiagonalSystemBuffered(mainDiagonal_,
                                                &rightSideBuffer_.front(), equationCount_,
                                                &buffer.front(), mainDiagonalLast_);
    return rightSideBuffer_;
}

Tridiagonal::Tridiagonal(const double mainDiagonal,
                         const double mainDiagonalLast,
                         const double h,
                         const size_t equationCount,
                         const size_t problemSize)
        : luBuffer_(equationCount),
          rightSideBuffer_(equationCount),
          mainDiagonal_(mainDiagonal),
          mainDiagonalLast_(mainDiagonalLast),
          h_(h),
          equationCount_(equationCount),
          problemSize_(problemSize) {
    luBuffer_.assign(equationCount, 0);
    rightSideBuffer_.assign(equationCount, 0);
}


std::vector<double> &Tridiagonal::RightSideBuffer() {
    return rightSideBuffer_;
}

double Tridiagonal::H() const {
    return h_;
}

size_t Tridiagonal::EquationCount() const {
    return equationCount_;
}

size_t Tridiagonal::ProblemSize() const {
    return problemSize_;
}


Tridiagonal Tridiagonal::Factory::CreateEmptyTridiagonal() {
    Tridiagonal tridiagonal(0,
                            0,
                            0,
                            0,
                            0
    );
    return tridiagonal;
}

Tridiagonal
Tridiagonal::Factory::CreateFullTridiagonal(const KnotVector &knotVector) {
    const auto numEquations = knotVector.size() - 2;
    const auto h = knotVector[1] - knotVector[0];
    Tridiagonal tridiagonal(
            4,
            4,
            h,
            numEquations,
            knotVector.size()
    );
    return tridiagonal;
}

Tridiagonal
Tridiagonal::Factory::CreateFirstReducedTridiagonal(const KnotVector &knotVector) {
    const auto numKnots = knotVector.size();
    const auto even = numKnots % 2 == 0;
//    const auto numEquations = even ? numKnots / 2 - 2 : numKnots / 2 - 1;
    const auto numEquations = numKnots / 2 - 1;
    const auto h = knotVector[1] - knotVector[0];
    auto mainDiagonalLast = -14;
    if (even) {
        mainDiagonalLast = -15;
    }

    Tridiagonal tridiagonal(
            -14,
            mainDiagonalLast,
            h,
            numEquations,
            knotVector.size()
    );
    return tridiagonal;
}
