#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

enum InterpolationMode
{
    Constant,
    Linear,
    InterpolationModeCount
};

class ControlPoint
{
public:
    ControlPoint();
    float Value();
    InterpolationMode Mode();

private:
    float _value;
    InterpolationMode _interpolationMode;
};

#endif // CONTROLPOINT_H
