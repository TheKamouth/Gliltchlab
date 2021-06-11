#include "TrackGraphicsItem.h"

#include "TimelineConstants.h"
#include "ColorPalette.h"

#include "FlowGraph/FlowData.h"

#include <QPainter>

TrackGraphicsItem::TrackGraphicsItem(int index, Track * track) :
    _track(track),
    _timelineTrackIndex(index)
{

}

QRectF TrackGraphicsItem::boundingRect() const
{
    QRect wholeTrackRect = QRect(0,
                               _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);
    return wholeTrackRect;
}

void TrackGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRect wholeTrackRect = QRect(0,
                               _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_MAX_DURATION_MS + TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);

    QRect trackInfoRect = QRect(0,
                               _timelineTrackIndex * TIMELINE_TRACK_HEIGHT + TIMELINE_UPPER_RULE_HEIGHT,
                               TIMELINE_TRACK_INFO_WIDTH,
                               TIMELINE_TRACK_HEIGHT);

    painter->setRenderHint( QPainter::Antialiasing, true);

    QBrush painterBrush = painter->brush();
    painterBrush.setColor(GREY);
    painterBrush.setStyle(Qt::SolidPattern);

    painter->setBrush(painterBrush);
    painter->drawRect(trackInfoRect);

    painterBrush.setColor(BLACK);
    painter->drawText( trackInfoRect, _track->Name() );

    FlowDataType type = _track->Type();
    QColor typeTextColor = FlowData::TypeToColor(type);
    QString typeText = "\n" + FlowData::TypeString(type);

    painterBrush.setColor(typeTextColor);
    painter->drawText( trackInfoRect, typeText );
}

int TrackGraphicsItem::TimelineTrackIndex()
{
    return _timelineTrackIndex;
}
