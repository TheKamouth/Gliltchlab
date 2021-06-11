#ifndef TRACKGRAPHICSITEM_H
#define TRACKGRAPHICSITEM_H

#include "Track.h"

#include <QGraphicsItem>

class TrackGraphicsItem : public QGraphicsItem
{
public:
    TrackGraphicsItem(int index, Track * track);

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

    int TimelineTrackIndex();

private :
    Track * _track;

    int _timelineTrackIndex;
};

#endif // TRACKGRAPHICSITEM_H
