#ifndef TIMELINEMANAGER_H
#define TIMELINEMANAGER_H

#include "Timeline/TimelineWidget.h"
#include "Timeline/TimelineView.h"
#include "TimeBarGraphicsItem.h"

#include <QObject>

class TimelineManager : public QObject
{
    Q_OBJECT;

public:
    TimelineManager();
    TimelineManager(TimelineManager const &)               = delete;
    void operator=(TimelineManager const &)  = delete;

    static TimelineManager & Instance()
    {
        static TimelineManager instance;
        return instance;
    }

    QDockWidget * TimelineDockWidget();
    Track * GetTrack(int index);
    void SetTime(float time);
    std::pair<float,float> GetSectionBeginAndEndTimes();

    void EmitAddTrackNode(Track * track)
    {
        emit AddTrackNode(track);
    }

signals:
    void AddTrackNode(Track * track);

private :
    TimelineWidget * _timelineDockWidget;
    TimelineView _timelineView;
    Timeline * _timeline;

    TimeBarGraphicsItem _timeBarGraphicsItem;
};

#endif // TIMELINEMANAGER_H
