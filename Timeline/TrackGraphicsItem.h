#ifndef TRACKGRAPHICSITEM_H
#define TRACKGRAPHICSITEM_H

#include "Track.h"

#include <QGraphicsItem>

class TrackGraphicsItem : public QGraphicsItem
{
public:
    TrackGraphicsItem();

    virtual QRectF boundingRect() const override;
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) override;

private :
    Track * _track;
};

#endif // TRACKGRAPHICSITEM_H
