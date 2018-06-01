//
// Created by Viliam on 16.05.2018.
//

#ifndef SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MATHFUNCTION_H
#define SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MATHFUNCTION_H


#include <functional>

typedef std::function<double(double, double)> MathFunction;

class InterpolativeMathFunction
{
    MathFunction z_;
    MathFunction dx_;
    MathFunction dy_;
    MathFunction dxy_;
public:
    InterpolativeMathFunction(MathFunction function);

    const MathFunction& Z() const;

    const MathFunction& Dx() const;

    const MathFunction& Dy() const;

    const MathFunction& Dxy() const;
};


#endif //SECONDREDUCEDUNIFORMBICUBICSPLINEINTERPOLATION_MATHFUNCTION_H
