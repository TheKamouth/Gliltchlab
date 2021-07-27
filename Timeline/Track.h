#ifndef TRACK_H
#define TRACK_H

#include "Curve.h"
#include "FlowGraph/FlowData.h"

#include "ITrackDataSequence.h"

#include <QString>

class ITrack
{

};

//template<class T>
class Track : public ITrack
{

public:
    Track(QString name, FlowDataType type);

    void SetType(FlowDataType);
    FlowData * GetDataAtTime(float time);
    FlowData * GetDataAtCurrentTime();
    QString Name();
    FlowDataType Type();

    Curve * GetCurve();
    void AddClip(ClipBase * clip);

private:
    QString _name;

    // Should it be somewhere else ?
    float _time;

    // only handling curve/float track for now
    //Curve _curve;

    FlowData _data;
    //T _flowData;

    // This non-pure type is known at runtime
    QMap<int,ClipBase*> _dataClips;
};

#endif // TRACK_H
