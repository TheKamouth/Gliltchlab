#ifndef ITRACKDATASEQUENCE_H
#define ITRACKDATASEQUENCE_H

#include "ClipBase.h"

#include <QMap>

// ITrackDataSeuqences compose a track
// ITrackDataSequence exposes a common interface for evaluating track at a time...
class ITrackDataSequence
{
public:
    QMap<int,ClipBase*> DataClips(){ return _dataClips; }

private:
    // ITrackData at a specific time
    QMap<int,ClipBase*> _dataClips;
};

#endif // ITRACKDATASEQUENCE_H
