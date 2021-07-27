#ifndef MATHUTILS_H
#define MATHUTILS_H

#include "math.h"

float Clamp(float value, float min, float max)
{
    return std::max( min, std::min( value, max) );
}

#endif // MATHUTILS_H
