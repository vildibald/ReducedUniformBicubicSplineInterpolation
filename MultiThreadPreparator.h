//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MULTITHREADPREPARATOR_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MULTITHREADPREPARATOR_H


#include <vector>
#include <thread>

class MultithreadPreparator {

public:

    template<typename T>
    void PrepareVector(std::vector<T>& vector) {
        auto threads = std::thread::hardware_concurrency();
        vector.reserve(threads);
        for (auto i = vector.size(); i < threads; i++) {
            // create copy of tridiagonal solver
            vector.emplace_back(vector[0]);
        }
    }

    template<typename T>
    void UnprepareVector(std::vector<T>& vector) {
        for (int i = 0; i < vector.size() - 1; ++i) {
            vector.pop_back();
        }
    }

    template<typename T>
    void PrepareVector(const bool inParallel, std::vector<T>& vector) {
        if (inParallel) {
            PrepareVector(vector);
        }
        else {
            UnprepareVector(vector);
        }
    }
};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MULTITHREADPREPARATOR_H
