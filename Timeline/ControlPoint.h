#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

enum InterpolationMode
{
    Constant,
    LinearInterp,
    CubicInterp,
    InterpolationModeCount
};

class ControlPoint
{
public:
    ControlPoint();

    void SetValue(float value);
    float Value();
    InterpolationMode Mode();

private:
    float _value;
    InterpolationMode _interpolationMode;
};

#endif // CONTROLPOINT_H
