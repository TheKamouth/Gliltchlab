#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include "Timeline.h"
#include "TimelineScene.h"

#include "QGraphicsView"

class TimelineView : public QGraphicsView
{
public:
    TimelineView();
    void SetTimeline(Timeline * timeline);

    void AddTrack();

protected:
    virtual void mousePressEvent(QMouseEvent * event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent * event) override;
    virtual void wheelEvent(QWheelEvent * event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    TimelineScene _timelineScene;
    Timeline * _timeline;

    float _currentScale;
    float _leftViewPos;
    float _currentVisibleTimeIntervalMs;
    float _currentPosition;

    bool _isMiddleMouseButtonPressed;
    bool _isLeftMouseButtonPressed;
    bool _leftClickedOnNodeItem;

    QPointF _dragStartPosition;
    QPointF _currentPositionPointF;

    void OnHorizontalScrollBarValueChanged(float value);
    void OnVerticalScrollBarValueChanged(float value);
};

#endif // TIMELINEVIEW_H
