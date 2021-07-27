#include "ControlPointgraphicsItem.h"

#include "CurveGraphicsItem.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"
#include "TimelineManager.h"

#include <QPainter>

ControlPointgraphicsItem::ControlPointgraphicsItem(ControlPoint * controlPoint, CurveGraphicsItem * curveGraphicsItem) :
    _controlPoint(controlPoint),
    _curveGraphicsItem(curveGraphicsItem),
    _isSelected(false)
{

}

QRectF ControlPointgraphicsItem::boundingRect() const
{
    int timelineTrackIndex = 0; //_trackGraphicsItem->TimelineTrackIndex();

    Curve * curve = _curveGraphicsItem->GetCurve();

    QPointF curveViewPosition = QPointF( TIMELINE_TRACK_INFO_WIDTH ,
                                         0);

    float value = _controlPoint->Value();
    float time = _controlPoint->Time();

    // TOODO : convert value to scene coordinates
    float lowerBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT;
    float upperBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;

    // alpha [0,1]
    float alpha = std::clamp( ( value - curve->LowerBound() ) / curve->Range(), 0.0f, 1.0f);
    float y = alpha * upperBoundY + ( 1.0f - alpha ) * lowerBoundY;

    QPointF controlPointPosition = QPointF(time, y);

    QRect controlPointRect = QRect(curveViewPosition.x() + controlPointPosition.x() - TIMELINE_CONTROL_POINT_SELECTED_RADIUS,
                                 controlPointPosition.y() - TIMELINE_CONTROL_POINT_SELECTED_RADIUS,
                                 TIMELINE_CONTROL_POINT_SELECTED_RADIUS * 2.0f,
                                 TIMELINE_CONTROL_POINT_SELECTED_RADIUS * 2.0f);

    return controlPointRect;
}

void ControlPointgraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int timelineTrackIndex = 0; //_trackGraphicsItem->TimelineTrackIndex();

    Curve * curve = _curveGraphicsItem->GetCurve();

    float value = _controlPoint->Value();
    float time = _controlPoint->Time();

    // TOODO : convert value to scene coordinates
    float lowerBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT;
    float upperBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;

    // alpha [0,1]
    float alpha = std::clamp( ( value - curve->LowerBound() ) / curve->Range(), 0.0f, 1.0f);
    float y = alpha * upperBoundY + ( 1.0f - alpha ) * lowerBoundY;

    QPointF controlPointPosition = QPointF(time, y);

    // dbg
    QRect controlPointRect = QRect(TIMELINE_TRACK_INFO_WIDTH + controlPointPosition.x() - TIMELINE_CONTROL_POINT_SELECTED_RADIUS,
                                 controlPointPosition.y() - TIMELINE_CONTROL_POINT_SELECTED_RADIUS,
                                 TIMELINE_CONTROL_POINT_SELECTED_RADIUS * 2.0f,
                                 TIMELINE_CONTROL_POINT_SELECTED_RADIUS * 2.0f);
    painter->drawRect(controlPointRect);

    /*QRect controlPointRect = QRect(controlPointPosition.x() - TIMELINE_CONTROL_POINT_RADIUS / 2.0f,
                                 controlPointPosition.y() - TIMELINE_CONTROL_POINT_RADIUS / 2.0f,
                                 8,
                                 8);*/

    QPointF curveViewPosition = QPointF( TIMELINE_TRACK_INFO_WIDTH ,
                                         0);

    QBrush brush = painter->brush();
    QPen pen = painter->pen();

    // Selection outline
    if (_isSelected)
    {
        pen.setColor(RED);
        brush.setColor(RED);
        brush.setStyle(Qt::SolidPattern);

        painter->setPen(pen);
        painter->setBrush(brush);

        painter->drawEllipse(controlPointPosition + curveViewPosition,
                             TIMELINE_CONTROL_POINT_SELECTED_RADIUS, TIMELINE_CONTROL_POINT_SELECTED_RADIUS);
    }

    // Center
    pen.setColor(LIGHT_GREY);
    brush.setColor(LIGHT_GREY);
    brush.setStyle(Qt::SolidPattern);

    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawEllipse(controlPointPosition + curveViewPosition,
                         TIMELINE_CONTROL_POINT_RADIUS, TIMELINE_CONTROL_POINT_RADIUS);

}

void ControlPointgraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event)
{

}

bool ControlPointgraphicsItem::IsSelected()
{
    return _isSelected;
}

void ControlPointgraphicsItem::SetSelected(bool selected)
{
    _isSelected = selected;

    update();
}

ControlPoint *ControlPointgraphicsItem::GetControlPoint()
{
    return _controlPoint;
}

