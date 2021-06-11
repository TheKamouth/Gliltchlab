#include "Timeline.h"

Timeline::Timeline() :
    _tracks(QList<Track *>())
{
    /*
    for( int i = 0 ; i< MAX_TRACK_COUNT; i++)
    {
        QString name = "Track" + QString::number(i);
        AddTrack(name, Float);
    }*/
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
