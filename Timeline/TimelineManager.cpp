#include "TimelineManager.h"

#include <QDebug>

TimelineManager::TimelineManager() :
    _timelineDockWidget(new TimelineWidget()),
    _timeline(new Timeline()),
    _timeBarGraphicsItem(_timeline)

{
    _timelineView.SetTimeline(_timeline);
    _timelineDockWidget->SetTimeline(&_timelineView);
    _timelineView.scene()->addItem( &_timeBarGraphicsItem);
}

QDockWidget * TimelineManager::TimelineDockWidget()
{
    return _timelineDockWidget;
}

Track *TimelineManager::GetTrack(int index)
{
    QList<Track*> tracks = _timeline->Tracks();

    if( index < 0 || index >= tracks.count())
    {
        qDebug() << "Track index out of range";
    }

    return tracks.at(index);
}

void TimelineManager::SetTime(float time)
{
    _timeBarGraphicsItem.SetTime(time);
}

std::pair<float, float> TimelineManager::GetSectionBeginAndEndTimes()
{
    return _timeBarGraphicsItem.GetSelectionBeginAndEndTimes();
}
