#ifndef CURVE_H
#define CURVE_H

#include "ControlPoint.h"
#include "ClipBase.h"

#include "ITrackDataSequence.h"

#include <QMap>
#include <QUrl>

class Curve : public ClipBase
{
public:
    Curve(QUrl url);

    // Duration is a property of all BaseClip implementation
    // Add IClip
    float Duration();
    float Evaluate(float time);

    float LowerBound();
    float UpperBound();
    float Range();

    ControlPoint * AddPoint(float time, ControlPoint controlPoint);

    QMap< float,ControlPoint > * ControlPoints();

private:
    QMap< float,ControlPoint > _controlPoints;
    float _durationMs;
    float _lowerBound;
    float _upperBound;
};

#endif // CURVE_H
