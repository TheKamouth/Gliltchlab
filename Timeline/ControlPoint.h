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
    ControlPoint(int timeMs, float value);

    void SetValue(float value);
    float Value();
    float Time();
    InterpolationMode Mode();

private:
    float _value;
    int _timeMs;
    InterpolationMode _interpolationMode;
};

#endif // CONTROLPOINT_H
