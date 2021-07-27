#include "TimelineManager.h"

#include "TimeManager.h"

#include <QDebug>
#include <QKeyEvent>

TimelineManager::TimelineManager() :
    _timelineDockWidget(new TimelineWidget()),
    _timeline(new Timeline())
{
    _timelineView.SetTimeline(_timeline);
    _timelineDockWidget->SetTimeline(&_timelineView);
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
    //_timeBarGraphicsItem.SetTime(time);
    _timelineView.OnTimeChanged(time);
}

std::pair<float, float> TimelineManager::GetSectionBeginAndEndTimes()
{
    return _timelineView.GetSelectionBeginAndEndTimes();
}

QPointF TimelineManager::GetViewPosition()
{
    return _timelineView.GetViewPosition();
}


