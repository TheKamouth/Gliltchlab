#include "TimelineView.h"

#include "TimelineConstants.h"
#include "TrackGraphicsItem.h"

#include <QMouseEvent>
#include <QScrollBar>

TimelineView::TimelineView() :
    _currentScale(1.0f),
    _leftViewPos(0.0f),
    _currentVisibleTimeIntervalMs(10000.0f),
    _currentPosition(0.0f),
    _isMiddleMouseButtonPressed(false)
{
    setScene(&_timelineScene);

    QObject::connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &TimelineView::OnHorizontalScrollBarValueChanged);
    QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TimelineView::OnVerticalScrollBarValueChanged);

    QRect sceneRect = QRect(0, 0, _currentVisibleTimeIntervalMs,  8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT);
    setSceneRect(sceneRect);

    //float yScaleFactor = height() / 8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;
    //scale(1.0f, yScaleFactor);

    _currentPositionPointF = QPointF(horizontalScrollBar()->value(), verticalScrollBar()->value());
    horizontalScrollBar()->setValue( _currentPositionPointF.x());
    verticalScrollBar()->setValue( _currentPositionPointF.y());

    // TMP : Test
    AddTrack();
}

void TimelineView::SetTimeline(Timeline * timeline)
{
    _timeline = timeline;
}

void TimelineView::AddTrack()
{
    TrackGraphicsItem * trackItem = new TrackGraphicsItem();
    _timelineScene.addItem(trackItem);
}

void TimelineView::mousePressEvent(QMouseEvent * event)
{
    QPointF sceneClickPos = mapToScene(event->pos());
    QGraphicsItem * clickedItem = _timelineScene.itemAt(sceneClickPos, transform());

    if (event->buttons() == Qt::MiddleButton )
    {
        _isMiddleMouseButtonPressed = true;
        _dragStartPosition = event->pos();
    }
}

void TimelineView::mouseMoveEvent(QMouseEvent * event)
{
    if(_isMiddleMouseButtonPressed)
    {
        _currentPositionPointF.setX(_currentPositionPointF.x() - _dragStartPosition.x());
        //horizontalScrollBar()->setValue(_currentPosition - (event->x() - _dragStartPosition.x()));

        horizontalScrollBar()->setValue( _currentPositionPointF.x());
        verticalScrollBar()->setValue( _currentPositionPointF.y());

        _dragStartPosition = QPointF(event->x(), event->y());
    }
}

void TimelineView::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::MiddleButton)
    {
        _isMiddleMouseButtonPressed = false;
        _dragStartPosition = QPointF( 0.0f, 0.0f);
    }
}

void TimelineView::wheelEvent(QWheelEvent *event)
{
    //qDebug() << __FUNCTION__ << _currentScale;

    QPoint angleDelta = event->angleDelta();

    if(angleDelta.y() > 0)
    {
        _currentScale = 1.1f;
        _currentVisibleTimeIntervalMs += 10.0f;
        _currentPositionPointF.setX( _currentPositionPointF.x() + 10.f);
    }
    else
    {
        _currentScale = 0.9f;
        _currentVisibleTimeIntervalMs -= 10.0f;
        _currentPositionPointF.setX( _currentPositionPointF.x() - 10.f);
    }

    //scale(_currentScale, 1.0);

    QRect sceneRect = QRect(0, 0, _currentVisibleTimeIntervalMs, 8 * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT);
    setSceneRect(sceneRect);

    horizontalScrollBar()->setValue( _currentPositionPointF.x());
    verticalScrollBar()->setValue( _currentPositionPointF.y());

    //float yScaleFactor = height() / 8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;
    //scale(1.0f, yScaleFactor);
}

void TimelineView::resizeEvent(QResizeEvent * event)
{
    QRect sceneRect = QRect(0, 0, _currentVisibleTimeIntervalMs, height());
    setSceneRect(sceneRect);

    //float yScaleFactor = height() / 8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;
    //scale(1.0f, yScaleFactor);

    horizontalScrollBar()->setValue( _currentPositionPointF.x());
    verticalScrollBar()->setValue( _currentPositionPointF.y());
}

void TimelineView::OnHorizontalScrollBarValueChanged(float value)
{
    _currentPositionPointF.setX(value);
}

void TimelineView::OnVerticalScrollBarValueChanged(float value)
{
    _currentPositionPointF.setY(value);
}
