#include "ControlPoint.h"

ControlPoint::ControlPoint() :
    _value(0.0f),
    _interpolationMode(Constant)
{

}

float ControlPoint::Value()
{
    return _value;
}

InterpolationMode ControlPoint::Mode()
{
    return _interpolationMode;
}
