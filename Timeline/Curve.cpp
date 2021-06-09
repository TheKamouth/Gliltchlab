#include "Curve.h"

#include <QDebug>

Curve::Curve() :
    _duration( 6.66f),
    _minValue( -1.0f),
    _maxValue( 1.0f)
{
    _controlPoints.insert(0.0f, ControlPoint());
    _controlPoints.insert(_duration, ControlPoint());
}

float Curve::Evaluate(float time)
{
    if( time < 0.0f || qFuzzyCompare(time, 0.0f))
    {
        ControlPoint & firstControlPoint = _controlPoints.first();
        return firstControlPoint.Value();
    }
    else if( time > _duration || qFuzzyCompare(time, _duration))
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
        }

        ControlPoint & previousControlPoint = previousControlPointIterator.value();
        ControlPoint & nextControlPoint = nextControlPointIterator.value();

        float previousValue = previousControlPoint.Value();
        float nextValue = previousControlPoint.Value();

        float previousKeyTime = previousControlPointIterator.key();
        float nextKeyTime = previousControlPointIterator.key();

        InterpolationMode previousValueMode = previousControlPoint.Mode();

        float value = 0.0f;

        switch(previousValueMode)
        {
            case Constant:
                value = previousValue;
                break;

            case Linear:
            {
                float timeInterval = nextKeyTime - previousKeyTime;
                float alpha = (time - previousKeyTime) / timeInterval;
                value = alpha * previousValue + ( 1.0f - alpha ) * nextValue;
                break;
            }

            default:
                qDebug() << "This interpolationMode is not handled";
                break;
        }

        return value;
    }
}
