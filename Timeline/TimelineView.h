#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include "Timeline.h"
#include "TimelineScene.h"
#include "TrackGraphicsItem.h"
#include "TimeBarGraphicsItem.h"
#include "ControlPointgraphicsItem.h"

#include <QGraphicsView>
#include <QMenu>

class TimelineView : public QGraphicsView
{
public:
    TimelineView();

    void SetTimeline(Timeline * timeline);
    void InitTimeline();

    void AddTrackGraphicsItem(int trackIndex, Track * track);
    void AddTrackNode(FlowDataType type);

    void OnTimeChanged(int timeMs);
    QPointF GetViewPosition();
    std::pair<float,float> GetSelectionBeginAndEndTimes();

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    Timeline * _timeline;
    TimelineScene _timelineScene;

    TimeBarGraphicsItem * _timeBarGraphicsItem;
    QList<TrackGraphicsItem*> _tracksGraphicsItems;

    ControlPointgraphicsItem * _selectedControlPointGraphicsItem;

    QMenu _contextMenu;

    float _currentScale;
    float _leftViewTime;
    float _currentVisibleTimeIntervalMs;
    float _currentPosition;

    bool _isMiddleMouseButtonPressed;
    bool _isLeftMouseButtonPressed;
    bool _leftClickedOnNodeItem;

    QPointF _dragStartPosition;
    QPointF _currentPositionPointF;

    void OnHorizontalScrollBarValueChanged(float value);
    void OnVerticalScrollBarValueChanged(float value);
    void UpdateScrollBars();
    QSize TimelineSize();
};

#endif // TIMELINEVIEW_H
