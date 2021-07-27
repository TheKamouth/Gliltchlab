#include "TimelineView.h"

#include "TimelineConstants.h"
#include "TrackGraphicsItem.h"
#include "CurveGraphicsItem.h"
#include "TimeBarGraphicsItem.h"
#include "ControlPointgraphicsItem.h"

#include "TimelineManager.h"
#include "TimeManager.h"

#include "MathUtils.h"

#include <QMouseEvent>
#include <QScrollBar>
#include <QDebug>

#include "algorithm"

TimelineView::TimelineView() :
    _timeline(nullptr),
    _timeBarGraphicsItem(nullptr),
    _selectedControlPointGraphicsItem(nullptr),
    _currentScale(1.0f),
    _leftViewTime(0.0f),
    _currentVisibleTimeIntervalMs(10000.0f),
    _currentPosition(0.0f),
    _isMiddleMouseButtonPressed(false),
    _dragStartPosition(0.0f,0.0f),
    _currentPositionPointF(0.0f,0.0f)
{
    setScene(&_timelineScene);

    QObject::connect(&TimeManager::Instance(), &TimeManager::TimeChanged, this, &TimelineView::OnTimeChanged);

    QObject::connect(horizontalScrollBar(), &QScrollBar::valueChanged, this, &TimelineView::OnHorizontalScrollBarValueChanged);
    QObject::connect(verticalScrollBar(), &QScrollBar::valueChanged, this, &TimelineView::OnVerticalScrollBarValueChanged);

    UpdateScrollBars();

    //float yScaleFactor = height() / 8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;
    //scale(1.0f, yScaleFactor);
}

void TimelineView::SetTimeline(Timeline * timeline)
{
    _timeline = timeline;

    InitTimeline();

    _timeBarGraphicsItem = new TimeBarGraphicsItem(_timeline);
    scene()->addItem( _timeBarGraphicsItem);
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
    _tracksGraphicsItems.append(trackItem);

    // This should be done for curve track only
    /*
    CurveGraphicsItem * curveGraphicsItem = new CurveGraphicsItem( track->GetCurve(), trackItem);
    _timelineScene.addItem(curveGraphicsItem);

    trackItem->SetCurveGraphicsItem(curveGraphicsItem);

    // Add ControlPointGraphicsItem
    Curve * curve = track->GetCurve();
    QMap<float, ControlPoint> * controlPoints = curve->ControlPoints();

    QMap< float,ControlPoint >::iterator controlPointIterator = controlPoints->begin();
    QMap< float,ControlPoint >::iterator endControlPointIterator = controlPoints->end();

    while( controlPointIterator != endControlPointIterator)
    {
        ControlPoint & controlPoint = controlPointIterator.value();
        ControlPointgraphicsItem * controlPointGraphicsItem = new ControlPointgraphicsItem(&controlPoint, curveGraphicsItem);

        _timelineScene.addItem(controlPointGraphicsItem);

        controlPointIterator++;
    }
    */
}

void TimelineView::AddTrackNode(FlowDataType type)
{
    int trackIndex = _timeline->TrackCount();
    QString trackName = "Track" + QString::number(trackIndex);
    Track * track = _timeline->AddTrack(trackName, type);

    TimelineManager::Instance().EmitAddTrackNode(track);

    // This should happen after TrackNode is created
    AddTrackGraphicsItem(trackIndex, track);

    UpdateScrollBars();
}

void TimelineView::OnTimeChanged(int timeMs)
{
    _timeBarGraphicsItem->update();

    for(int i = 0 ; i < _tracksGraphicsItems.count() ; i++)
    {
        TrackGraphicsItem * trackGraphicsItem = _tracksGraphicsItems[i];
        trackGraphicsItem->update();
    }

    // Update ControlPointGraphicsItem
}

QPointF TimelineView::GetViewPosition()
{
    return _currentPositionPointF;
}

std::pair<float, float> TimelineView::GetSelectionBeginAndEndTimes()
{
    return _timeBarGraphicsItem->GetSelectionBeginAndEndTimes();
}

void TimelineView::mousePressEvent(QMouseEvent * event)
{
    QPointF sceneClickPos = mapToScene(event->pos());
    QGraphicsItem * clickedItem = _timelineScene.itemAt(sceneClickPos, transform());
    TrackGraphicsItem * trackItem = dynamic_cast<TrackGraphicsItem *>(clickedItem);
    CurveGraphicsItem * curveItem = dynamic_cast<CurveGraphicsItem *>(clickedItem);
    TimeBarGraphicsItem * timeBarItem = dynamic_cast<TimeBarGraphicsItem *>(clickedItem);
    ControlPointgraphicsItem * controlPointItem = dynamic_cast<ControlPointgraphicsItem *>(clickedItem);

    if(trackItem != nullptr && controlPointItem == nullptr)
    {
        controlPointItem = trackItem->GetClosestControlPoint(sceneClickPos);
    }

    if (event->buttons() == Qt::LeftButton )
    {
        ControlPointgraphicsItem * controlPointItem = dynamic_cast<ControlPointgraphicsItem *>(clickedItem);

        if(controlPointItem != nullptr)
        {
            qDebug() << "controlPointItem clicked at " << event->pos();

            if(_selectedControlPointGraphicsItem != nullptr)
            {
                _selectedControlPointGraphicsItem->SetSelected(false);
            }

            controlPointItem->SetSelected(true);
            _selectedControlPointGraphicsItem = controlPointItem;
        }
        else if(trackItem != nullptr)
        {
            if(controlPointItem != nullptr)
            {
                _selectedControlPointGraphicsItem = controlPointItem;
                controlPointItem->SetSelected(true);
            }
            else if(_selectedControlPointGraphicsItem != nullptr)
            {
                _selectedControlPointGraphicsItem->SetSelected(false);
                _selectedControlPointGraphicsItem = nullptr;
            }
        }
        else
        {
            if(_selectedControlPointGraphicsItem != nullptr)
            {
                _selectedControlPointGraphicsItem->SetSelected(false);
                _selectedControlPointGraphicsItem = nullptr;
            }
        }
    }
    else if (event->buttons() == Qt::MiddleButton )
    {
        if(_selectedControlPointGraphicsItem != nullptr)
        {
            _selectedControlPointGraphicsItem->SetSelected(false);
            _selectedControlPointGraphicsItem = nullptr;
        }

        _isMiddleMouseButtonPressed = true;
        _dragStartPosition = sceneClickPos;

        if(timeBarItem != nullptr)
        {
            qDebug() << "timeBarItem clicked at " << event->pos();
            TimeManager::Instance().SetTime(sceneClickPos.x() - TIMELINE_TRACK_INFO_WIDTH);
        }
    }
    else if(event->button() == Qt::RightButton)
    {
        _contextMenu.clear();

        if(controlPointItem != nullptr)
        {
            _selectedControlPointGraphicsItem = controlPointItem;
            controlPointItem->SetSelected(true);
        }

        if( curveItem != nullptr)
        {
            qDebug() << "curveItem clicked at " << event->pos();

            _contextMenu.addAction( "Add point",
                                    [=]() -> void {
                ControlPoint * controlPoint = curveItem->AddPoint(sceneClickPos);
                ControlPointgraphicsItem * controlPointGraphicsItem = new ControlPointgraphicsItem(controlPoint, curveItem);

                _timelineScene.addItem(controlPointGraphicsItem);
                curveItem->update();
            });
        }
        else if(trackItem != nullptr)
        {
            qDebug() << "trackItem clicked at " << event->pos() ;

            if(controlPointItem != nullptr)
            {
                _selectedControlPointGraphicsItem = controlPointItem;
                controlPointItem->SetSelected(true);

                _contextMenu.addAction( "Interpolation mode:",
                                        [=]() -> void {qDebug()<< "TODO";});
            }

            _contextMenu.addAction( "Add point",
                                    [=]() -> void {
                CurveGraphicsItem * curveGraphicsItem = trackItem->GetCurveGraphicsItem();
                ControlPoint * controlPoint = curveGraphicsItem->AddPoint(sceneClickPos);

                if (curveGraphicsItem != nullptr)
                {
                    ControlPointgraphicsItem * controlPointGraphicsItem = new ControlPointgraphicsItem(controlPoint, curveGraphicsItem);
                    curveGraphicsItem->GetControlPointsGraphicItem()->append(controlPointGraphicsItem);

                    _timelineScene.addItem(controlPointGraphicsItem);
                    curveGraphicsItem->update();
                }

            });
        }
        else if(timeBarItem != nullptr)
        {
            qDebug() << "timeBarItem clicked at " << event->pos();
            timeBarItem->SetSelectionTime(sceneClickPos.x() - TIMELINE_TRACK_INFO_WIDTH);
        }
        else
        {
            _contextMenu.addAction( "Add float track",[=]() -> void { AddTrackNode(Float);} );
            _contextMenu.addAction( "Add int track",[=]() -> void { AddTrackNode(Int);} );
            _contextMenu.addAction( "Add image track",[=]() -> void { AddTrackNode(Image);} );
            _contextMenu.addAction( "Add audio track",[=]() -> void { /*AddTrackNode(Audio);*/ });
            _contextMenu.addAction( "Add video track",[=]() -> void { /*AddTrackNode(Video);*/ });
        }

        _contextMenu.move(event->globalPos());
        _contextMenu.show();
    }
}

void TimelineView::mouseMoveEvent(QMouseEvent * event)
{
    QPointF sceneMousePos = mapToScene(event->pos());

    if(_isMiddleMouseButtonPressed)
    {
        //qDebug() << _currentPositionPointF << " " << event->pos() << " " << _dragStartPosition < ;

        _currentPositionPointF -= sceneMousePos - _dragStartPosition;
        _currentPositionPointF.setX( Clamp(_currentPositionPointF.x(), 0.0f, _timeline->LastTimelineEventTime() - _currentVisibleTimeIntervalMs));
        _currentPositionPointF.setY( Clamp(_currentPositionPointF.y(), 0.0f, TimelineSize().height() - height() + horizontalScrollBar()->height()));

        horizontalScrollBar()->setValue(_currentPositionPointF.x());
        verticalScrollBar()->setValue(_currentPositionPointF.y());

        _dragStartPosition = sceneMousePos;

        // to update every QGraphicsItem no parameter needed
        OnTimeChanged(0.0f);
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
    QPoint angleDelta = event->angleDelta();
    float delta = angleDelta.y();

    _currentPositionPointF.setY( _currentPositionPointF.y() - delta);
    verticalScrollBar()->setValue( _currentPositionPointF.y());

    UpdateScrollBars();
}

void TimelineView::resizeEvent(QResizeEvent * event)
{
    //float yScaleFactor = height() / 8.0f * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;
    //scale(1.0f, yScaleFactor);

    _currentVisibleTimeIntervalMs = width() - TIMELINE_TRACK_INFO_WIDTH;

    UpdateScrollBars();
}

void TimelineView::OnHorizontalScrollBarValueChanged(float value)
{
    _currentPositionPointF.setX(value);
    UpdateScrollBars();

    // to update every QGraphicsItem no parameter needed
    OnTimeChanged(0.0f);

    update();
}

void TimelineView::OnVerticalScrollBarValueChanged(float value)
{
    _currentPositionPointF.setY(value);
    UpdateScrollBars();

    // no parameter needed
    OnTimeChanged(0.0f);

    update();
}

void TimelineView::UpdateScrollBars()
{
    //return;
    //qDebug() << _currentPositionPointF;

    if(_timeline == nullptr)
        return;

    /*
    QRect sceneRect = QRect(0.0f,
                            0.0f,
                            timelineSize.width(),
                            timelineSize.height()
                            );

    setSceneRect(sceneRect);
    */

    QSize timelineSize = TimelineSize();

    horizontalScrollBar()->setMinimum(0.0f);
    horizontalScrollBar()->setMaximum(_timeline->LastTimelineEventTime() - _currentVisibleTimeIntervalMs);

    verticalScrollBar()->setMinimum(0.0f);
    verticalScrollBar()->setMaximum(timelineSize.height() - height() + horizontalScrollBar()->height());
}

QSize TimelineView::TimelineSize()
{
    return QSize(TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                 _timeline->TrackCount() * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT);
}
