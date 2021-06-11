#include "ControlPoint.h"

ControlPoint::ControlPoint() :
    _value(0.0f),
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

InterpolationMode ControlPoint::Mode()
{
    return _interpolationMode;
}
