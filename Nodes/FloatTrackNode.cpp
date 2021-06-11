#include "FloatTrackNode.h"

#include "Timeline/TimelineManager.h"

FloatTrackNode::FloatTrackNode() :
    _track(nullptr)
{
    // Temporary : always associating track 0 with node
    Track * track0 = TimelineManager::Instance().GetTrack(0);
    SetAssociatedTrack(track0);
}

// How should this method be called ?
void FloatTrackNode::SetAssociatedTrack(Track * track)
{
    _track = track;
}

bool FloatTrackNode::BeforeProcessing()
{
    return true;
}

bool FloatTrackNode::ProcessInternal()
{
    // Doing this once suffice as pin points to track data
    // A signal telling that output as changed could be useful

    // Read associated track data
    QString associatedTrackName = _track->Name();
    FlowData * data =_track->GetDataAtCurrentTime();

    // Write it to output pin
    IDataPin * outputPin = GetDataPinAt<1>();
    outputPin->SetData(data);

    qDebug() << QString::number(data->GetFloat());

    return true;
}
