#include "Timeline.h"

#include "TimelineConstants.h"

Timeline::Timeline() :
    _tracks(QList<Track *>()),
    _tempo(120)
{
    for( int i = 0 ; i< MAX_TRACK_COUNT; i++)
    {
        QString name = "Track" + QString::number(i);
        AddTrack(name, Unassigned);
    }
}

Track * Timeline::AddTrack(QString trackName, FlowDataType type)
{
    Track * track = new Track(trackName, type);
    _tracks.append(track);

    // needed ?
    //emit TrackAdded(track);

    return track;
}

const QList<Track *> & Timeline::Tracks()
{
    return _tracks;
}

int Timeline::TrackCount()
{
    return _tracks.count();
}

int Timeline::LastTimelineEventTime()
{
    // TODO
    return TIMELINE_TRACK_MAX_DURATION_MS;
}

float Timeline::GetTempo()
{
    return _tempo;
}
