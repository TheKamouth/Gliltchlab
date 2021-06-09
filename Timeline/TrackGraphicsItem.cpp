#include "TrackGraphicsItem.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"

#include <QPainter>

TrackGraphicsItem::TrackGraphicsItem()
{

}

QRectF TrackGraphicsItem::boundingRect() const
{
    //QPointF nodePosition = _node->FlowGraphNodePosition();
    int trackIndex = 0;

    QRect wholeTrackRect = QRect(0,
                               trackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_MAX_DURATION_SEC + TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);
    return wholeTrackRect;
}

void TrackGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    int trackIndex = 0;

    QRect wholeTrackRect = QRect(0,
                               trackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_MAX_DURATION_SEC + TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);

    QRect trackInfoRect = QRect(0,
                               trackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);

    painter->setRenderHint( QPainter::Antialiasing, true);

    QBrush painterBrush = painter->brush();
    painterBrush.setColor(WHITE);
    painterBrush.setStyle(Qt::SolidPattern);

    painter->setBrush(painterBrush);
    painter->drawRect(trackInfoRect);

    painterBrush.setColor(BLACK);
    painter->drawText( trackInfoRect,"Track " + QString::number(trackIndex) );
}
