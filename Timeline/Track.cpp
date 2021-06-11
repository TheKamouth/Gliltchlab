#include "Track.h"

#include "TimeManager.h"

Track::Track(QString name, FlowDataType type) :
    _name(name),
    _time(0.0f)
{
    _data.SetType(type);
}

FlowData * Track::GetDataAtTime(float time)
{
    float curveValueAtTime = _curve.Evaluate(time);

    // this should be generic
    // implying Track should be templated ?
    _data.SetFloat(curveValueAtTime);

    return &_data;
}

FlowData * Track::GetDataAtCurrentTime()
{
    float currentTime = TimeManager::Instance().Time();
    float curveValueAtTime = _curve.Evaluate(currentTime);

    // this should be generic
    // implying Track should be templated ?
    _data.SetFloat(curveValueAtTime);

    return &_data;
}

QString Track::Name()
{
    return _name;
}

FlowDataType Track::Type()
{
    return GetDataAtTime(0.0f)->GetType();
}

Curve * Track::GetCurve()
{
    return &_curve;
}
