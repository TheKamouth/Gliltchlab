#ifndef CURVE_H
#define CURVE_H

#include "ControlPoint.h"

#include <QMap>

class Curve
{
public:
    Curve();
    float Evaluate(float time);

private:
    QMap< float,ControlPoint > _controlPoints;
    float _duration;
    float _minValue;
    float _maxValue;
};

#endif // CURVE_H
