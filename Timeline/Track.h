#ifndef TRACK_H
#define TRACK_H

#include "Curve.h"
#include "FlowGraph/FlowData.h"

#include <QString>

class Track
{

public:
    Track(QString name, FlowDataType type);

    FlowData * GetDataAtTime(float time);
    FlowData * GetDataAtCurrentTime();
    QString Name();
    FlowDataType Type();
    Curve * GetCurve();

private:
    QString _name;

    // Should it be somewhere else ?
    float _time;

    // only handling curve/float track for now
    Curve _curve;

    FlowData _data;
};

#endif // TRACK_H
