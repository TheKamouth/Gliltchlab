#include "Track.h"

#include "Curve.h"

#include "TimeManager.h"

Track::Track(QString name, FlowDataType type = Unassigned) :
    _name(name),
    _time(0.0f)
{
    _data.SetType(type);

    // This was somehow needed... commenting it might break something
    /*
    switch(_data.GetType())
    {
        case Float:
            _dataClips.insert(0, new Curve());
            break;
        case Int:
            _dataClips.insert(0, new Curve());
            break;
        case Unassigned:
            break;
        default:
            break;
    }
    */
}

void Track::SetType(FlowDataType type)
{
    _data.SetType(type);
}

FlowData * Track::GetDataAtTime(float time)
{
    // Get first active ITrackDataSequence at time
    // Can be Curve sequence, ImageSequence file sequence, Video file sequence, Audio file sequence

    float curveValueAtTime = 0.5;//_curve.Evaluate(time);


    // this should be generic
    // implying Track should be templated ?

    switch(_data.GetType())
    {
        case Float:
            _data.SetFloat(curveValueAtTime);
            break;
        case Int:
            _data.SetInt((int)curveValueAtTime);
            break;
        default:
            break;
    }

    return &_data;
}

FlowData * Track::GetDataAtCurrentTime()
{
    float currentTime = TimeManager::Instance().Time();
    float curveValueAtTime =  0.5;//_curve.Evaluate(time);

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
    return nullptr;
    //return &_curve;
}

void Track::AddClip(ClipBase * clip)
{
    // drop position should be used to set clip position
    _dataClips.insert(0.0f, clip);
}
