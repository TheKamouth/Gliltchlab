#ifndef TIMELINE_H
#define TIMELINE_H

#include "Track.h"

#include <QList>
#include <QObject>

const int MAX_TRACK_COUNT = 16;

class Timeline : public QObject
{
    Q_OBJECT;

public:
    Timeline();

    Track * AddTrack(QString trackName, FlowDataType type);
    const QList<Track *> & Tracks();
    int TrackCount();

    // This is used when no timeline section is selected
    int LastTimelineEventTime();
    float GetTempo();

// useless ?
//signals:
//    void TrackAdded(Track * track);

private :
    // Tracks are identified by
    QList<Track*> _tracks;
    float _tempo;
};

#endif // TIMELINE_H
