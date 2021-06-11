#include "Curve.h"

#include "TimelineConstants.h"

#include <QDebug>

Curve::Curve() :
    _durationMs( 10000.0f),
    _lowerBound( -1.0f),
    _upperBound( 1.0f)
{
    _controlPoints.insert(0.0f, ControlPoint());
    _controlPoints.insert(_durationMs, ControlPoint());
}

float Curve::Evaluate(float time)
{
    if( time < 0.0f || qFuzzyCompare(time, 0.0f))
    {
        ControlPoint & firstControlPoint = _controlPoints.first();
        return firstControlPoint.Value();
    }
    else if( time > _durationMs || qFuzzyCompare(time, _durationMs))
    {
        ControlPoint & lastControlPoint = _controlPoints.last();
        return lastControlPoint.Value();
    }
    else
    {
        // iterate over keys, when control point key is greater than time we found nextControlPoint
        QMap< float,ControlPoint >::iterator previousControlPointIterator = _controlPoints.begin();
        QMap< float,ControlPoint >::iterator nextControlPointIterator = _controlPoints.begin();
        nextControlPointIterator++;

        float keyTime = nextControlPointIterator.key();

        while( keyTime < time)
        {
            nextControlPointIterator++;
            previousControlPointIterator++;

            keyTime = nextControlPointIterator.key();
        }

        ControlPoint previousControlPoint = previousControlPointIterator.value();
        ControlPoint nextControlPoint = nextControlPointIterator.value();

        float previousValue = previousControlPoint.Value();
        float nextValue = nextControlPoint.Value();

        float previousKeyTime = previousControlPointIterator.key();
        float nextKeyTime = nextControlPointIterator.key();

        InterpolationMode previousValueMode = previousControlPoint.Mode();

        float value = 0.0f;

        switch(previousValueMode)
        {
            case Constant:
                value = previousValue;
                break;

            case LinearInterp:
            {
                float timeInterval = nextKeyTime - previousKeyTime;
                float alpha = (time - previousKeyTime) / timeInterval;
                value = alpha * previousValue + ( 1.0f - alpha ) * nextValue;
                break;
            }

            default:
            {
                qDebug() << "This interpolationMode is not properly handled";

                float timeInterval = nextKeyTime - previousKeyTime;
                float alpha = (time - previousKeyTime) / timeInterval;
                value = alpha * previousValue + ( 1.0f - alpha ) * nextValue;
                break;
            }
        }

        return value;
    }
}

void Curve::AddPoint(float time, ControlPoint controlPoint)
{
    qDebug() << "Adding control point at " << time << "ms, value = " << controlPoint.Value();

    _controlPoints.insert(time, controlPoint);
}

float Curve::LowerBound()
{
    return _lowerBound;
}

float Curve::UpperBound()
{
    return _upperBound;
}

float Curve::Range()
{
    return std::abs(_upperBound - _lowerBound);
}

float Curve::Duration()
{
    return _durationMs;
}

QMap<float, ControlPoint> * Curve::ControlPoints()
{
    return &_controlPoints;
}
