#include "ControlPoint.h"

ControlPoint::ControlPoint(int timeMs, float value) :
    _value(value),
    _timeMs(timeMs),
    _interpolationMode(LinearInterp)
{

}

void ControlPoint::SetValue(float value)
{
    _value = value;
}

float ControlPoint::Value()
{
    return _value;
}

float ControlPoint::Time()
{
    return _timeMs;
}

InterpolationMode ControlPoint::Mode()
{
    return _interpolationMode;
}
