#include "TimelineView.h"

#include "TimelineConstants.h"
#include "TrackGraphicsItem.h"
#include "CurveGraphicsItem.h"
#include "TimeBarGraphicsItem.h"

#include "TimelineManager.h"
#include "TimeManager.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

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

    _currentPositionPointF = QPointF(0.0f, 0.0f);
    horizontalScrollBar()->setValue( _currentPositionPointF.x());
    verticalScrollBar()->setValue( _currentPositionPointF.y());
}

void TimelineView::SetTimeline(Timeline * timeline)
{
    _timeline = timeline;

    InitTimeline();
}

void TimelineView::InitTimeline()
{
    const QList<Track*> & tracks = _timeline->Tracks();
    int trackCount = tracks.count();

    for(int i = 0 ; i < trackCount ; i++)
    {
        Track * track = tracks.at(i);
        AddTrackGraphicsItem(i, track);
    }
}

void TimelineView::AddTrackGraphicsItem(int trackIndex, Track * track)
{
    TrackGraphicsItem * trackItem = new TrackGraphicsItem( trackIndex, track);
    _timelineScene.addItem(trackItem);

    // TODO : add curve graphics item
    CurveGraphicsItem * curveGraphicsItem = new CurveGraphicsItem( track->GetCurve(), trackItem);
    _timelineScene.addItem(curveGraphicsItem);
}

void TimelineView::mousePressEvent(QMouseEvent * event)
{
    QPointF sceneClickPos = mapToScene(event->pos());
    QGraphicsItem * clickedItem = _timelineScene.itemAt(sceneClickPos, transform());
    TrackGraphicsItem * trackItem = dynamic_cast<TrackGraphicsItem *>(clickedItem);
    CurveGraphicsItem * curveItem = dynamic_cast<CurveGraphicsItem *>(clickedItem);
    TimeBarGraphicsItem * timeBarItem = dynamic_cast<TimeBarGraphicsItem *>(clickedItem);

    if (event->buttons() == Qt::MiddleButton )
    {
        _isMiddleMouseButtonPressed = true;
        _dragStartPosition = event->pos();

        if(timeBarItem != nullptr)
        {
            qDebug() << "timeBarItem clicked at " << event->pos();
            TimeManager::Instance().SetTime(sceneClickPos.x() - TIMELINE_TRACK_INFO_WIDTH);
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        _contextMenu.clear();

        if( curveItem != nullptr)
        {
            qDebug() << "curveItem clicked at " << event->pos();

            _contextMenu.addAction( "Add point",
                                    [=]() -> void {
                curveItem->AddPoint(sceneClickPos);
                curveItem->update();
            });
        }
        else if(trackItem != nullptr)
        {
            qDebug() << "trackItem clicked at " << event->pos();
        }
        else if(timeBarItem != nullptr)
        {
            qDebug() << "timeBarItem clicked at " << event->pos();
            timeBarItem->SetSelectionTime(sceneClickPos.x() - TIMELINE_TRACK_INFO_WIDTH);
        }
        else
        {
            _contextMenu.addAction( "Add float track",
                                    [=]() -> void {

                int trackIndex = _timeline->TrackCount();
                QString trackName = "Track" + QString::number(trackIndex);
                Track * track = _timeline->AddTrack(trackName, Float);

                TimelineManager::Instance().EmitAddTrackNode(track);

                // This should happen after TrackNode is created
                AddTrackGraphicsItem(trackIndex, track);
            });
        }

        _contextMenu.move(event->globalPos());
        _contextMenu.show();
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
