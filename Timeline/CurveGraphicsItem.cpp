#include "CurveGraphicsItem.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"
#include "TimeManager.h"
#include "TrackGraphicsItem.h"

#include <QRect>
#include <QPainter>
#include <QWidget>

CurveGraphicsItem::CurveGraphicsItem(Curve * curve, TrackGraphicsItem * trackGraphicsItem) :
    _curve(curve),
    _trackGraphicsItem(trackGraphicsItem)
{

}

QRectF CurveGraphicsItem::boundingRect() const
{
    int timelineTrackIndex = _trackGraphicsItem->TimelineTrackIndex();

    QRect wholeTrackRect = QRect(0,
                                 timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                 TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT);

    QRect curveRect = QRect(TIMELINE_TRACK_INFO_WIDTH,
                            timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                            TIMELINE_TRACK_MAX_DURATION_MS,
                            TIMELINE_TRACK_HEIGHT);

    return curveRect;
}

void CurveGraphicsItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    int timelineTrackIndex = _trackGraphicsItem->TimelineTrackIndex();

    QRect wholeTrackRect = QRect(0,
                                 timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                 TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT);

    QRect curveRect = QRect(TIMELINE_TRACK_INFO_WIDTH,
                            timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                            TIMELINE_TRACK_MAX_DURATION_MS,
                            TIMELINE_TRACK_HEIGHT);

    QRect lowerBoundRect = QRect(TIMELINE_TRACK_INFO_WIDTH,
                                 timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT+ TIMELINE_TRACK_HEIGHT / 2.0f,
                                 widget->width() - TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT / 2.0f);

    QRect upperBoundRect = QRect(TIMELINE_TRACK_INFO_WIDTH,
                                 timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                                 widget->width() - TIMELINE_TRACK_INFO_WIDTH,
                                 TIMELINE_TRACK_HEIGHT / 2.0f);

    painter->setRenderHint( QPainter::Antialiasing, true);

    QBrush painterBrush = painter->brush();
    painterBrush.setColor(BLACK);
    painterBrush.setStyle(Qt::SolidPattern);
    painter->setBrush(painterBrush);

    //FlowDataType type = _track->Type();
    //QColor typeTextColor = FlowData::TypeToColor(type);
    QString lowerBoundText = QString::number(_curve->LowerBound());
    QString upperBoundText = QString::number(_curve->UpperBound());

    painter->drawText( lowerBoundRect, lowerBoundText, QTextOption(Qt::AlignLeft | Qt::AlignBottom ) );
    painter->drawText( upperBoundRect, upperBoundText, QTextOption(Qt::AlignLeft | Qt::AlignTop ) );

    painterBrush.setColor(WHITE);
    painterBrush.setStyle(Qt::SolidPattern);
    //painter->drawRect(curveRect);

    // points and curve drawing
    // TODO
    QMap<float,ControlPoint>::iterator lastControlPointsIt = _curve->ControlPoints()->end() - 1;
    QMap<float,ControlPoint>::iterator controlPointsIt = _curve->ControlPoints()->begin();
    while( controlPointsIt != lastControlPointsIt)
    {
        float timeMs = controlPointsIt.key();
        float x = TIMELINE_TRACK_INFO_WIDTH + timeMs;

        ControlPoint controlPoint = controlPointsIt.value();
        float value = controlPoint.Value();

        QMap<float,ControlPoint>::iterator nextIt = controlPointsIt +1;
        float nextValuetimeMs = nextIt.key();
        float xNextPoint = TIMELINE_TRACK_INFO_WIDTH + nextValuetimeMs;

        ControlPoint nextControlPoint = nextIt.value();
        float nextValue = nextControlPoint.Value();

        // add margin
        float lowerBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT;
        float upperBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT;

        // alpha [0,1]
        float alpha = std::clamp( ( value - _curve->LowerBound() ) / _curve->Range(), 0.0f, 1.0f);
        float alphaNextValue = std::clamp( ( nextValue - _curve->LowerBound() ) / _curve->Range(), 0.0f, 1.0f);

        float y = alpha * upperBoundY + ( 1.0f - alpha ) * lowerBoundY;
        float yNextPoint = lowerBoundY - alphaNextValue * TIMELINE_TRACK_HEIGHT;

        painter->drawEllipse( x - 2, y - 2, 4, 4);

        switch( controlPoint.Mode())
        {
            case Constant:
                painter->setPen( QPen( QColor(0,0,0), 2, Qt::SolidLine));
                painter->drawLine(x, y, xNextPoint, y);
                break;
            case LinearInterp:
                painter->setPen( QPen( QColor(0,0,0), 2, Qt::SolidLine));
                painter->drawLine(x, y, xNextPoint, yNextPoint);
                break;
            case CubicInterp:
            {
                QPointF dir = QPointF(xNextPoint - x, yNextPoint - y);
                dir *= 0.5;
                dir.setY(0.0f);

                QPointF fromPinPos = QPointF(x, y);
                QPointF fromPinPosB = QPointF(x, y) + dir;
                QPointF toPinPos = QPointF(xNextPoint, yNextPoint);
                QPointF toPinPosB = QPointF(xNextPoint, yNextPoint) - dir;

                QPainterPath connectionPath;
                connectionPath.moveTo(fromPinPos);
                connectionPath.cubicTo(fromPinPosB, toPinPosB, toPinPos);
                //connectionPath.setFillRule(Qt::OddEvenFill);

                painterBrush.setColor(BLACK);
                painterBrush.setStyle(Qt::SolidPattern);

                // connection edge
                painter->setPen(QPen(BLACK, 2));
                painter->drawPath(connectionPath);
            }
                break;
            default:
                break;
        }

        controlPointsIt++;
    }

    // Cursor rect
    QRect cursorRect = QRect(TIMELINE_TRACK_INFO_WIDTH + TimeManager::Instance().Time(),
                                TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT * _trackGraphicsItem->TimelineTrackIndex(),
                                1,
                                TIMELINE_TRACK_HEIGHT);

    painter->setPen(QPen(BLACK, 1));
    painterBrush.setColor(BLACK);
    painterBrush.setStyle(Qt::SolidPattern);
    painter->drawRect(cursorRect);
}

ControlPoint * CurveGraphicsItem::AddPoint(QPointF scenePos)
{
    float timeMs = scenePos.x() - TIMELINE_TRACK_INFO_WIDTH;

    int timelineTrackIndex = _trackGraphicsItem->TimelineTrackIndex();
    float lowerBoundY = timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT + TIMELINE_TRACK_HEIGHT;
    float alpha = ( lowerBoundY - scenePos.y() ) / TIMELINE_TRACK_HEIGHT;

    float value = alpha * _curve->UpperBound() + ( 1.0f - alpha ) * _curve->LowerBound();

    //weird call
    ControlPoint * controlPoint = _curve->AddPoint(timeMs, ControlPoint(timeMs, value) );

    return  controlPoint;
}

Curve * CurveGraphicsItem::GetCurve()
{
    return _curve;
}

QList<ControlPointgraphicsItem *> * CurveGraphicsItem::GetControlPointsGraphicItem()
{
    return &_controlPointsGraphicItem;
}
