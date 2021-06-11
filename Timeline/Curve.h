#ifndef CURVE_H
#define CURVE_H

#include "ControlPoint.h"

#include <QMap>

class Curve
{
public:
    Curve();

    float Evaluate(float time);
    void AddPoint(float time, ControlPoint controlPoint);

    QMap< float,ControlPoint > * ControlPoints();

    float LowerBound();
    float UpperBound();
    float Range();
    float Duration();

private:
    QMap< float,ControlPoint > _controlPoints;
    float _durationMs;
    float _lowerBound;
    float _upperBound;
};

#endif // CURVE_H
